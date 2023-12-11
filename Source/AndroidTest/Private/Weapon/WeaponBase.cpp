// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Log.h"
#include "Camera/CameraComponent.h"
#include "Weapon/AutomaticWeaponBase.h"
#include "Weapon/PistolBase.h"
#include "Weapon/ShotgunBase.h"


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
	RootMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FInvItemDataTable AWeaponBase::GetWeaponSettings() const
{
	return ItemSettings;
}

void AWeaponBase::PullTheTrigger()
{
}

void AWeaponBase::ReleaseTheTrigger()
{
}

bool AWeaponBase::CanBeUsedNow() const
{
	return false;
}

TArray<AActor*> AWeaponBase::MakeTestAttack()
{
	return {};
}

float AWeaponBase::GetScatter() const
{
	return 0;
}

bool AWeaponBase::CanBeReloaded() const
{
	return false;
}

void AWeaponBase::Reload()
{
}

bool AWeaponBase::IsReloading() const
{
	return false;
}

float AWeaponBase::GetCurrentReloadingTimeout() const
{
	return 0.f;
}

int AWeaponBase::GetMagazineCapacity() const
{
	return 0;
}

