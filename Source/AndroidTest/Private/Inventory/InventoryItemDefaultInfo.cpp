// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemDefaultInfo.h"

#include "MainGameState.h"

UInventoryItemDefaultInfo* UInventoryItemDefaultInfo::Create(FName RowName, int32 Count,
                                                             FInvItemDataTable Info, TSubclassOf<UInventoryItemDefaultInfo> Class)
{
	const auto ToRet = NewObject<UInventoryItemDefaultInfo>(GetTransientPackage(), Class);
	ToRet->RowName = RowName;
	ToRet->Count = Count;
	ToRet->Info = Info;
	return ToRet;
}

bool operator==(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	return Self.RowName == Other.RowName;
}

bool operator!=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	return !(Self == Other);
}

bool operator<(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	const auto SelfType = UEnum::GetValueAsString(Self.Info.Type);
	const auto OtherType = UEnum::GetValueAsString(Other.Info.Type);
	const auto Result = SelfType.Compare(OtherType, ESearchCase::IgnoreCase);
	if(Result != 0)
		return Result < 0;
		
	const auto SelfName = Self.Info.DisplayName;
	const auto OtherName = Other.Info.DisplayName;

	return SelfName.CompareToCaseIgnored(OtherName) < 0;
}

bool operator<=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	return !(Other < Self);
}

bool operator>(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	return Other < Self;
}

bool operator>=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other)
{
	return !(Self < Other);
}
