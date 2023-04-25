// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/InventoryItemBaseActor.h"
#include "QuestStructures.generated.h"

class UQuestTask;



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////    REWARD INFO:
////////////////////////////////////////////////////////



USTRUCT(BlueprintType)
struct FQuestRewardsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ExpReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FInventoryItemInitStruct> ItemsReward;  	
};



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////	QUEST PART INFO:
////////////////////////////////////////////////////////



USTRUCT(BlueprintType)
struct FQuestPartInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced) TArray<UQuestTask*> Tasks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FQuestRewardsInfo Rewards;
};


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////





/**
 * DONT USE, JUST TO DEFINE STRUCTURES
 */
UCLASS(DisplayName = "DONTUSE_QuestStructures") class ANDROIDTEST_API UQuestStructures : public UObject{GENERATED_BODY()};
