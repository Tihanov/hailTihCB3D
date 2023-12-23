// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestManagerComponent.h"

#include "Log.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/UObjectGlobals.h"
#include "Quest/QuestTask.h"
#include "Utils/Utils.h"


UQuestManagerComponent::UQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestManagerComponent::AddQuest(UQuestAsset* Quest)
{
	if(CompletedQuests.Find(Quest) != INDEX_NONE || CurrentQuestsAndInfo.Find(Quest))
		return;

	CurrentQuestsAndInfo.Add(Quest, {});
	FQuestCompletingInfo& Info = CurrentQuestsAndInfo[Quest];
	Info.QuestPart = 0;
	Info.Memory = Quest->Memory;
	UpdateQuestCompletingInfoToDoTasks(Quest, Info);

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

void UQuestManagerComponent::SetQuestVariable(UQuestAsset* QuestAsset, FName VarName, int Value)
{
	CHECK_RETURN_ON_FAIL(!QuestAsset);
	const auto CompletingInfo = CurrentQuestsAndInfo.Find(QuestAsset);
	CHECK_RETURN_ON_FAIL(!CompletingInfo);

	CompletingInfo->Memory[VarName] = Value;
	OnQuestVariableChangedDelegate.Broadcast(this, QuestAsset, VarName, Value);
}

int UQuestManagerComponent::GetQuestVariable(UQuestAsset* QuestAsset, FName VarName, bool& OutExist) const
{
	OutExist = false;
	
	CHECK_RETURN(!QuestAsset, return INT_MIN);
	const auto CompletingInfo = CurrentQuestsAndInfo.Find(QuestAsset);
	CHECK_RETURN(!CompletingInfo, return INT_MIN);

	const auto ToRet = CompletingInfo->Memory.Find(VarName);
	OutExist = !!ToRet;
	return OutExist ? *ToRet : INT_MIN;
}

int UQuestManagerComponent::GetQuestVariable(UQuestAsset* QuestAsset, FName VarName) const
{
	CHECK_RETURN(!QuestAsset, return INT_MIN);
	const auto CompletingInfo = CurrentQuestsAndInfo.Find(QuestAsset);
	CHECK_RETURN(!CompletingInfo, return INT_MIN);

	const auto ToRet = CompletingInfo->Memory.Find(VarName);
	return !!ToRet ? *ToRet : INT_MIN;
}

bool UQuestManagerComponent::DoesQuesVariableExists(UQuestAsset* QuestAsset, FName VarName) const
{
	CHECK_RETURN(!QuestAsset, return false);
	const auto CompletingInfo = CurrentQuestsAndInfo.Find(QuestAsset);
	CHECK_RETURN(!CompletingInfo, return false);

	return CompletingInfo->Memory.Contains(VarName);
}

void UQuestManagerComponent::CheckOnAllTasksCompleted(UQuestAsset* QuestAsset)
{
	auto& QuestInfo = CurrentQuestsAndInfo[QuestAsset];
	const auto PrevPart = QuestAsset->Parts[QuestInfo.QuestPart];

	int CompletedTasks = 0;
	for(const auto& Task : QuestInfo.TasksAndState)
		CompletedTasks += Task.Value;
	
	if(CompletedTasks == QuestInfo.TasksAndState.Num())
	{
		// If the current part is the last part of quest
		if(QuestAsset->Parts.Num() == QuestInfo.QuestPart + 1)
		{
			for (const auto& DoAfter : QuestAsset->ToDoAfter)
				DoAfter->Do(this);
			this->SetQuestComplete(QuestAsset);
			OnQuestStateChangedDelegate.Broadcast(QuestAsset);

			CallCallbacksOfPart_After(PrevPart);
			
			return;
		}
		QuestInfo.QuestPart += 1;
		QuestInfo.TasksAndState.Empty();
		UpdateQuestCompletingInfoToDoTasks(QuestAsset, QuestInfo);
		OnQuestStateChangedDelegate.Broadcast(QuestAsset);
		
		CallCallbacksOfPart_After(PrevPart);
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
	// Call Before Callbacks
	const auto CurrentPart = Quest->Parts[ToInitInfo.QuestPart];
	for (const auto& BeforeCallback : CurrentPart.BeforeCallbacks)
	{
		BeforeCallback->Do(this);
	}

	// Init Quest Part Tasks
	for (const auto& Task : CurrentPart.Tasks)
	{
		auto NewTask = DuplicateObject(Task, nullptr);
		ToInitInfo.TasksAndState.Add(NewTask, false);
		NewTask->S_Init(Cast<APlayerController>(GetOwner()));
		NewTask->OnTaskDoneDelegate.AddDynamic(this, &UQuestManagerComponent::OnTaskDoneCallback);
		NewTask->ParentQuestAsset = Quest;
		if(NewTask->IsDone())
		{
			const auto& QuestInfo = CurrentQuestsAndInfo[Quest];
			const int CurrentPartIndex = QuestInfo.QuestPart;
			OnTaskDoneCallback(NewTask);
			if(CurrentPartIndex != QuestInfo.QuestPart || CompletedQuests.Contains(Quest))
				break;
		}
	}
}

void UQuestManagerComponent::CallCallbacksOfPart_After(const FQuestPartInfo& Part)
{
	for (const auto& AfterCallback : Part.AfterCallbacks)
	{
		CHECK_RETURN(!AfterCallback, continue;);
		AfterCallback->Do(this);
	}
}

