// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Npc/NpcAiCharacter.h"

#include "Log.h"
#include "MainGameState.h"
#include "Utils/UtilsStructs.h"
#include "Ai/Npc/NpcAiController.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryStructures.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Weapon/WeaponBase.h"


ANpcAiCharacter::ANpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"PerceptionStimuliSourceComponent");
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	
	AIControllerClass = ANpcAiController::StaticClass();
	GetCapsuleComponent()->ComponentTags.Add(FName(TEXT("MainCapsule")));
}

void ANpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FGenericTeamId ANpcAiCharacter::GetGenericTeamId() const
{
	return TI_DefaultNpc;
}

void ANpcAiCharacter::EquipWeapon(FName SocketName /*= TEXT("PistolSocket")*/)
{
	if(EquippedWeapon.IsValid())
		UnequipWeapon();
	if(!bHaveWeapon)
	{
		ULog::Warning(FString::Printf(TEXT("%s cant equip weapon"), *GetName()), LO_Both);
		return;
	}
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
	const auto Weapon = GetWorld()->SpawnActor<AWeaponBase>(ItemData->Other.WeaponItemSettings.WeaponClass,
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
