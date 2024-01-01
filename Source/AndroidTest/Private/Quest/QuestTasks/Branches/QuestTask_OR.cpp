// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestTasks/Branches/QuestTask_OR.h"

#include "Log.h"
#include "Quest/QuestStructures.h"
#include "Utils/Utils.h"

void UQuestTask_OR::S_Init(APlayerController* InPlayerController)
{
	QuestManagerComponent = InPlayerController->GetComponentByClass<UQuestManagerComponent>();
	CHECK_ON_TRUE_JUST_RETURN(QuestManagerComponent == nullptr);
	CHECK_ON_TRUE_JUST_RETURN(QTask_A == nullptr || QTask_B == nullptr);
	
	QTask_A->OnTaskDoneDelegate.AddDynamic(this, &UQuestTask_OR::OnTaskDoneCallback);
	QTask_B->OnTaskDoneDelegate.AddDynamic(this, &UQuestTask_OR::OnTaskDoneCallback);

	QTask_A->S_Init(InPlayerController);
	QTask_B->S_Init(InPlayerController);

	Super::S_Init(InPlayerController);
}

void UQuestTask_OR::OnTaskDoneCallback(UQuestTask* Task)
{
	QTask_A->OnTaskDoneDelegate.RemoveAll(this);
	QTask_B->OnTaskDoneDelegate.RemoveAll(this);

	if(Task == QTask_A)
	{
		if(QCallback_A)
			QCallback_A->Do(QuestManagerComponent);
	}
	else if(Task == QTask_B)
	{
		if(QCallback_B)
			QCallback_B->Do(QuestManagerComponent);
	}
	else
	{
		CHECK_ON_TRUE_JUST_RETURN(!"UQuestTask_OR: Task != TaskA and Task != TaskB");
	}

	bIsDone = true;
	OnTaskDoneDelegate.Broadcast(this);
}
