// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShotgunBase.h"


#include "Log.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainGameState.h"


AShotgunBase::AShotgunBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShotgunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReduceScatter(DeltaTime);
	TryReload(DeltaTime);
	ShotDelay(DeltaTime);
}

void AShotgunBase::InitAsEquippedWeapon_Implementation(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName)
{
	Super::InitAsEquippedWeapon_Implementation(WeaponOwner, Options, ItemName);
	if(!RootMeshComponent->DoesSocketExist("ShootFrom"))
	{
		ULog::Warning("Add ShootFrom socket to Weapon Mesh");
		return;
	}
	RootMeshComponent->SetSimulatePhysics(false);
	RootMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentScatter = GetInfo<UShotgunInfo>()->MinScatter;
	CurrentMagazineCapacity = GetInfo<UShotgunInfo>()->MagazineCapacity;
}


void AShotgunBase::PullTheTrigger()
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
	TMap<AActor*, float> DamagedActorAndDamage;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End; // Implements in circle
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		GetInfo<UShotgunInfo>()->ShotSound,
		RootMeshComponent->GetSocketLocation("ShootFrom"),
		FRotator::ZeroRotator);

	for(int i = 0; i < 7; ++i)
	{
		End = Start
			+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
			* GetInfo<UShotgunInfo>()->ShotRange;
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
			* GetInfo<UShotgunInfo>()->ShotRange * 1.1;
	
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
			DamagedActorAndDamage
				.FindOrAdd(HitResult.GetActor())
				+= UGameplayStatics::ApplyPointDamage(
               		HitResult.GetActor(),
               		GetInfo<UShotgunInfo>()->Damage
					* (1.f - (HitResult.Distance / GetInfo<UShotgunInfo>()->ShotRange)),
               		FVector::ZeroVector /*TODO*/,
               		HitResult,
               		GetOwner()->GetInstigatorController(),
               		GetOwner(),
               		{});
		}
	}

	CurrentScatter = FMath::Clamp(
		CurrentScatter + GetInfo<UShotgunInfo>()->ShotScatter,
		GetInfo<UShotgunInfo>()->MinScatter,
		GetInfo<UShotgunInfo>()->MaxScatter);
	IsShotDelay = true;
	OnAttackDelegate.Broadcast(this, IsDamageWasDone, {DamagedActorAndDamage});
}
void AShotgunBase::ReleaseTheTrigger()
{
	Super::ReleaseTheTrigger();
	OnReleaseTheTriggerDelegate.Broadcast(this);
}
bool AShotgunBase::CanBeUsedNow() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading && !IsShotDelay;
}

TArray<AActor*> AShotgunBase::MakeTestAttack()
{
	return Super::MakeTestAttack();
}

float AShotgunBase::GetScatter() const
{
	return CurrentScatter;
}

bool AShotgunBase::CanBeReloaded() const
{
	return true;
}

void AShotgunBase::Reload()
{
	Super::Reload();
	IsWeaponInReloading = true;
}
bool AShotgunBase::IsReloading() const
{
	return IsWeaponInReloading;
}
float AShotgunBase::GetCurrentReloadingTimeout() const
{
	return CurrentReloadTimeout;
}
int AShotgunBase::GetMagazineCapacity() const
{
	return CurrentMagazineCapacity;
}

void AShotgunBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - GetInfo<UShotgunInfo>()->ScatterReductionInOneSec * DeltaTime,
		GetInfo<UShotgunInfo>()->MinScatter,
		GetInfo<UShotgunInfo>()->MaxScatter
	);
}

void AShotgunBase::TryReload(float DeltaTime)
{
	if(!IsWeaponInReloading)
		return;
	CurrentReloadTimeout = FMath::Clamp(
		CurrentReloadTimeout + DeltaTime,
		0.f,
		GetInfo<UShotgunInfo>()->ReloadTimeoutInSec
	);
	if(CurrentReloadTimeout >= GetInfo<UShotgunInfo>()->ReloadTimeoutInSec)
	{
		CurrentReloadTimeout = 0.f;
		IsWeaponInReloading = false;
		CurrentMagazineCapacity = GetInfo<UShotgunInfo>()->MagazineCapacity;
	}
}

void AShotgunBase::ShotDelay(float DeltaTime)
{
	if(!IsShotDelay)
		return;
	CurrentShotDelay = FMath::Clamp(
		CurrentShotDelay + DeltaTime,
		0.f,
		GetInfo<UShotgunInfo>()->ShotDelayInSec
	);
	if(CurrentShotDelay >= GetInfo<UShotgunInfo>()->ShotDelayInSec)
	{
		CurrentShotDelay = 0.f;
		IsShotDelay = false;
	}
}

