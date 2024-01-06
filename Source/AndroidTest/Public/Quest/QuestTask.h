// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TitleInterface.h"
#include "UObject/Object.h"
#include "QuestTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskDoneDelegate, class UQuestTask*, Task);

UCLASS(BlueprintType, Blueprintable, Abstract, DefaultToInstanced, EditInlineNew, meta = (ShowWorldContextPin))
class ANDROIDTEST_API UQuestTask
	: public UObject
	, public ITitleInterface 
{
	GENERATED_BODY()

public: /*Delegates*/
	UPROPERTY(BlueprintCallable, Category = "Delegates", DisplayName = "OnTaskDone")
		FTaskDoneDelegate OnTaskDoneDelegate;

public: /*Vars*/
	UPROPERTY(BlueprintReadOnly)
		class UQuestAsset* ParentQuestAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Title = FText::FromString(TEXT("BaseTask"));

public:
	UFUNCTION(BlueprintCallable)
		virtual void S_Init(class APlayerController* PlayerController) { BP_Init(PlayerController); };
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Init(class APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsDone();

	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
		FText GetDisplayName();

	virtual FText GetTitle() const override { return Title; };
};

