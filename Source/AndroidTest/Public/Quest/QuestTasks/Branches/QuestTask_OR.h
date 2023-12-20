// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestManagerComponent.h"
#include "Quest/QuestTask.h"
#include "QuestTask_OR.generated.h"

class UQuestCallback;

UCLASS(Abstract)
class ANDROIDTEST_API UQuestTask_OR : public UQuestTask
{
	GENERATED_BODY()

public:
	virtual void S_Init(APlayerController* InPlayerController) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options") UQuestTask* QTask_A;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options") UQuestCallback* QCallback_A;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options") UQuestTask* QTask_B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options") UQuestCallback* QCallback_B;

	UQuestManagerComponent* QuestManagerComponent;

private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "bIsDone?", meta = (AllowPrivateAccess = true))
		bool bIsDone = false;

	UFUNCTION() void OnTaskDoneCallback(UQuestTask* Task);
};
