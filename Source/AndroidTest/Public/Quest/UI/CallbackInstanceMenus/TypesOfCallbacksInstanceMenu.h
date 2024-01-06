// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/UI/CallbackOfQuestInstanceMenu.h"
#include "TypesOfCallbacksInstanceMenu.generated.h"

UCLASS()
class ANDROIDTEST_API UBeforeCallbackOfTaskInstanceMenu : public UCallbackOfQuestInstanceMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void Init(UQuestAsset* InQuestAsset, int32 InIndex);
};

UCLASS()
class ANDROIDTEST_API UAfterCallbackOfTaskInstanceMenu : public UCallbackOfQuestInstanceMenu
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void Init(UQuestAsset* InQuestAsset, int32 InIndex);
};
