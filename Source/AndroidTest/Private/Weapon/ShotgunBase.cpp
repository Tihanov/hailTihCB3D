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

	CurrentScatter = ItemSettings.Other.WeaponItemSettings.MinScatter;
	CurrentMagazineCapacity = ItemSettings.Other.WeaponItemSettings.MagazineCapacity;
}


void AShotgunBase::StartShooting_Implementation()
{
	Super::StartShooting_Implementation();
	if(CurrentMagazineCapacity <= 0)
	{
		ReloadWeapon_Implementation();
		return;
	}
	if(!CanWeaponShoot_Implementation())
		return;
	OnStartShootingDelegate.Broadcast(this);
	CurrentMagazineCapacity -= 1;

	FHitResult HitResult;
	bool IsDamageWasDone = false;
	TMap<AActor*, float> DamagedActorAndDamage;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End; // Implements in circle
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		ItemSettings.Other.WeaponItemSettings.ShotSound,
		RootMeshComponent->GetSocketLocation("ShootFrom"),
		FRotator::ZeroRotator);

	for(int i = 0; i < 7; ++i)
	{
		End = Start
			+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
			* ItemSettings.Other.WeaponItemSettings.ShotRange;
		UKismetSystemLibrary::LineTraceSingle(
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
				UKismetMathLibrary::FindLookAtRotation(ShootFromLocation, HitResult.ImpactPoint))
			* ItemSettings.Other.WeaponItemSettings.ShotRange * 1.1;
	
		auto Result = UKismetSystemLibrary::LineTraceSingle(
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
               		ItemSettings.Other.WeaponItemSettings.Damage,
               		FVector::ZeroVector /*TODO*/,
               		HitResult,
               		GetOwner()->GetInstigatorController(),
               		GetOwner(),
               		{});
		}
	}

	CurrentScatter = FMath::Clamp(
		CurrentScatter + ItemSettings.Other.WeaponItemSettings.ShotScatter,
		ItemSettings.Other.WeaponItemSettings.MinScatter,
		ItemSettings.Other.WeaponItemSettings.MaxScatter);
	IsShotDelay = true;
	OnMadeShotDelegate.Broadcast(this, IsDamageWasDone, {DamagedActorAndDamage});
}
void AShotgunBase::StopShooting_Implementation()
{
	Super::StopShooting_Implementation();
	OnStopShootingDelegate.Broadcast(this);
}
bool AShotgunBase::CanWeaponShoot_Implementation() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading && !IsShotDelay;
}

float AShotgunBase::GetWeaponScatter_Implementation() const
{
	return CurrentScatter;
}

void AShotgunBase::ReloadWeapon_Implementation()
{
	Super::ReloadWeapon_Implementation();
	IsWeaponInReloading = true;
}
bool AShotgunBase::IsWeaponReloading_Implementation() const
{
	return IsWeaponInReloading;
}
float AShotgunBase::GetCurrentWeaponReloadingTimeout_Implementation() const
{
	return CurrentReloadTimeout;
}
int AShotgunBase::GetMagazineCapacity_Implementation() const
{
	return CurrentMagazineCapacity;
}

void AShotgunBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - ItemSettings.Other.WeaponItemSettings.ScatterReductionInOneSec * DeltaTime,
		ItemSettings.Other.WeaponItemSettings.MinScatter,
		ItemSettings.Other.WeaponItemSettings.MaxScatter
	);
}

void AShotgunBase::TryReload(float DeltaTime)
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

void AShotgunBase::ShotDelay(float DeltaTime)
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

