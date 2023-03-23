// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OpenInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void CloseInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OpenCloseInventory();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool PickUpItem(class AInventoryItemBaseActor* Item);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool ThrowOutItem(int32 Index);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		const UInventoryComponent* GetInventoryComponent() const;
};
