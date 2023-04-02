// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Log.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!InvDataTable)
	{
		ULog::Error("InvDataTable == nullptr", LO_Both);
	}
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
