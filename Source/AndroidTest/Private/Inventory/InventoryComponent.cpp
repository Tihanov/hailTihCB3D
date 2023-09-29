// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "Log.h"
#include "MainGameState.h"
#include "Inventory/InventorySlotsComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InventorySlots = CreateDefaultSubobject<UInventorySlotsComponent>(TEXT("InventorySlots"));
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
	check(GameState);
	InvDataTable = &GameState->InventoryDataTable;
	
	if(!InvDataTable)
	{
		ULog::Error("InvDataTable == nullptr", LO_Both);
	}
}

int32 /*CountOfNotAddedItems*/ UInventoryComponent::AddItem(FName RowName, int32 CountOfItems)
{
	const auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
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
		if (i->RowName == RowName && Row->MaxStackCount != i->Count)
		{
			auto TempAdd = FMath::Clamp(CountOfAdd, 0, Row->MaxStackCount - i->Count);
			i->Count += TempAdd;
			CountOfAdd -= TempAdd;
			OnItemChangedDelegate.Broadcast(i);
		}
	}
	const auto CountOfOverhead = FMath::Clamp(CountOfAdd - Row->MaxStackCount, 0, MAX_int32);
	Weight -= Row->WeightKg * CountOfOverhead;
	CountOfAdd -= CountOfOverhead;

	if (CountOfAdd == 0)
		return CountOfNotAddedItems_ToRet;
	const auto ToAdd = UInventoryItemDefaultInfo::Create ( RowName, CountOfAdd, *Row, Row->Other.InfoClass );
	if(InventoryArray.IsEmpty())
	{
		InventoryArray.Add(ToAdd);
		goto SKIP_FOR;
	}
	for(int32 i = InventoryArray.Num() - 1; i >= 0; --i)
	{
		if((*InventoryArray[i]) == (*ToAdd))
		{
			InventoryArray.Insert(ToAdd, i + 1);
			goto SKIP_FOR;
		}
	}
	for(int32 i = 0; i < InventoryArray.Num(); ++i)
	{
		if((*InventoryArray[i]) > (*ToAdd))
		{
			InventoryArray.Insert(ToAdd, i);
			goto SKIP_FOR;
		}
	}
	InventoryArray.Add(ToAdd);
	SKIP_FOR:
	
	OnItemAddedDelegate.Broadcast(ToAdd);

	if(CountOfOverhead > 0)
		AddItem(RowName, CountOfOverhead);
	
	return CountOfNotAddedItems_ToRet;
}

bool UInventoryComponent::AddUniqueItem(UInventoryItemDefaultInfo* ItemInfo)
{
	if(ItemInfo->Info.WeightKg + Weight > MaxWeight)
		return false;
	
	if(InventoryArray.IsEmpty())
	{
		InventoryArray.Add(ItemInfo);
		goto SKIP_FOR;
	}
	for(int32 i = InventoryArray.Num() - 1; i >= 0; --i)
	{
		if((*InventoryArray[i]) == (*ItemInfo))
		{
			InventoryArray.Insert(ItemInfo, i + 1);
			goto SKIP_FOR;
		}
	}
	for(int32 i = 0; i < InventoryArray.Num(); ++i)
	{
		if((*InventoryArray[i]) > (*ItemInfo))
		{
			InventoryArray.Insert(ItemInfo, i);
			goto SKIP_FOR;
		}
	}
	InventoryArray.Add(ItemInfo);
	SKIP_FOR:

	OnItemAddedDelegate.Broadcast(ItemInfo);
	return true;
}

void UInventoryComponent::PickUpItem(AInventoryItemBaseActor* ItemActor)
{
	if(!ItemActor)
	{
		ULog::Warning("ItemActor == nullptr");
		return;
	}


	if(ItemActor->ItemInfo->Info.IsItemUnique)
	{
		if(AddUniqueItem(ItemActor->ItemInfo))
			ItemActor->Destroy(true);
		return;
	}

	
	int CountOfNotAddedItems = AddItem(ItemActor->ItemInfo->RowName, ItemActor->ItemInfo->Count);
	if(CountOfNotAddedItems == ItemActor->ItemInfo->Count)
		return;

	if(CountOfNotAddedItems == 0)
		ItemActor->Destroy(true);
	else
		ItemActor->ItemInfo->Count = CountOfNotAddedItems;
}

void UInventoryComponent::TrashItem(UInventoryItemDefaultInfo* ItemStack, int32 CountToDel)
{
	bool IsUnique = ItemStack->Info.IsItemUnique;

	for(int32 i = InventoryArray.Num() - 1; i >= 0; --i)
	{
		if(IsUnique && InventoryArray[i] == ItemStack)
		{
			Weight -= InventoryArray[i]->Info.WeightKg * CountToDel; 
			InventoryArray.RemoveAt(i);
			OnItemTrashedDelegate.Broadcast(ItemStack);
			break;
		}
		if(!IsUnique && InventoryArray[i]->RowName == ItemStack->RowName)
		{
			auto ToSub = FMath::Clamp(CountToDel, 0, InventoryArray[i]->Count);
			Weight -= InventoryArray[i]->Info.WeightKg * ToSub; 
			CountToDel -= ToSub;
			InventoryArray[i]->Count -= ToSub;
			if(InventoryArray[i]->Count == 0)
			{
				OnItemTrashedDelegate.Broadcast(InventoryArray[i]);
				InventoryArray.RemoveAt(i);
			}
			else
				OnItemChangedDelegate.Broadcast(InventoryArray[i]);
			if(CountToDel == 0)
				break;
		}
	}
}

void UInventoryComponent::ThrowOutItem(UInventoryItemDefaultInfo* ItemStack, int32 CountToDel)
{
	TrashItem(ItemStack, CountToDel);

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
			GetWorld()->SpawnActor(ItemStack->Info.Other.Class, &Transform, Params));

	if(ItemStack->Info.IsItemUnique)
		Item->Init(ItemStack);
	else
		Item->InitWithRowName(ItemStack->RowName, CountToDel);
}

void UInventoryComponent::MoveItemToOtherInventory(UInventoryItemDefaultInfo* ItemStack,
	UInventoryComponent* OtherInventory)
{
	if(ItemStack->Count * ItemStack->Info.WeightKg + OtherInventory->Weight > OtherInventory->MaxWeight)
		return;
	InventoryArray.Remove(ItemStack);
	OnItemTrashedDelegate.Broadcast(ItemStack);
	
	OtherInventory->InventoryArray.Add(ItemStack);
	OtherInventory->OnItemAddedDelegate.Broadcast(ItemStack);
	OtherInventory->InventoryArray.Sort();
}

int32 UInventoryComponent::GetCountOfSpecificItemsByInfo(UInventoryItemDefaultInfo* ItemInfo) const
{
	if(ItemInfo->Info.IsItemUnique)
		return ItemInfo->Count;

	int32 RetCount = 0;
	bool IsFound = false;
	for (const auto ItemInfoIter : InventoryArray)
	{
		if(ItemInfo->RowName == ItemInfoIter->RowName)
		{
			RetCount += ItemInfoIter->Count;
			IsFound = true;
			continue;
		}
		if(IsFound)
			break;
	}
	return RetCount;
}

int32 UInventoryComponent::GetCountOfItemsByName(FName ItemName) const
{
	int32 RetCount = 0;
	bool IsFound = false;
	for (const auto ItemInfoIter : InventoryArray)
	{
		if(ItemName == ItemInfoIter->RowName)
		{
			RetCount += ItemInfoIter->Count;
			IsFound = true;
			continue;
		}
		if(IsFound)
			break;
	}
	return RetCount;
}

TArray<UInventoryItemDefaultInfo*> UInventoryComponent::GetItemsOfType(EInvItemType Type)
{
	if(Type == EInvItemType::All)
		return InventoryArray;
	TArray<UInventoryItemDefaultInfo*> ToRet;
	bool IsTypeFound = false;
	
	for (auto& i: InventoryArray)
	{
		if(i->Info.Type == Type)
		{
			ToRet.Add(i);
			IsTypeFound = true;
			continue;
		}
		if(IsTypeFound)
			break;
	}

	return ToRet;
}

int32 UInventoryComponent::GetSize() const
{
	return InventoryArray.Num();
}

int32 UInventoryComponent::GetCountOfItems() const
{
	int32 Count = 0;
	for (auto i : InventoryArray)
		Count += i->Count;
	return Count;
}
