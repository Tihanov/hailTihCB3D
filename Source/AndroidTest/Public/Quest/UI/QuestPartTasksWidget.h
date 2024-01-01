// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "QuestPartTasksWidget.generated.h"

class UComboBoxString;
class UQuestTask;
class UDetailsView;
class UVerticalBox;
class UTextBlock;
class UGridPanel;
class UQuestAsset;

UCLASS(Blueprintable, BlueprintType)
class ANDROIDTEST_API UTasksQuestPartWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void Init(UQuestAsset* InQuestAsset, int32 InIndex);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Init")
		void BP_Init(UQuestAsset* InQuestAsset, int32 InIndex);

	UFUNCTION(BlueprintCallable)
		void Save();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Save")
		void BP_Save();

	
	UFUNCTION(BlueprintPure)
		UComboBoxString* GetTaskComboBoxOfTask(UWidget* TaskWidget) const;
	UFUNCTION(BlueprintPure)
		UDetailsView* GetDetailsViewOfTask(UWidget* TaskWidget) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UGridPanel* RootPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* TasksBox;

	UPROPERTY()
		TArray<UQuestTask*> PartTasks;

protected:
	void InitPartTasksArray();
	void InitTasksBox();
	void InitTaskComboBoxWithAllTasks(UComboBoxString* TaskComboBox);

private:
	UPROPERTY()
		UQuestAsset* QuestAsset;
	int32 Index;
	
	const int32 INDEX_OF_TASK_COMBO_BOX = 1;// Cast<GridPanel>(TasksBox[0])[INDEX_OF_TASK_COMBO_BOX]
	const int32 INDEX_OF_DETAILS_VIEW = 1; // TasksBox[INDEX_OF_DETAILS_VIEW]
};
