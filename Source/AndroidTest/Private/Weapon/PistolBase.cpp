// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PistolBase.h"

#include "Log.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainGameState.h"


APistolBase::APistolBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APistolBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReduceScatter(DeltaTime);
	TryReload(DeltaTime);
	ShotDelay(DeltaTime);
}

void APistolBase::InitAsEquippedWeapon_Implementation(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName)
{
	Super::InitAsEquippedWeapon_Implementation(WeaponOwner, Options, ItemName);
	if(!RootMeshComponent->DoesSocketExist("ShootFrom"))
	{
		ULog::Warning("Add ShootFrom socket to Weapon Mesh");
		return;
	}
	RootMeshComponent->SetSimulatePhysics(false);
	RootMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentScatter = GetInfo<UPistolInfo>()->MinScatter;
	CurrentMagazineCapacity = GetInfo<UPistolInfo>()->MagazineCapacity;
}


void APistolBase::PullTheTrigger()
{
	Super::PullTheTrigger();
	if(CurrentMagazineCapacity <= 0)
	{
		Reload();
		return;
	}
	if(!CanBeUsedNow())
		return;
	OnPullTheTriggerDelegate.Broadcast(this);
	CurrentMagazineCapacity -= 1;

	FHitResult HitResult;
	bool IsDamageWasDone = false;
	float DoneDamage = 0.f;
	AActor* DamagedActor = nullptr;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End = Start
		+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
		* GetInfo<UPistolInfo>()->ShotRange;
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		GetInfo<UPistolInfo>()->ShotSound,
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
		* GetInfo<UPistolInfo>()->ShotRange * 1.1;
	
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
				GetInfo<UPistolInfo>()->Damage
				* (1.f - (HitResult.Distance / GetInfo<UPistolInfo>()->ShotRange)),
				FVector::ZeroVector /*TODO*/,
				HitResult,
				GetOwner()->GetInstigatorController(),
				GetOwner(),
				{}
			);
	}

	CurrentScatter = FMath::Clamp(
		CurrentScatter + GetInfo<UPistolInfo>()->ShotScatter,
		GetInfo<UPistolInfo>()->MinScatter,
		GetInfo<UPistolInfo>()->MaxScatter);
	IsShotDelay = true;
	decltype(FDamagedActorsAndDamageProxyMap::DamagedActors) TempMap;
	if(IsDamageWasDone)
		TempMap.Add(DamagedActor, DoneDamage);
	OnAttackDelegate.Broadcast(this, IsDamageWasDone, {TempMap});
}

void APistolBase::ReleaseTheTrigger()
{
	Super::ReleaseTheTrigger();
	OnReleaseTheTriggerDelegate.Broadcast(this);
}

bool APistolBase::CanBeUsedNow() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading && !IsShotDelay;
}

TArray<AActor*> APistolBase::MakeTestAttack()
{
	FHitResult HitResult;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End = Start
		+ OwnerAimCameraRight->GetForwardVector()
		* GetInfo<UPistolInfo>()->ShotRange;

	auto Result = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>{this, GetOwner()},
			EDrawDebugTrace::None,
			HitResult,
			true);
	
	FVector ShootFromLocation = RootMeshComponent->GetSocketLocation("ShootFrom");
	Start = ShootFromLocation;
	End = Start
		+ UKismetMathLibrary::GetForwardVector(
			UKismetMathLibrary::FindLookAtRotation(ShootFromLocation, Result ? HitResult.ImpactPoint : End))
		* GetInfo<UPistolInfo>()->ShotRange * 1.1;
	
	Result = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>{this, GetOwner()},
			EDrawDebugTrace::None,
			HitResult,
			true);

	if(Result)
		return {HitResult.GetActor()};
	else
		return {};
}

float APistolBase::GetScatter() const
{
	return CurrentScatter;
}

bool APistolBase::CanBeReloaded() const
{
	return true;
}

void APistolBase::Reload()
{
	Super::Reload();
	IsWeaponInReloading = true;
}
bool APistolBase::IsReloading() const
{
	return IsWeaponInReloading;
}
float APistolBase::GetCurrentReloadingTimeout() const
{
	return CurrentReloadTimeout;
}
int APistolBase::GetMagazineCapacity() const
{
	return CurrentMagazineCapacity;
}

void APistolBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - GetInfo<UPistolInfo>()->ScatterReductionInOneSec * DeltaTime,
		GetInfo<UPistolInfo>()->MinScatter,
		GetInfo<UPistolInfo>()->MaxScatter
	);
}

void APistolBase::TryReload(float DeltaTime)
{
	if(!IsWeaponInReloading)
		return;
	CurrentReloadTimeout = FMath::Clamp(
		CurrentReloadTimeout + DeltaTime,
		0.f,
		GetInfo<UPistolInfo>()->ReloadTimeoutInSec
	);
	if(CurrentReloadTimeout >= GetInfo<UPistolInfo>()->ReloadTimeoutInSec)
	{
		CurrentReloadTimeout = 0.f;
		IsWeaponInReloading = false;
		CurrentMagazineCapacity = GetInfo<UPistolInfo>()->MagazineCapacity;
	}
}

void APistolBase::ShotDelay(float DeltaTime)
{
	if(!IsShotDelay)
		return;
	CurrentShotDelay = FMath::Clamp(
		CurrentShotDelay + DeltaTime,
		0.f,
		GetInfo<UPistolInfo>()->ShotDelayInSec
	);
	if(CurrentShotDelay >= GetInfo<UPistolInfo>()->ShotDelayInSec)
	{
		CurrentShotDelay = 0.f;
		IsShotDelay = false;
	}
}

