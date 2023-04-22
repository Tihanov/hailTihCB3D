// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Inventory/InventoryItemBaseActor.h"
#include "QuestStructs.generated.h"

USTRUCT(BlueprintType)
struct FQuestSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "IsMainQuest?") bool bMainQuest;
};

USTRUCT(BlueprintType)
struct FQuestRewardsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ExpReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FInventoryItemInitStruct> ItemsReward;  	
};

USTRUCT(BlueprintType, DisplayName = "ComeToActor")
struct FQuestTaskComeToActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSoftObjectPtr<AActor> ActorToCameTo;
};

USTRUCT(BlueprintType)
struct FQuestTasksInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FQuestTaskComeToActor> ComeToActorTasks;	
};

USTRUCT(BlueprintType)
struct FQuestPartInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestTasksInfo Tasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestRewardsInfo Rewards;
};

USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText						DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText						Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestSettings				Settings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FQuestPartInfo>		QuestParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestRewardsInfo			Rewards;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName>				QuestsToDoNext;	
};
