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

void UInventorySlotsComponent::SetCountOfItemsOnSlot(ESlotType Slot, int32 Count)
{
	Slots.FindOrAdd(Slot, {}).Array.Init(nullptr, Count);
}

void UInventorySlotsComponent::SetItemIntoSlot(ESlotType Slot, int32 Index, UInventoryItemDefaultInfo* Item)
{
	if(!Slots.Contains(Slot) || !Slots[Slot].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return;
	}

	
	Slots[Slot].Array[Index] = Item;
	if(Slots[Slot].EquippedItemIndex == Index)
		OnItemEquipsFromSlotDelegate.Broadcast(Slot, Slots[Slot]);
	OnSlotsUpdatesDelegate.Broadcast(Slot, Slots[Slot]);
}

void UInventorySlotsComponent::UnsetItemFromSlot(ESlotType Slot, int32 Index)
{
	if(!Slots.Contains(Slot) || !Slots[Slot].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return;
	}
	
	Slots[Slot].Array[Index] = nullptr;
	OnSlotsUpdatesDelegate.Broadcast(Slot, Slots[Slot]);
}

FItemsAndEquippedItemIndex UInventorySlotsComponent::GetItemsFromSlot(ESlotType Slot) const
{
	if(!Slots.Contains(Slot))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return {};
	}

	return Slots[Slot];
}

void UInventorySlotsComponent::EquipItemFromSlot(ESlotType Slot, int32 Index)
{
	if(!Slots.Contains(Slot) || !Slots[Slot].Array.IsValidIndex(Index))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return;
	}

	Slots[Slot].EquippedItemIndex = Index;
	OnItemEquipsFromSlotDelegate.Broadcast(Slot, Slots[Slot]);
}

void UInventorySlotsComponent::UnequipItemFromSlot(ESlotType Slot)
{
	if(!Slots.Contains(Slot))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return;
	}
	Slots[Slot].EquippedItemIndex = -1;
	OnItemUnequipsFromSlotDelegate.Broadcast(Slot);
}

UInventoryItemDefaultInfo* UInventorySlotsComponent::GetEquippedItemFromSlot(ESlotType Slot) const
{
	if(!Slots.Contains(Slot))
	{
		ULog::Error(
			FString::Printf(TEXT("Cant find %s slot in slots map"), *UEnum::GetValueAsString(Slot)),
			LO_Both);
		return nullptr;
	}
	if(Slots[Slot].EquippedItemIndex == -1)
		return nullptr;
	return Slots[Slot].Array[Slots[Slot].EquippedItemIndex];
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
