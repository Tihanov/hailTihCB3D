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
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine = true)) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "IsMainQuest?") bool IsMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "{Id}: {DisplayName}")) TArray<FQuestPartInfo> Parts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FQuestRewardsInfo Rewards;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UQuestAsset*> QuestsToDoNext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced) TArray<UQuestCallback*> ToDoAfter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int> Memory; 
};
