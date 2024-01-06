// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstanceMenu.h"
#include "TasksOfQuestPartInstanceMenu.generated.h"

UCLASS()
class ANDROIDTEST_API UTasksOfQuestPartInstanceMenu : public UInstanceMenu
{
	GENERATED_BODY()

public:
	UTasksOfQuestPartInstanceMenu();
	
public:
	UFUNCTION(BlueprintCallable)
		void Init(UQuestAsset* InQuestAsset, int32 InIndex);

	virtual void Save() override;
	
private:
	UPROPERTY() UQuestAsset* QuestAsset = nullptr;
	int32 Index = 0;
};
