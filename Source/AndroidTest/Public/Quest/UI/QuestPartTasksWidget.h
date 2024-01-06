// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "QuestPartTasksWidget.generated.h"

class UButton;
class UComboBoxString;
class UQuestTask;
class UDetailsView;
class UVerticalBox;
class UTextBlock;
class UGridPanel;
class UQuestAsset;

UCLASS(Blueprintable, BlueprintType, Deprecated)
class ANDROIDTEST_API UDEPRECATED_TasksQuestPartWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDelegate, UWidget*, Widget);
	
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
		UGridPanel* GetGridPanelAOfTask(UWidget* TaskWidget) const;
	UFUNCTION(BlueprintPure)
		UButton* GetRemoveButtonOfTask(UWidget* TaskWidget) const;
	UFUNCTION(BlueprintPure)
		UComboBoxString* GetTaskComboBoxOfTask(UWidget* TaskWidget) const;
	UFUNCTION(BlueprintPure)
		UDetailsView* GetDetailsViewOfTask(UWidget* TaskWidget) const;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnChangeDelegate OnChangeDelegate;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UGridPanel* RootPanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* TasksBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* AddTaskButton;

	UPROPERTY()
		TArray<UQuestTask*> PartTasks;

protected:
	void InitPartTasksArray();
	void InitTasksBox();
	void InitTaskComboBoxWithAllTasks(UComboBoxString* TaskComboBox);
	
	void AddTaskToArray(const FString& TaskName, int32 InIndex);

	UFUNCTION()
		void OnTaskComboBoxSelectionChangedCallback(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
		void OnRemoveButtonPressedCallback();
	UFUNCTION()
		void OnAddTaskButtonClickedCallback();
	
private:
	UPROPERTY()
		UQuestAsset* QuestAsset;
	int32 Index;

	bool bIsTaskBoxInitialization = false;

	const int32 INDEX_OF_GRID_PANEL_A = 0;// Cast<GridPanel>(TasksBox[INDEX_OF_GRID_PANEL_A])
	const int32 INDEX_OF_TASK_REMOVE_BUTTON = 0;// Cast<GridPanel>(TasksBox[INDEX_OF_GRID_PANEL_A])[INDEX_OF_TASK_REMOVE_BUTTON]
	const int32 INDEX_OF_TASK_COMBO_BOX = 1;// Cast<GridPanel>(TasksBox[INDEX_OF_GRID_PANEL_A])[INDEX_OF_TASK_COMBO_BOX]
	const int32 INDEX_OF_DETAILS_VIEW = 1; // TasksBox[INDEX_OF_DETAILS_VIEW]

	static TMap<FString, UClass*> AllTaskAssets;
	inline static const FString TASK_NONE_NAME = TEXT("Ничего");
	static void InitAllTaskAssetsMap();
	static void InitAllTaskAssetsMapIfEmpty();
};
