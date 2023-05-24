// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManagerComponent.h"

#include "Log.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/UObjectGlobals.h"
#include "Quest/QuestTask.h"


UQuestManagerComponent::UQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

TMap<UQuestTask*, bool>& UQuestManagerComponent::GetAllTasksFromQuest(UQuestAsset* Quest)
{
	const auto& Result = CurrentQuestsAndInfo.Find(Quest);
	ULog::Error_WithCondition("Cant find Quest", !Result);
	return Result->TasksAndState;
}

void UQuestManagerComponent::TrackQuest(UQuestAsset* ToTrack)
{
	auto OldQuest = TrackedQuest;
	TrackedQuest = ToTrack;
	OnTrackedQuestSetDelegate.Broadcast(ToTrack, OldQuest);
}

void UQuestManagerComponent::CheckOnAllTasksCompleted(UQuestAsset* QuestAsset)
{
	auto& QuestInfo = CurrentQuestsAndInfo[QuestAsset];
	int CompletedTasks = 0;
	for(const auto& Task : QuestInfo.TasksAndState)
		CompletedTasks += Task.Value;
	
	if(CompletedTasks == QuestInfo.TasksAndState.Num())
	{
		if(QuestAsset->Parts.Num() == QuestInfo.QuestPart + 1)
		{
			for (const auto& DoAfter : QuestAsset->ToDoAfter)
				DoAfter->Do();
			this->SetQuestComplete(QuestAsset);
			OnQuestStateChangedDelegate.Broadcast(QuestAsset);
			return;
		}
		QuestInfo.QuestPart += 1;
		QuestInfo.TasksAndState.Empty();
		UpdateQuestCompletingInfoToDoTasks(QuestAsset, QuestInfo);
		OnQuestStateChangedDelegate.Broadcast(QuestAsset);
	}
}

void UQuestManagerComponent::OnTaskDoneCallback(UQuestTask* Task)
{
	auto QuestInfo = CurrentQuestsAndInfo.Find(Task->ParentQuestAsset);
	ULog::Error_WithCondition("Cant find quest", !QuestInfo, LO_Both);
	auto TaskState = QuestInfo->TasksAndState.Find(Task);
	ULog::Error_WithCondition("Cant find task", !TaskState, LO_Both);
	*TaskState = true;
	OnQuestStateChangedDelegate.Broadcast(Task->ParentQuestAsset);

	CheckOnAllTasksCompleted(Task->ParentQuestAsset);
}

void UQuestManagerComponent::UpdateQuestCompletingInfoToDoTasks(UQuestAsset* Quest, FQuestCompletingInfo& ToInitInfo)
{
	for (const auto& Task : Quest->Parts[ToInitInfo.QuestPart].Tasks)
	{
		auto NewTask = DuplicateObject(Task, nullptr);
		ToInitInfo.TasksAndState.Add(NewTask, false);
		NewTask->Init(Cast<APlayerController>(GetOwner()));
		NewTask->OnTaskDoneDelegate.AddDynamic(this, &UQuestManagerComponent::OnTaskDoneCallback);
		NewTask->ParentQuestAsset = Quest;
		if(NewTask->IsDone())
		{
			const auto& QuestInfo = CurrentQuestsAndInfo[Quest];
			const int CurrentPart = QuestInfo.QuestPart;
			OnTaskDoneCallback(NewTask);
			if(CurrentPart != QuestInfo.QuestPart || CompletedQuests.Contains(Quest))
				break;
		}
	}
}

