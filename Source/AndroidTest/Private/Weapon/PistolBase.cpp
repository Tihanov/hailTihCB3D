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

	CurrentScatter = ItemSettings.Other.WeaponItemSettings.MinScatter;
	CurrentMagazineCapacity = ItemSettings.Other.WeaponItemSettings.MagazineCapacity;
}

FInvItemDataTable APistolBase::GetWeaponSettings_Implementation() const
{
	return ItemSettings;
}

bool APistolBase::CanWeaponShoot_Implementation() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading && !IsShotDelay;
}

void APistolBase::StartShooting_Implementation()
{
	Super::StartShooting_Implementation();
	if(CurrentMagazineCapacity <= 0)
	{
		ReloadWeapon_Implementation();
		return;
	}
	if(!CanWeaponShoot_Implementation())
		return;
	CurrentMagazineCapacity -= 1;

	IsShootingNow = true;

	FHitResult HitResult;
	bool IsDamageWasDone = false;
	float DoneDamage = 0.f;
	AActor* DamagedActor = nullptr;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End = Start
		+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
		* ItemSettings.Other.WeaponItemSettings.ShotRange;
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	auto Result = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start, End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>{this, GetOwner()},
			MainGameState->DebugCameraTracersType,
			HitResult,
			true);
	if(Result)
	{
		FVector ShootFromLocation = RootMeshComponent->GetSocketLocation("ShootFrom");
		Start = ShootFromLocation;
		End = Start
			+ UKismetMathLibrary::GetForwardVector(
				UKismetMathLibrary::FindLookAtRotation(ShootFromLocation, HitResult.ImpactPoint))
			* ItemSettings.Other.WeaponItemSettings.ShotRange * 1.1;
		
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
					ItemSettings.Other.WeaponItemSettings.Damage,
					FVector::ZeroVector /*TODO*/,
					HitResult,
					GetOwner()->GetInstigatorController(),
					GetOwner(),
					{}
				);
		}
	}
	CurrentScatter = FMath::Clamp(
		CurrentScatter + ItemSettings.Other.WeaponItemSettings.ShotScatter,
		ItemSettings.Other.WeaponItemSettings.MinScatter,
		ItemSettings.Other.WeaponItemSettings.MaxScatter);
	IsShotDelay = true;
	OnMadeShotDelegate.Broadcast(this, IsDamageWasDone, DamagedActor, DoneDamage);
}
void APistolBase::StopShooting_Implementation()
{
	Super::StopShooting_Implementation();
}

float APistolBase::GetWeaponScatter_Implementation() const
{
	return CurrentScatter;
}

void APistolBase::ReloadWeapon_Implementation()
{
	Super::ReloadWeapon_Implementation();
	IsWeaponInReloading = true;
}
bool APistolBase::IsWeaponReloading_Implementation() const
{
	return IsWeaponInReloading;
}
float APistolBase::GetCurrentWeaponReloadingTimeout_Implementation() const
{
	return CurrentReloadTimeout;
}
int APistolBase::GetMagazineCapacity_Implementation() const
{
	return CurrentMagazineCapacity;
}

void APistolBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - ItemSettings.Other.WeaponItemSettings.ScatterReductionInOneSec * DeltaTime,
		ItemSettings.Other.WeaponItemSettings.MinScatter,
		ItemSettings.Other.WeaponItemSettings.MaxScatter
	);
}

void APistolBase::TryReload(float DeltaTime)
{
	if(!IsWeaponInReloading)
		return;
	CurrentReloadTimeout = FMath::Clamp(
		CurrentReloadTimeout + DeltaTime,
		0.f,
		ItemSettings.Other.WeaponItemSettings.ReloadTimeoutInSec
	);
	if(CurrentReloadTimeout >= ItemSettings.Other.WeaponItemSettings.ReloadTimeoutInSec)
	{
		CurrentReloadTimeout = 0.f;
		IsWeaponInReloading = false;
		CurrentMagazineCapacity = ItemSettings.Other.WeaponItemSettings.MagazineCapacity;
	}
}

void APistolBase::ShotDelay(float DeltaTime)
{
	if(!IsShotDelay)
		return;
	CurrentShotDelay = FMath::Clamp(
		CurrentShotDelay + DeltaTime,
		0.f,
		ItemSettings.Other.WeaponItemSettings.ShotDelayInSec
	);
	if(CurrentShotDelay >= ItemSettings.Other.WeaponItemSettings.ShotDelayInSec)
	{
		CurrentShotDelay = 0.f;
		IsShotDelay = false;
	}
}

