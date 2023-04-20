// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionInterface.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	None			UMETA(Display = "Default"),
	PickUp			UMETA(Display = "Pick Up"),
	NPCDialogue		UMETA(Display = "NPC Dialogue"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ANDROIDTEST_API IActionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		EActionType GetActionType();
};
