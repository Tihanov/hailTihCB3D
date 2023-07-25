// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskDoneDelegate, class UQuestTask*, Task);

UCLASS(BlueprintType, Blueprintable, Abstract, DefaultToInstanced, EditInlineNew, meta = (ShowWorldContextPin))
class ANDROIDTEST_API UQuestTask: public UObject 
{
	GENERATED_BODY()

public: /*Delegates*/
	UPROPERTY(BlueprintCallable, Category = "Delegates", DisplayName = "OnTaskDone")
		FTaskDoneDelegate OnTaskDoneDelegate;

public: /*Vars*/
	UPROPERTY(BlueprintReadOnly)
		class UQuestAsset* ParentQuestAsset;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Init(class APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsDone();
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		FText GetDisplayName();
};

