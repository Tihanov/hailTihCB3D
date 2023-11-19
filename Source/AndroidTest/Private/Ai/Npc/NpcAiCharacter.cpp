// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiCharacter.h"

#include "Log.h"
#include "MainGameState.h"
#include "Ai/Npc/NpcAiController.h"
#include "Ai/Npc/NpcMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DataTable.h"
#include "HealthPoints/HealthPointsComponent.h"
#include "Inventory/ChestComponent.h"
#include "Inventory/InventoryStructures.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Weapon/WeaponBase.h"
#include "Utils/Utils.h"


ANpcAiCharacter::ANpcAiCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UNpcMovementComponent::StaticClass()))
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"PerceptionStimuliSourceComponent");
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	
	AIControllerClass = ANpcAiController::StaticClass();
	GetCapsuleComponent()->ComponentTags.Add(FName(TEXT("MainCapsule")));

	HpComponent = CreateDefaultSubobject<UHealthPointsComponent>("HealthPointsComponent");
	HpComponent->SetMaxHealthPoints(HealPoints);
	HpComponent->SetHealthPoints(HpComponent->GetMaxHealthPoints());

	ChestComponent = CreateDefaultSubobject<UChestComponent>("ChestComponent");
}

void ANpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if(bHaveWeapon && !AimWithWeaponAnimation)
		ULog::Error(FString::Printf(TEXT("%s: bHaveWeapon == true, but AimWithWeaponAnimation == nullptr"), *GetName()),
		            LO_Both);
#endif
}

void ANpcAiCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(EquippedWeapon.IsValid())
	{
		const auto Weapon = EquippedWeapon.Get();
		Weapon->Destroy();
	}
}

FVector ANpcAiCharacter::GetPawnViewLocation() const
{
	if(HeadSocketName == NAME_None)
		return Super::GetPawnViewLocation();
	return GetMesh()->GetSocketLocation(HeadSocketName);
}

void ANpcAiCharacter::EquipWeapon(FName SocketName /*= NAME_None*/)
{
	if(EquippedWeapon.IsValid())
		UnequipWeapon();
	if(!bHaveWeapon)
	{
		ULog::Warning(FString::Printf(TEXT("%s cant equip weapon"), *GetName()), LO_Both);
		return;
	}
	if(SocketName == NAME_None)
		SocketName = WeaponSocket;
#if WITH_EDITOR
	check(GetMesh());
	if(!GetMesh()->DoesSocketExist(SocketName))
	{
		ULog::Warning(FString::Printf(TEXT("%s dont have socket %s"), *GetName(), *SocketName.ToString()), LO_Both);
		return;
	}	
#endif
	const auto MainGameState = GetWorld()->GetGameState<AMainGameState>();
	check(MainGameState);
	const auto ItemData = MainGameState->InventoryDataTable->FindRow<FInvItemDataTable>(WeaponItemName, TEXT(""));
	if(!ItemData)
	{
		ULog::Warning(FString::Printf(TEXT("%s cant find weapon with name %s"), *GetName(), *WeaponItemName.ToString()), LO_Both);
		return;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetScale3D(ItemData->Other.Scale);
	const auto Weapon = GetWorld()->SpawnActor<AWeaponBase>(ItemData->Other.WeaponInfo->WeaponClass,
	                                                        SpawnTransform);
	EquippedWeapon = Weapon;
	Weapon->InitAsEquippedWeapon(this, *ItemData, WeaponItemName);

	const FAttachmentTransformRules ATRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
	                                        EAttachmentRule::KeepRelative, true);
	Weapon->AttachToComponent(GetMesh(), ATRules, SocketName);
}

void ANpcAiCharacter::UnequipWeapon()
{
	if(EquippedWeapon.IsNull())
		return;
	const auto Weapon = EquippedWeapon.Get();
	Weapon->Destroy();
	EquippedWeapon.Reset();
}

void ANpcAiCharacter::SetCurrentState(const ENpcState NewState)
{
	const auto OldState = CurrentState;
	CurrentState = NewState;
	OnNpcChangeStateDelegate.Broadcast(NewState, OldState);
}

UBehaviorTree* ANpcAiCharacter::GetBehaviorByState(const ENpcState State) const
{
	if(StateBehavior.Contains(State))
		return StateBehavior[State];
	return nullptr;
}

UBehaviorTree* ANpcAiCharacter::GetCurrentBehavior() const
{
	return GetBehaviorByState(CurrentState);
}
