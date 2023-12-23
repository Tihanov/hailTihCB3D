// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/InventoryItemBaseActor.h"
#include "QuestStructures.generated.h"

class UQuestManagerComponent;
class UQuestTask;
class UQuestCallback;


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////    REWARD INFO:
////////////////////////////////////////////////////////



USTRUCT(BlueprintType)
struct FQuestRewardsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ExpReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced) TArray<class UInventoryItemDefaultInfo*> ItemsReward;  	
};



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////	QUEST PART INFO:
////////////////////////////////////////////////////////



USTRUCT(BlueprintType)
struct FQuestPartInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine = true)) FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, meta = (TitleProperty = "Title")) TArray<UQuestTask*> Tasks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FQuestRewardsInfo Rewards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced) TArray<UQuestCallback*> BeforeCallbacks;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced) TArray<UQuestCallback*> AfterCallbacks;
};


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////	METHODS TO DO AFTER:
////////////////////////////////////////////////////////


UCLASS(BlueprintType, Blueprintable, Abstract, DefaultToInstanced, EditInlineNew, meta = (ShowWorldContextPin = "true"))
class ANDROIDTEST_API UQuestCallback: public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Do(const UQuestManagerComponent* QuestManagerComponent);
};



/**
 * DONT USE, JUST TO DEFINE STRUCTURES
 */
UCLASS(DisplayName = "DONTUSE_QuestStructures") class ANDROIDTEST_API UQuestStructures : public UObject{GENERATED_BODY()};
