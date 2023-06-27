// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/InventoryStructures.h"
#include "InHandItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInHandItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANDROIDTEST_API IInHandItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InHand")
		void InitAsInHandItem();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InHand")
		void UseInHandItem(AActor* CausedBy);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InHand")
		void PassiveEffectOfInHandItem(AActor* EffectOn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InHand")
		bool DoesInHandItemHavePassiveEffect() const;
};
