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
	for (const auto& Task : Quest->Parts[Info.QuestPart].Tasks)
		Info.TasksAndState.Add(DuplicateObject(Task, nullptr), false);
	CurrentQuestsAndInfo.Add(Quest, Info);
}

void UQuestManagerComponent::TaskCheck()
{
	for (auto& QuestAndInfo : CurrentQuestsAndInfo)
	{
		auto& Quest = QuestAndInfo.Key;
		auto& Info = QuestAndInfo.Value;
		auto& Tasks = Info.TasksAndState;	
		
		int CompletedTasks = 0;
		for (auto& Task : Tasks)
		{
			if(!Task.Value)
				Task.Value = Task.Key->IsDone(Cast<APlayerController>(GetOwner()));
			CompletedTasks += static_cast<int>(Task.Value);
		}
		if(CompletedTasks == Tasks.Num())
		{
			if(Quest->Parts.Num() == Info.QuestPart + 1)
			{
				CompletedQuests.Add(Quest);
				CurrentQuestsAndInfo.Remove(Quest);
				continue;
			}
			Info.QuestPart += 1;
			Info.TasksAndState.Empty();
			for (const auto& Task : Quest->Parts[Info.QuestPart].Tasks)
			  	Info.TasksAndState.Add(DuplicateObject(Task, nullptr), false);
		}
	}
}

