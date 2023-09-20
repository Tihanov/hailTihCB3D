// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Log.h"
#include "Camera/CameraComponent.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Root Mesh");
	SetRootComponent(RootMeshComponent);
}

void AWeaponBase::InitAsEquippedWeapon_Implementation(APawn* WeaponOwner, FInvItemDataTable Options, FName ItemName)
{
	SetOwner(WeaponOwner);
	auto RightCameraArray = GetOwner()->GetComponentsByTag(UCameraComponent::StaticClass(), "AimCameraRight");
	ULog::Fatal_WithCondition(
		TEXT("Cant find 'Right Aim Camera' in Owner Object"),
		RightCameraArray.IsEmpty());
	OwnerAimCameraRight = Cast<UCameraComponent>(RightCameraArray[0]);
	ItemSettings = Options;

	RootMeshComponent->SetStaticMesh(ItemSettings.Mesh);
	RootMeshComponent->SetWorldScale3D(ItemSettings.Other.Scale);
}

FInvItemDataTable AWeaponBase::GetWeaponSettings() const
{
	return ItemSettings;
}

TArray<AActor*> AWeaponBase::MakeTestShoot_Implementation()
{
	return {};
}


bool AWeaponBase::CanWeaponShoot_Implementation() const
{
	return false;
}

void AWeaponBase::StartShooting_Implementation()
{
}

void AWeaponBase::StopShooting_Implementation()
{
}

float AWeaponBase::GetWeaponScatter_Implementation() const
{
	return 0.f;
}

void AWeaponBase::ReloadWeapon_Implementation()
{
}

bool AWeaponBase::IsWeaponReloading_Implementation() const
{
	return false;
}

float AWeaponBase::GetCurrentWeaponReloadingTimeout_Implementation() const
{
	return 0.f;
}

int AWeaponBase::GetMagazineCapacity_Implementation() const
{
	return 0;
}

