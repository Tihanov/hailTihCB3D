// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/TasksOfQuestPartInstanceMenu.h"

#include "Quest/QuestAsset.h"
#include "Quest/QuestTask.h"

UTasksOfQuestPartInstanceMenu::UTasksOfQuestPartInstanceMenu()
{
	BaseInstanceClass = UQuestTask::StaticClass();
}

void UTasksOfQuestPartInstanceMenu::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	QuestAsset = InQuestAsset;
	Index = InIndex;

	const auto& Tasks = QuestAsset->Parts[Index].Tasks;
	TArray<UObject*> Instances;
	Instances.Reserve(Tasks.Num());
	for (const auto& Task : Tasks)
	{
		Instances.Add(Task);
	}

	Super::Init(Instances);
}

void UTasksOfQuestPartInstanceMenu::Save()
{
	Super::Save();
}
