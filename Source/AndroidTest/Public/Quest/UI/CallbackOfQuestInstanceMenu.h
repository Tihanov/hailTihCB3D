// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstanceMenu.h"
#include "CallbackOfQuestInstanceMenu.generated.h"

class UQuestCallback;

UCLASS()
class ANDROIDTEST_API UCallbackOfQuestInstanceMenu : public UInstanceMenu
{
	GENERATED_BODY()

public:
	UCallbackOfQuestInstanceMenu();
	
public:
	void Init(UQuestAsset* InQuestAsset, TArray<UQuestCallback*>& InQuestCallbacks);

	virtual void Save() override;
	
private:
	TArray<UQuestCallback*>* QuestCallbacks;
	UPROPERTY() UQuestAsset* QuestAsset;
};
