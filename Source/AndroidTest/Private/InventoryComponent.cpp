// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include <stdexcept>

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

bool UInventoryComponent::AddItem(FName RowName)
{
	FInvItemArray* ToInc = nullptr;
	const auto Row = InvDataTable->FindRow<FInvItemDataTable>(RowName, "");
	if(!Row)
	{
		ULog::Error("no RowName in InvDataTable", LO_Both);
		return false;
	}
	if (MaxWeight < Weight + Row->WeightKg)
		return false;

	for (auto& i : InventoryArray)
	{
		if (i.RowName == RowName && Row->MaxStackCount != i.Count)
			ToInc = &i;
	}

	if(!ToInc)
		InventoryArray.Add({ RowName, 1 });
	else 
		ToInc->Count++;
	Weight += Row->WeightKg;

	return true;
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

void UInventoryComponent::ThrowAwayItem(int32 Index, int32 Count)
{
	if(Index < 0 || Index >= InventoryArray.Num())
		return;
	const auto Row = InvDataTable->FindRow<FInvItemDataTable>(InventoryArray[Index].RowName, "");
	if (InventoryArray[Index].Count - Count <= 0)
	{
		Weight -= Row->WeightKg * InventoryArray[Index].Count;
		InventoryArray.RemoveAt(Index);
		return;
	}
	InventoryArray[Index].Count -= Count;
	Weight -= Row->WeightKg * Count;
}

