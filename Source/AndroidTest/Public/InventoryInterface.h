// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANDROIDTEST_API IInventoryInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		void OpenInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		void CloseInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		void OpenCloseInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		bool PickUpItem(class AInventoryItemBaseActor* Item);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		bool ThrowOutItem(int32 Index, int32 Count);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
		const UInventoryComponent* GetInventoryComponent() const;
};
