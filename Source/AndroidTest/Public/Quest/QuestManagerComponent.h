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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FQuestVariableChangedDelegate,
	UQuestManagerComponent*, QuestManagerComponent,
	UQuestAsset*, Quest,
	FName, VarName,
	int, Value);


USTRUCT(BlueprintType)struct FQuestCompletingInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) int QuestPart = 0;
	UPROPERTY(BlueprintReadWrite) TMap<UQuestTask*, bool> TasksAndState;
	UPROPERTY(BlueprintReadWrite) TMap<FName, int> Memory;
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

	UFUNCTION(BlueprintCallable)
		void SetQuestVariable(UQuestAsset* QuestAsset, FName VarName, int Value);

	// If Var does not exist, return INT_MIN 
	UFUNCTION(BlueprintPure)
		int GetQuestVariable(UQuestAsset* QuestAsset,
			FName VarName,
			UPARAM(DisplayName = "DoesExist?") bool& OutExist) const;
	// If Var does not exist, return INT_MIN 
	int GetQuestVariable(UQuestAsset* QuestAsset, FName VarName) const;

	UFUNCTION(BlueprintPure)
		bool DoesQuesVariableExists(UQuestAsset* QuestAsset, FName VarName) const;
	
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
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnQuestVariableChanged")
		FQuestVariableChangedDelegate OnQuestVariableChangedDelegate;
	
private:
	void CheckOnAllTasksCompleted(UQuestAsset* QuestAsset);
	UFUNCTION() void OnTaskDoneCallback(UQuestTask* Task);
	bool SetQuestComplete(UQuestAsset* Quest);
	void UpdateQuestCompletingInfoToDoTasks(UQuestAsset* Quest, FQuestCompletingInfo& ToInitInfo);

	void CallCallbacksOfPart_After(const FQuestPartInfo& Part);
};
