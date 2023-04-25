// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/QuestAsset.h"
#include "QuestManagerComponent.generated.h"

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

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void AddQuest(UQuestAsset* Quest);
	UFUNCTION(BlueprintPure)
		UPARAM(DisplayName = "Part Info") FQuestPartInfo
			GetCurrentPartFromQuest(
				UQuestAsset* Quest,
				UPARAM(DisplayName = "DoesExist?") bool& Exist);
	
public:
	UPROPERTY(BlueprintReadWrite)
		TMap<UQuestAsset*, FQuestCompletingInfo> CurrentQuestsAndInfo;
	UPROPERTY(BlueprintReadWrite)
		TArray<UQuestAsset*> CompletedQuests;

private:
	FTimerHandle TaskCheckTimerHandle;
	void TaskCheck();
};
