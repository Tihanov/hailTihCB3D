// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/QuestAsset.h"
#include "QuestManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddNewQuestDelegate, UQuestAsset*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestCompleteDelegate, UQuestAsset*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStateChangedDelegate, UQuestAsset*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTrackedQuestSetDelegate, UQuestAsset*, NewQuest, UQuestAsset*, OldQuest);

USTRUCT(BlueprintType)
struct FQuestCompletingInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) int QuestPart = 0;
	UPROPERTY(BlueprintReadWrite) TMap<UQuestTask*, bool> TasksAndState;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestManagerComponent();

public:
	UFUNCTION(BlueprintCallable)
		void AddQuest(UQuestAsset* Quest);
	UFUNCTION(BlueprintPure)
		UPARAM(DisplayName = "Part Info") FQuestPartInfo
			GetCurrentPartFromQuest(
				UQuestAsset* Quest,
				UPARAM(DisplayName = "DoesExist?") bool& Exist);
	UFUNCTION(BlueprintPure)
		UPARAM(DisplayName = "Task And State Array") TMap<UQuestTask*, bool>&
			GetAllTasksFromQuest(
				UQuestAsset* Quest);
	UFUNCTION(BlueprintSetter)
		void TrackQuest(UQuestAsset* ToTrack);
	UFUNCTION(BlueprintGetter)
		UQuestAsset* GetTrackedQuest() const {return TrackedQuest; }
	
public:
	UPROPERTY(BlueprintReadWrite)
		TMap<UQuestAsset*, FQuestCompletingInfo> CurrentQuestsAndInfo;
	UPROPERTY(BlueprintReadWrite)
		TArray<UQuestAsset*> CompletedQuests;
private:
	UPROPERTY(BlueprintGetter = GetTrackedQuest, BlueprintSetter = TrackQuest, Category = "Quests", meta=(AllowPrivateAccess))
		UQuestAsset* TrackedQuest; 

public: // DELEGATES
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnAddNewQuest")
		FAddNewQuestDelegate OnAddNewQuestDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnQuestComplete")
		FQuestCompleteDelegate OnQuestCompleteDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Delegates", DisplayName = "OnQuestStateChanged")
		FQuestStateChangedDelegate OnQuestStateChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnTrackedQuestSet")
		FTrackedQuestSetDelegate OnTrackedQuestSetDelegate;
	
private:
	void CheckOnAllTasksCompleted(UQuestAsset* QuestAsset);
	UFUNCTION() void OnTaskDoneCallback(UQuestTask* Task);
	bool SetQuestComplete(UQuestAsset* Quest);
	void UpdateQuestCompletingInfoToDoTasks(UQuestAsset* Quest, FQuestCompletingInfo& ToInitInfo);
};
