// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AutomaticWeaponBase.h"

#include "Log.h"
#include "MainGameState.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/Utils.h"


AAutomaticWeaponBase::AAutomaticWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAutomaticWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReduceScatter(DeltaTime);
	TryReload(DeltaTime);
	ShotDelay(DeltaTime);
}

void AAutomaticWeaponBase::InitAsEquippedWeapon_Implementation(APawn* WeaponOwner, FInvItemDataTable Options,
	FName ItemName)
{
	Super::InitAsEquippedWeapon_Implementation(WeaponOwner, Options, ItemName);
	
	if(!RootMeshComponent->DoesSocketExist("ShootFrom"))
	{
		ULog::Warning("Add ShootFrom socket to Weapon Mesh");
		return;
	}
	RootMeshComponent->SetSimulatePhysics(false);
	RootMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentScatter = GetInfo<UAutomaticWeaponInfo>()->MinScatter;
	CurrentMagazineCapacity = GetInfo<UAutomaticWeaponInfo>()->MagazineCapacity;
}

void AAutomaticWeaponBase::PullTheTrigger()
{
	Super::PullTheTrigger();
	if(CurrentMagazineCapacity <= 0)
	{
		return;
	}
	if(!CanBeUsedNow())
		return;
	IsShotDelay = true;
	GetWorld()
		->GetTimerManager()
		.SetTimer(ShootTimerHandler,
			this, &AAutomaticWeaponBase::TryShoot,
			GetInfo<UAutomaticWeaponInfo>()->ShotDelayInSec,
			true, 0);
	OnPullTheTriggerDelegate.Broadcast(this);
}

void AAutomaticWeaponBase::ReleaseTheTrigger()
{
	Super::ReleaseTheTrigger();
	GetWorld()
		->GetTimerManager()
		.ClearTimer(ShootTimerHandler);
	OnReleaseTheTriggerDelegate.Broadcast(this);
}

bool AAutomaticWeaponBase::CanBeUsedNow() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading;
}

TArray<AActor*> AAutomaticWeaponBase::MakeTestAttack() // TODO
{
	return Super::MakeTestAttack();
}

float AAutomaticWeaponBase::GetScatter() const
{
	return CurrentScatter;
}

bool AAutomaticWeaponBase::CanBeReloaded() const
{
	return true;
}

void AAutomaticWeaponBase::Reload()
{
	IsWeaponInReloading = true;
}
bool AAutomaticWeaponBase::IsReloading() const
{
	return IsWeaponInReloading;
}
float AAutomaticWeaponBase::GetCurrentReloadingTimeout() const
{
	return CurrentReloadTimeout;
}
int AAutomaticWeaponBase::GetMagazineCapacity() const
{
	return CurrentMagazineCapacity;
}

void AAutomaticWeaponBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - GetInfo<UAutomaticWeaponInfo>()->ScatterReductionInOneSec * DeltaTime,
		GetInfo<UAutomaticWeaponInfo>()->MinScatter,
		GetInfo<UAutomaticWeaponInfo>()->MaxScatter
	);
}

void AAutomaticWeaponBase::TryReload(float DeltaTime)
{
	if(!IsWeaponInReloading)
		return;
	CurrentReloadTimeout = FMath::Clamp(
		CurrentReloadTimeout + DeltaTime,
		0.f,
		GetInfo<UAutomaticWeaponInfo>()->ReloadTimeoutInSec
	);
	if(CurrentReloadTimeout >= GetInfo<UAutomaticWeaponInfo>()->ReloadTimeoutInSec)
	{
		CurrentReloadTimeout = 0.f;
		IsWeaponInReloading = false;
		CurrentMagazineCapacity = GetInfo<UAutomaticWeaponInfo>()->MagazineCapacity;
	}
}


void AAutomaticWeaponBase::TryShoot()
{
	if(CurrentMagazineCapacity <= 0)
	{
		Reload();
		ReleaseTheTrigger();
		return;
	}
	if(!CanBeUsedNow())
		return;
	CurrentMagazineCapacity -= 1;

	const auto AutomaticWeaponInfo = GetInfo<UAutomaticWeaponInfo>();
	CHECK_RETURN_ON_FAIL(!AutomaticWeaponInfo);
	
	FHitResult HitResult;
	bool IsDamageWasDone = false;
	float DoneDamage = 0.f;
	AActor* DamagedActor = nullptr;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End = Start
		+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
		* AutomaticWeaponInfo->ShotRange;
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		AutomaticWeaponInfo->ShotSound,
		RootMeshComponent->GetSocketLocation("ShootFrom"),
		FRotator::ZeroRotator);

	auto Result = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>{this, GetOwner()},
			MainGameState->DebugCameraTracersType,
			HitResult,
			true);

	FVector ShootFromLocation = RootMeshComponent->GetSocketLocation("ShootFrom");
	Start = ShootFromLocation;
	End = Start
		+ UKismetMathLibrary::GetForwardVector(
			UKismetMathLibrary::FindLookAtRotation(ShootFromLocation, Result ? HitResult.ImpactPoint : End))
		* AutomaticWeaponInfo->ShotRange * 1.1;
		
	Result = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>{this, GetOwner()},
			MainGameState->DebugWeaponTracersType,
			HitResult,
			true);
	if(Result)
	{
		IsDamageWasDone = true;
		DamagedActor = HitResult.GetActor();
		DoneDamage =
			UGameplayStatics::ApplyPointDamage(
				HitResult.GetActor(),
				AutomaticWeaponInfo->Damage
				* (1.f - (HitResult.Distance / AutomaticWeaponInfo->ShotRange)),
				FVector::ZeroVector /*TODO*/,
				HitResult,
				GetOwner()->GetInstigatorController(),
				GetOwner(),
				{}
			);
	}

	CurrentScatter = FMath::Clamp(
		CurrentScatter + AutomaticWeaponInfo->ShotScatter,
		AutomaticWeaponInfo->MinScatter,
		AutomaticWeaponInfo->MaxScatter);
	decltype(FDamagedActorsAndDamageProxyMap::DamagedActors) TempMap;
	if(IsDamageWasDone)
		TempMap.Add(DamagedActor, DoneDamage);
	OnAttackDelegate.Broadcast(this, IsDamageWasDone, {TempMap});
}

void AAutomaticWeaponBase::ShotDelay(float DeltaTime)
{
	if(!IsShotDelay)
		return;
	CurrentShotDelay = FMath::Clamp(
		CurrentShotDelay + DeltaTime,
		0.f,
		GetInfo<UAutomaticWeaponInfo>()->ShotDelayInSec
	);
	if(CurrentShotDelay >= GetInfo<UAutomaticWeaponInfo>()->ShotDelayInSec)
	{
		CurrentShotDelay = 0.f;
		IsShotDelay = false;
	}
}

