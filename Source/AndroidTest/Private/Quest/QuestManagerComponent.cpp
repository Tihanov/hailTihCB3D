// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManagerComponent.h"

#include "Log.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"


UQuestManagerComponent::UQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UQuestManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		TaskCheckTimerHandle,
		this,
		&UQuestManagerComponent::TaskCheck,
		0.2,
		true);
}

void UQuestManagerComponent::AddQuest(UQuestAsset* Quest)
{
	FQuestCompletingInfo Info;
	Info.QuestPart = 0;
	for(const auto& Task: Quest->Parts[0].Tasks)
		Info.TasksToComplete.Add(DuplicateObject(Task, nullptr), false);
	Info.QuestAsset = Quest;
	CurrentQuests.Add(Quest, Info);
}

void UQuestManagerComponent::TaskCheck()
{
	for (auto& CurrentQuest : CurrentQuests)
	{
		auto& QuestComplInfo = CurrentQuest.Value; 
		auto& TasksToComplete = QuestComplInfo.TasksToComplete;
		int CompletedTasks = 0;
		for (auto& Task : TasksToComplete)
		{
			if(!Task.Value)
				Task.Value = Task.Key->IsDone(Cast<APlayerController>(GetOwner()));
			CompletedTasks += static_cast<int>(Task.Value);
		}
		if(CompletedTasks == TasksToComplete.Num())
		{
			if(CurrentQuest.Key->Parts.Num() == QuestComplInfo.QuestPart + 1)
			{
				CompletedQuests.Add(CurrentQuest.Key);
				CurrentQuests.Remove(CurrentQuest.Key);
				continue;
			}
			QuestComplInfo.QuestPart += 1;
			QuestComplInfo.TasksToComplete.Empty();
			for(const auto& Task: QuestComplInfo.QuestAsset->Parts[QuestComplInfo.QuestPart].Tasks)
				QuestComplInfo.TasksToComplete.Add(DuplicateObject(Task, nullptr), false);
		}
	}
}

