// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/InventoryItemBaseActor.h"
#include "QuestAsset.generated.h"

USTRUCT(BlueprintType)
struct FQuestRewardsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ExpReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FInventoryItemInitStruct> ItemsReward;  	
};

UCLASS(BlueprintType, Blueprintable, Abstract, DefaultToInstanced, EditInlineNew)
class UTaskType: public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsDone(class APlayerController* PlayerController);
};

USTRUCT(BlueprintType)
struct FQuestPartInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced) TArray<UTaskType*> Tasks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestRewardsInfo Rewards;
};

UCLASS(BlueprintType)
class ANDROIDTEST_API UQuestAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "IsMainQuest?") bool IsMain;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FQuestPartInfo> Parts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FQuestRewardsInfo Rewards;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<UQuestAsset*> QuestsToDoNext;	
};
