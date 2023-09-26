// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStructures.h"
#include "UObject/Object.h"
#include "InventoryItemDefaultInfo.generated.h"

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class UInventoryItemDefaultInfo : public UObject 
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FName RowName = TEXT("None");
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 Count = -1;
	UPROPERTY(BlueprintReadWrite) FInvItemDataTable Info;

	// FOR INSTANCED INIT ONLY
	UPROPERTY(BlueprintReadOnly, EditAnywhere) bool bInitInstanced = false;
	
	static UInventoryItemDefaultInfo* Create(FName RowName, int32 Count, FInvItemDataTable Info,
		TSubclassOf<UInventoryItemDefaultInfo> Class = UInventoryItemDefaultInfo::StaticClass());

	friend bool operator==(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
	friend bool operator!=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
	friend bool operator<(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
	friend bool operator<=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
	friend bool operator>(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
	friend bool operator>=(const UInventoryItemDefaultInfo& Self, const UInventoryItemDefaultInfo& Other);
};
