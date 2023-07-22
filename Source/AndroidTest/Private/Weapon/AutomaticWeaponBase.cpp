// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AutomaticWeaponBase.h"

#include "Log.h"
#include "MainGameState.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AAutomaticWeaponBase::AAutomaticWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAutomaticWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReduceScatter(DeltaTime);
	TryReload(DeltaTime);
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

	CurrentScatter = ItemSettings.Other.WeaponItemSettings.MinScatter;
	CurrentMagazineCapacity = ItemSettings.Other.WeaponItemSettings.MagazineCapacity;
}

void AAutomaticWeaponBase::StartShooting_Implementation()
{
	Super::StartShooting_Implementation();
	if(CurrentMagazineCapacity <= 0)
	{
		ReloadWeapon_Implementation();
		return;
	}
	if(!CanWeaponShoot_Implementation())
		return;
	GetWorld()
		->GetTimerManager()
		.SetTimer(ShootTimerHandler,
			this, &AAutomaticWeaponBase::TryShoot,
			ItemSettings.Other.WeaponItemSettings.ShotDelayInSec,
			true, 0);
	OnStartShootingDelegate.Broadcast(this);
}

void AAutomaticWeaponBase::StopShooting_Implementation()
{
	Super::StopShooting_Implementation();
	GetWorld()
		->GetTimerManager()
		.ClearTimer(ShootTimerHandler);
	OnStopShootingDelegate.Broadcast(this);
}

bool AAutomaticWeaponBase::CanWeaponShoot_Implementation() const
{
	return CurrentMagazineCapacity > 0 && !IsWeaponInReloading && !IsShotDelay;
}





float AAutomaticWeaponBase::GetWeaponScatter_Implementation() const
{
	return CurrentScatter;
}

void AAutomaticWeaponBase::ReloadWeapon_Implementation()
{
	IsWeaponInReloading = true;
}
bool AAutomaticWeaponBase::IsWeaponReloading_Implementation() const
{
	return IsWeaponInReloading;
}
float AAutomaticWeaponBase::GetCurrentWeaponReloadingTimeout_Implementation() const
{
	return CurrentReloadTimeout;
}
int AAutomaticWeaponBase::GetMagazineCapacity_Implementation() const
{
	return CurrentMagazineCapacity;
}

void AAutomaticWeaponBase::ReduceScatter(float DeltaTime)
{
	CurrentScatter = FMath::Clamp(
		CurrentScatter - ItemSettings.Other.WeaponItemSettings.ScatterReductionInOneSec * DeltaTime,
		ItemSettings.Other.WeaponItemSettings.MinScatter,
		ItemSettings.Other.WeaponItemSettings.MaxScatter
	);
}

void AAutomaticWeaponBase::TryReload(float DeltaTime)
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

void AAutomaticWeaponBase::TryShoot()
{
	if(CurrentMagazineCapacity <= 0)
	{
		ReloadWeapon_Implementation();
		StopShooting_Implementation();
		return;
	}
	if(!CanWeaponShoot_Implementation())
		return;
	CurrentMagazineCapacity -= 1;

	FHitResult HitResult;
	bool IsDamageWasDone = false;
	float DoneDamage = 0.f;
	AActor* DamagedActor = nullptr;
	FVector Start = OwnerAimCameraRight->GetComponentLocation();
	FVector End = Start
		+ UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwnerAimCameraRight->GetForwardVector(), CurrentScatter)
		* ItemSettings.Other.WeaponItemSettings.ShotRange;
	auto MainGameState = Cast<AMainGameState>(GetWorld()->GetGameState());

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		ItemSettings.Other.WeaponItemSettings.ShotSound,
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
	decltype(FDamagedActorsAndDamageProxyMap::DamagedActors) TempMap;
	TempMap.Add(DamagedActor, DoneDamage);
	OnMadeShotDelegate.Broadcast(this, IsDamageWasDone, {TempMap});
}

void AAutomaticWeaponBase::ShotDelay(float DeltaTime)
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

