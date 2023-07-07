// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "Log.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!InvDataTable)
	{
		ULog::Error("InvDataTable == nullptr", LO_Both);
	}
}

int32 /*CountOfNotAddedItems*/ UInventoryComponent::AddItem(FName RowName, int32 CountOfItems)
{
	const auto Row = InvDataTable->FindRow<FInvItemDataTable>(RowName, "");
	if(!Row)
	{
		ULog::Error("no RowName in InvDataTable", LO_Both);
		return CountOfItems;
	}
	auto CountOfAdd = FMath::Clamp(static_cast<int32>(FMath::Floor(
		(MaxWeight - Weight) / Row->WeightKg)), 0, CountOfItems);
	const int32 CountOfNotAddedItems_ToRet = CountOfItems - CountOfAdd;
	if(CountOfAdd == 0)
		return CountOfNotAddedItems_ToRet;
	Weight += Row->WeightKg * CountOfAdd;

	for (auto& i : InventoryArray)
	{
		if (i.RowName == RowName && Row->MaxStackCount != i.Count)
		{
			auto TempAdd = FMath::Clamp(CountOfAdd, 0, Row->MaxStackCount - i.Count);
			i.Count += TempAdd;
			CountOfAdd -= TempAdd;
		}
	}

	if (CountOfAdd == 0)
		return CountOfNotAddedItems_ToRet;
	InventoryArray.Add({ RowName, CountOfAdd });

	return CountOfNotAddedItems_ToRet;
}

void UInventoryComponent::PickUpItem(AInventoryItemBaseActor* ItemActor)
{
	if(!ItemActor)
	{
		ULog::Warning("ItemActor == nullptr");
		return;
	}

	int CountOfNotAddedItems = AddItem(ItemActor->RowName, ItemActor->CountOf);
	if(CountOfNotAddedItems == ItemActor->CountOf)
		return;

	OnItemPickedUpDelegate.Broadcast(ItemActor->RowName, ItemActor->CountOf - CountOfNotAddedItems);
	
	if(CountOfNotAddedItems == 0)
		ItemActor->Destroy(true);
	else
		ItemActor->CountOf = CountOfNotAddedItems;
}

bool UInventoryComponent::TrashItem(int32 Index, int32 Count, FName& RowName)
{
	if(Index < 0 || Index >= InventoryArray.Num())
		return false;
	RowName = InventoryArray[Index].RowName;

	const auto Row = InvDataTable->FindRow<FInvItemDataTable>(InventoryArray[Index].RowName, "");
	if (InventoryArray[Index].Count - Count <= 0)
	{
		Weight -= Row->WeightKg * InventoryArray[Index].Count;
		InventoryArray.RemoveAt(Index);
		return true;
	}
	InventoryArray[Index].Count -= Count;
	Weight -= Row->WeightKg * Count;

	return true;
}

void UInventoryComponent::ThrowOutItem(int32 Index, int32 Count)
{
	FName RowName;
	if(!TrashItem(Index, Count, RowName))
	{
		ULog::Warning("Cannot throw out item");
		return;
	}

	auto ItemDTR = InvDataTable->FindRow<FInvItemDataTable>(RowName, "");
	if(!ItemDTR)
	{
		ULog::Warning("Cannot find Item Data Table Row by Row Name");
		return;
	}

	FTransform Transform;
	if(const auto Controller = Cast<APlayerController>(GetOwner()))
	{
		const auto Actor = Controller->GetPawn();
		if(Actor)
			Transform = Actor->GetTransform();
	}
	else
		Transform = GetOwner()->GetActorTransform();
	Transform.SetRotation(FQuat{0,0,0,0});
	Transform.SetScale3D(FVector::OneVector);
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto Item =
		Cast<AInventoryItemBaseActor>(
			GetWorld()->SpawnActor(ItemDTR->Other.Class, &Transform, Params));

	Item->Init({
		RowName,
		InvDataTable,
		Count
	});

	OnItemThrowOutDelegate.Broadcast(Item, Index);
}

int32 UInventoryComponent::GetSize() const
{
	return InventoryArray.Num();
}

int32 UInventoryComponent::GetCountOfItems() const
{
	int32 Count = 0;
	for (auto i : InventoryArray)
		Count += i.Count;
	return Count;
}

FInvItemArray UInventoryComponent::GetItemOnIndex(int32 Index, bool& DoesExist) const
{
	if(InventoryArray.IsValidIndex(Index))
	{
		DoesExist = true;
		return InventoryArray[Index];
	}
	DoesExist = false;
	return {};
}

void UInventoryComponent::SetWeaponToSlot(int32 SlotIndex, const FInvItemArray& Weapon)
{
	if(SlotIndex < 0 || SlotIndex >= WeaponSlots.Num())
	{
		ULog::Warning(FString::Printf(TEXT("SlotIndex must by > 0 and < %i"), WeaponSlots.Num()));
		return;
	}

	WeaponSlots[SlotIndex] = Weapon;
	OnWeaponSlotsUpdatedDelegate.Broadcast(this, {GetWeaponSlots()});
}

void UInventoryComponent::RemoveWeaponFromSlot(int32 SlotIndex)
{
	if(SlotIndex < 0 || SlotIndex >= WeaponSlots.Num())
	{
		ULog::Warning(FString::Printf(TEXT("SlotIndex must by > 0 and < %i"), WeaponSlots.Num()));
		return;
	}
	WeaponSlots[SlotIndex].Reset();
	OnWeaponSlotsUpdatedDelegate.Broadcast(this, {GetWeaponSlots()});
}

FInvItemArray UInventoryComponent::GetWeaponFromSlot(int32 SlotIndex, bool& DoesWeaponSetInSlot) const
{
	if(SlotIndex < 0 || SlotIndex >= WeaponSlots.Num())
	{
		ULog::Warning(FString::Printf(TEXT("SlotIndex must by > 0 and < %i"), WeaponSlots.Num()));
		DoesWeaponSetInSlot = false;
		return {};
	}
	DoesWeaponSetInSlot = WeaponSlots[SlotIndex].IsSet();
	return WeaponSlots[SlotIndex].Get({});
}

TArray<FInvItemArray> UInventoryComponent::GetWeaponSlots() const
{
	TArray<FInvItemArray> ToRet;
	ToRet.SetNum(WeaponSlots.Num());
	for(int i = 0; i < WeaponSlots.Num(); ++i)
		ToRet[i] = WeaponSlots[i].Get({});
	return ToRet;
}

void UInventoryComponent::EquipWeaponFromSlot(int32 SlotIndex)
{
	if(SlotIndex < 0 || SlotIndex >= WeaponSlots.Num())
	{
		ULog::Warning(FString::Printf(TEXT("SlotIndex must by > 0 and < %i"), WeaponSlots.Num()));
		return;
	}

	if(!WeaponSlots[SlotIndex].IsSet())
	{
		UnequipWeapon();
		return;
	}

	EquippedWeaponSlotIndex = SlotIndex;
	OnEquipWeaponDelegate.Broadcast(
		this,
		WeaponSlots[SlotIndex].Get({}),
		!WeaponSlots[SlotIndex].IsSet());
}

void UInventoryComponent::UnequipWeapon()
{
	EquippedWeaponSlotIndex.Reset();
	OnUnequipWeaponDelegate.Broadcast(this);
}

int32 UInventoryComponent::GetEquippedWeaponSlot(bool& IsSomeWeaponSlotEquipped) const
{
	IsSomeWeaponSlotEquipped = EquippedWeaponSlotIndex.IsSet();
	return EquippedWeaponSlotIndex.Get(-1);
}

FInvItemArray UInventoryComponent::GetEquippedWeaponInfo(bool& IsWeaponEquipped) const
{
	IsWeaponEquipped = EquippedWeaponSlotIndex.IsSet();
	if(IsWeaponEquipped)
	{
		IsWeaponEquipped = WeaponSlots[EquippedWeaponSlotIndex.GetValue()].IsSet();
		return WeaponSlots[EquippedWeaponSlotIndex.GetValue()].Get({});
	}
	return {};
}

