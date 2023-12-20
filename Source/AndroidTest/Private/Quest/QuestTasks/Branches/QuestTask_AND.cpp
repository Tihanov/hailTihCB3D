// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestTasks/Branches/QuestTask_AND.h"

#include "Log.h"
#include "Utils/Utils.h"

void UQuestTask_AND::S_Init(APlayerController* InPlayerController)
{
	for (const auto& Task : Tasks)
	{
		CHECK_RETURN_ON_FAIL(Task == nullptr);
		Task->OnTaskDoneDelegate.AddDynamic(this, &UQuestTask_AND::OnTaskDoneCallback);
		Task->S_Init(InPlayerController);
	}
	
	Super::S_Init(InPlayerController);
}

void UQuestTask_AND::OnTaskDoneCallback(UQuestTask* Task)
{
	DoneTaskCount += 1;

	if(DoneTaskCount == Tasks.Num())
	{
		bIsDone = true;
		OnTaskDoneDelegate.Broadcast(this);
	}
}
