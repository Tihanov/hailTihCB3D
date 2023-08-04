// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySlotsComponent.h"

#include "Inventory/InventoryComponent.h"
#include "Log.h"


UInventorySlotsComponent::UInventorySlotsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventorySlotsComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto InventoryComponent = Cast<UInventoryComponent>(
		GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
	check(InventoryComponent);
	InventoryComponent->OnItemTrashedDelegate.AddDynamic(this,
		&UInventorySlotsComponent::OnItemFromInventoryTrashed);
}

void UInventorySlotsComponent::SetCountOfItemsOnSlot(ESlotType SlotType, int32 Count)
{
	Slots.FindOrAdd(SlotType, {}).Array.Init(nullptr, Count);
}

void UInventorySlotsComponent::SetItemIntoSlot(ESlotType SlotType, int32 Index, UInventoryItemDefaultInfo* Item)
{
	if(!Slots.Contains(SlotType) || !Slots[SlotType].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return;
	}

	
	Slots[SlotType].Array[Index] = Item;
	if(Slots[SlotType].EquippedItemIndex == Index)
		OnItemEquipsFromSlotDelegate.Broadcast(SlotType, Slots[SlotType]);
	OnSlotsUpdatesDelegate.Broadcast(SlotType, Slots[SlotType]);
}

void UInventorySlotsComponent::UnsetItemFromSlot(ESlotType SlotType, int32 Index)
{
	if(!Slots.Contains(SlotType) || !Slots[SlotType].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return;
	}
	
	Slots[SlotType].Array[Index] = nullptr;
	OnSlotsUpdatesDelegate.Broadcast(SlotType, Slots[SlotType]);
}

FItemsAndEquippedItemIndex UInventorySlotsComponent::GetItemsFromSlotsOfType(ESlotType SlotType) const
{
	if(!Slots.Contains(SlotType))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return {};
	}

	return Slots[SlotType];
}

void UInventorySlotsComponent::EquipItemFromSlot(ESlotType SlotType, int32 Index)
{
	if(!Slots.Contains(SlotType) || !Slots[SlotType].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return;
	}

	Slots[SlotType].EquippedItemIndex = Index;
	OnItemEquipsFromSlotDelegate.Broadcast(SlotType, Slots[SlotType]);
}

void UInventorySlotsComponent::UnequipItemFromSlot(ESlotType SlotType)
{
	if(!Slots.Contains(SlotType))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return;
	}
	Slots[SlotType].EquippedItemIndex = -1;
	OnItemUnequipsFromSlotDelegate.Broadcast(SlotType);
}

UInventoryItemDefaultInfo* UInventorySlotsComponent::GetEquippedItemFromSlotsOfType(ESlotType SlotType) const
{
	if(!Slots.Contains(SlotType))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return nullptr;
	}
	if(Slots[SlotType].EquippedItemIndex == -1)
		return nullptr;
	return Slots[SlotType].Array[Slots[SlotType].EquippedItemIndex];
}

int32 UInventorySlotsComponent::GetEquippedSlotIndexOfSlotType(ESlotType SlotType) const
{
	if(!Slots.Contains(SlotType))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(SlotType)),
			LO_Both);
		return -1;
	}
	return Slots[SlotType].EquippedItemIndex;
}

void UInventorySlotsComponent::OnItemFromInventoryTrashed(UInventoryItemDefaultInfo* ItemInfo)
{
	for (auto& Slot : Slots)
	{
		auto& EquippedItemIndex = Slot.Value.EquippedItemIndex;
		int32 Index = 0;
		for (auto& Item : Slot.Value.Array)
		{
			if(Item == nullptr)
				continue;
			if(Item == ItemInfo)
			{
				Item = nullptr;
				OnSlotsUpdatesDelegate.Broadcast(Slot.Key, Slot.Value);
				if(Index == EquippedItemIndex)
				{
					EquippedItemIndex = -1;
					OnItemUnequipsFromSlotDelegate.Broadcast(Slot.Key);
				}
				return;
			}
			
			Index += 1;
		}
	}
}
