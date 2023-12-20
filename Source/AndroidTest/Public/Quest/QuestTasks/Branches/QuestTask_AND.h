// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/QuestTask.h"
#include "QuestTask_AND.generated.h"

UCLASS(Abstract)
class ANDROIDTEST_API UQuestTask_AND : public UQuestTask
{
	GENERATED_BODY()

public:
	virtual void S_Init(APlayerController* InPlayerController) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Category = "Options") TArray<UQuestTask*> Tasks;

private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "bIsDone?", meta = (AllowPrivateAccess = true))
		bool bIsDone = false;
	int32 DoneTaskCount = 0;

	UFUNCTION() void OnTaskDoneCallback(UQuestTask* Task);
};
