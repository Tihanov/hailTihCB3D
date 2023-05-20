// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Engine/DataTable.h"
#include "Inventory/InventoryItemBaseActor.h"

#include "InventoryStructures.generated.h"

UENUM(BlueprintType)
enum class EInvItemType : uint8
{
	Default				UMETA(Display = "Default")
};

USTRUCT(BlueprintType)
struct FInvItemDataTableOptional
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AInventoryItemBaseActor>	Class = AInventoryItemBaseActor::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector									Scale = { 1,1,1 };
};

USTRUCT(BlueprintType)
struct FInvItemDataTable : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText				DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText				Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EInvItemType		Type			= EInvItemType::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) class UStaticMesh*	Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) class UTexture2D*	Ico;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float				WeightKg		= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float				BuyPrice		= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float				SellPrice		= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int					MaxStackCount	= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FInvItemDataTableOptional Other;
};

/**
 * DONT USE, JUST TO DEFINE STRUCTURES
 */
UCLASS(DisplayName = "DONTUSE_InventoryStructures") class ANDROIDTEST_API UInventoryStructures : public UObject{GENERATED_BODY() };