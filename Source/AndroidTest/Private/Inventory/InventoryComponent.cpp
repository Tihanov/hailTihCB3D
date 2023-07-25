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

	if (CountOfAdd == 0)
		return CountOfNotAddedItems_ToRet;
	InventoryArray.Add(CreateInventoryItemInfo( RowName, CountOfAdd ));
	OnItemAddedDelegate.Broadcast(InventoryArray.Last());

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

	if(CountOfNotAddedItems == 0)
		ItemActor->Destroy(true);
	else
		ItemActor->CountOf = CountOfNotAddedItems;
}

bool UInventoryComponent::TrashItem(int32 Index, int32 Count, FName& RowName)
{
	if(Index < 0 || Index >= InventoryArray.Num())
		return false;
	RowName = InventoryArray[Index]->RowName;

	const auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(InventoryArray[Index]->RowName, "");
	if (InventoryArray[Index]->Count - Count <= 0)
	{
		Weight -= Row->WeightKg * InventoryArray[Index]->Count;
		OnItemTrashedDelegate.Broadcast(InventoryArray[Index]);
		InventoryArray[Index]->ConditionalBeginDestroy();
		InventoryArray.RemoveAt(Index);
		InventorySlots->CheckAllSlotsOnValid();
		return true;
	}
	InventoryArray[Index]->Count -= Count;
	Weight -= Row->WeightKg * Count;
	OnItemChangedDelegate.Broadcast(InventoryArray[Index]);
	
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

	auto ItemDTR = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
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
		Count
	});
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
