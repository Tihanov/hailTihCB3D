// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Quest/QuestStructures.h"
#include "QuestAsset.generated.h"


UCLASS(BlueprintType)
class ANDROIDTEST_API UQuestAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine = true)) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "IsMainQuest?") bool IsMain;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FQuestPartInfo> Parts;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FQuestRewardsInfo Rewards;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<UQuestAsset*> QuestsToDoNext;	
};
