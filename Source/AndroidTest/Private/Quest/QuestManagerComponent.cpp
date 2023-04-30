// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManagerComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"
#include "Quest/QuestTask.h"


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
	if(CompletedQuests.Find(Quest) != INDEX_NONE || CurrentQuestsAndInfo.Find(Quest))
		return;
	FQuestCompletingInfo Info;
	Info.QuestPart = 0;
	UpdateQuestCompletingInfoToDoTasks(Quest, Info);
	CurrentQuestsAndInfo.Add(Quest, Info);

	OnAddNewQuestDelegate.Broadcast(Quest);
}

bool UQuestManagerComponent::SetQuestComplete(UQuestAsset* Quest)
{
	auto QuestAndInfo = CurrentQuestsAndInfo.Find(Quest);
	if(!QuestAndInfo)
		return false;
	CompletedQuests.Add(Quest);
	CurrentQuestsAndInfo.Remove(Quest);
	OnQuestCompleteDelegate.Broadcast(Quest);
	return true;
}

FQuestPartInfo UQuestManagerComponent::GetCurrentPartFromQuest(UQuestAsset* Quest, bool& Exist)
{
	const auto& Result = CurrentQuestsAndInfo.Find(Quest);
	if(!Result)
	{
		Exist = false;
		return {};
	}
	Exist = true;
	return Quest->Parts[Result->QuestPart];
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
				Task.Value = Task.Key->IsDone();
			CompletedTasks += static_cast<int>(Task.Value);
		}
		if(CompletedTasks == Tasks.Num())
		{
			if(Quest->Parts.Num() == Info.QuestPart + 1)
			{
				this->SetQuestComplete(Quest);
				continue;
			}
			Info.QuestPart += 1;
			Info.TasksAndState.Empty();
			UpdateQuestCompletingInfoToDoTasks(Quest, Info);
		}
	}
}

void UQuestManagerComponent::UpdateQuestCompletingInfoToDoTasks(UQuestAsset* Quest, FQuestCompletingInfo& ToInitInfo)
{
	for (const auto& Task : Quest->Parts[ToInitInfo.QuestPart].Tasks)
	{
		auto NewTask = DuplicateObject(Task, nullptr);
		ToInitInfo.TasksAndState.Add(NewTask, false);
		NewTask->Init(Cast<APlayerController>(GetOwner()));
	}
}

