// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/Components/AIShootComponent.h"

#include "Log.h"
#include "Ai/Enemy/NpcEnemyController.h"
#include "Ai/Npc/NpcAiCharacter.h"
#include "Utils/Utils.h"


UAIShootComponent::UAIShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAIShootComponent::StartShooting()
{
	SetupShooting();
	StartShootingImplementation();
}

void UAIShootComponent::StopShooting()
{
	CHECK_ON_TRUE_JUST_RETURN(Weapon.IsNull());

	const auto Character = GetNpcAiCharacter();
	CHECK_ON_TRUE_JUST_RETURN(Character == nullptr);
	Character->StopAnimMontage();
	
	SetShootingState(EShootingState::None);
	StopShootingImplementation();
}

void UAIShootComponent::SetShootingState(EShootingState NewShootingState)
{
	ShootingState = NewShootingState;
	OnChangeShootingStateDelegate.Broadcast(this);
}

void UAIShootComponent::StopShootingViaReloading()
{
	CHECK_ON_TRUE_JUST_RETURN(Weapon.IsNull());

	/*
	 * TODO RELOADING ANIMATION
	 */
	const auto Character = GetNpcAiCharacter();
	CHECK_ON_TRUE_JUST_RETURN(Character == nullptr);
	Character->StopAnimMontage();
	
	Weapon->Reload();
	SetShootingState(EShootingState::Reloading);
	Weapon->OnReloadingFinishedDelegate.AddDynamic(this, &UAIShootComponent::OnReloadingFinishedCallback);
	StopShootingImplementation();
}

ANpcEnemyController*  UAIShootComponent::GetNpcEnemyController() const
{
	const auto Controller = GetOwner<ANpcEnemyController>();
	check(Controller);
	return Controller;
}

ANpcAiCharacter* UAIShootComponent::GetNpcAiCharacter() const
{
	const auto Character = GetNpcEnemyController()->GetPawn<ANpcAiCharacter>();
	return Character;
}

void UAIShootComponent::SetupShooting()
{
	const auto Character = GetNpcAiCharacter();
	CHECK_ON_TRUE_JUST_RETURN(Character == nullptr);

	const auto SPWeapon = Character->GetEquippedWeapon();
	CHECK_ON_TRUE_JUST_RETURN(SPWeapon.IsNull());

	Weapon = SPWeapon;
	WeaponShootingType = SPWeapon->GetInfo()->GetType();
	CHECK_ON_TRUE_JUST_RETURN(WeaponShootingType == EWeaponShootingType::Unknown);
}

void UAIShootComponent::StartShootingImplementation()
{
	CHECK_ON_TRUE_JUST_RETURN(Weapon.IsNull());
	const auto Info = Weapon->GetInfo();
	CHECK_ON_TRUE_JUST_RETURN(Info == nullptr);

	const auto Character = GetNpcAiCharacter();
	CHECK_ON_TRUE_JUST_RETURN(Character == nullptr);
	Character->PlayAnimMontage(Character->GetAimWithWeaponAnimation());
	
	switch (WeaponShootingType)
	{
	case EWeaponShootingType::Single:
		{
			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UAIShootComponent::ShootSingleCallback,
			                                       Info->ShotDelayInSec, true, 0.f);
			break;
		}
	case EWeaponShootingType::Auto:
		{
			GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &UAIShootComponent::ShootAutoCallback,
												   Info->ShotDelayInSec, true, 0.f);
		}
	default:
		CHECK_ON_TRUE_JUST_RETURN( __LINE__ && "AIShootComponent" );
	}
	SetShootingState(EShootingState::Shooting);
}

void UAIShootComponent::StopShootingImplementation()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
}

void UAIShootComponent::ShootSingleCallback()
{
	CHECK_ON_TRUE_JUST_RETURN(!Weapon.IsValid());
	if(Weapon->GetMagazineCapacity() <= 0)
	{
		StopShootingViaReloading();
		return;
	}

	if(GetNpcEnemyController()->GetHostileActor() || !bShootOnlyIfSeeHostileActor)
	{
		Weapon->PullTheTrigger();
		Weapon->ReleaseTheTrigger();
	}
}

void UAIShootComponent::ShootAutoCallback()
{
	// TODO
}

void UAIShootComponent::OnReloadingFinishedCallback(AWeaponBase* WeaponBase)
{
	ensure(WeaponBase);
	Weapon->OnReloadingFinishedDelegate.RemoveDynamic(this, &UAIShootComponent::OnReloadingFinishedCallback);
	StartShootingImplementation();
}

