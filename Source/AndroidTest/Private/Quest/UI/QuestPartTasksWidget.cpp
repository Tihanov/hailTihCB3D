// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/QuestPartTasksWidget.h"

#include "Log.h"
#include "AssetRegistry/AssetRegistryHelpers.h"
#include "Components/ComboBox.h"
#include "Components/ComboBoxString.h"
#include "Components/DetailsView.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "Math/Plane.h"
#include "Quest/QuestAsset.h"
#include "Quest/QuestTask.h"
#include "Utils/Utils.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Components/Button.h"
#include "Engine/AssetManager.h"
#include "UObject/UnrealTypePrivate.h"

void UTasksQuestPartWidget::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	QuestAsset = InQuestAsset;
	Index = InIndex;
	
	InitPartTasksArray();

	InitTasksBox();

	AddTaskButton->OnClicked.AddDynamic(this, &UTasksQuestPartWidget::OnAddTaskButtonClickedCallback);

	BP_Init(InQuestAsset, InIndex);
}

void UTasksQuestPartWidget::Save()
{
	auto& Tasks = QuestAsset->Parts[Index].Tasks;
	Tasks.SetNum(0);
	int32 i = 0;
	for (const auto PartTask : PartTasks)
	{
		Tasks.Add(DuplicateObject(PartTask, QuestAsset));
		
		i += 1;
	}

	BP_Save();
}

UGridPanel* UTasksQuestPartWidget::GetGridPanelAOfTask(UWidget* TaskWidget) const
{
	const auto Task = Cast<UVerticalBox>(TaskWidget);
	CHECK_ON_TRUE_RETURN(Task == nullptr, nullptr);
	
	return Cast<UGridPanel>(Task->GetChildAt(INDEX_OF_GRID_PANEL_A));
}

UButton* UTasksQuestPartWidget::GetRemoveButtonOfTask(UWidget* TaskWidget) const
{
	const auto GridPanelA = GetGridPanelAOfTask(TaskWidget);
	CHECK_ON_TRUE_RETURN(GridPanelA == nullptr, nullptr);
	
	return Cast<UButton>(GridPanelA->GetChildAt(INDEX_OF_TASK_REMOVE_BUTTON));
}

UComboBoxString* UTasksQuestPartWidget::GetTaskComboBoxOfTask(UWidget* TaskWidget) const
{
	const auto GridPanelA = GetGridPanelAOfTask(TaskWidget);
	CHECK_ON_TRUE_RETURN(GridPanelA == nullptr, nullptr);
	
	return Cast<UComboBoxString>(GridPanelA->GetChildAt(INDEX_OF_TASK_COMBO_BOX));
}

UDetailsView* UTasksQuestPartWidget::GetDetailsViewOfTask(UWidget* TaskWidget) const
{
	const auto Task = Cast<UVerticalBox>(TaskWidget);
	CHECK_ON_TRUE_RETURN(Task == nullptr, nullptr);
	
	return Cast<UDetailsView>(Task->GetChildAt(INDEX_OF_DETAILS_VIEW));
}

void UTasksQuestPartWidget::InitPartTasksArray()
{
	const auto& Tasks = QuestAsset->Parts[Index].Tasks;
	PartTasks.SetNum(0);
	for (const auto Task : Tasks)
	{
		PartTasks.Add(DuplicateObject(Task, this));
	}
}

void UTasksQuestPartWidget::InitTasksBox()
{
	bIsTaskBoxInitialization = true;
	int32 i = -1;
	for (auto Child : TasksBox->GetAllChildren())
	{
		i += 1;
		const auto TaskComboBox = GetTaskComboBoxOfTask(Child);
		CHECK_ON_TRUE_DO_TASK(TaskComboBox == nullptr, continue;);
		InitTaskComboBoxWithAllTasks(TaskComboBox);
		if(!TaskComboBox->OnSelectionChanged.IsAlreadyBound(this,
			&UTasksQuestPartWidget::OnTaskComboBoxSelectionChangedCallback))
		{
			TaskComboBox->OnSelectionChanged.AddDynamic(
				this, &UTasksQuestPartWidget::OnTaskComboBoxSelectionChangedCallback);
		}

		const auto DetailsView = GetDetailsViewOfTask(Child);
		CHECK_ON_TRUE_DO_TASK(DetailsView == nullptr, continue;);

		const auto RemoveButton = GetRemoveButtonOfTask(Child);
		CHECK_ON_TRUE_DO_TASK(RemoveButton == nullptr, continue;);
		RemoveButton->OnClicked.AddDynamic(this, &UTasksQuestPartWidget::OnRemoveButtonPressedCallback);
		
		if(PartTasks.IsValidIndex(i))
		{
			Child->SetVisibility(ESlateVisibility::Visible);
			DetailsView->SetObject(PartTasks[i]);
			if(PartTasks[i])
				TaskComboBox->SetSelectedOption(PartTasks[i]->Title.ToString());
		}
		else
		{
			Child->SetVisibility(ESlateVisibility::Collapsed);
			TaskComboBox->ClearSelection();
			DetailsView->SetObject(nullptr);
		}

	}
	bIsTaskBoxInitialization = false;
}

void UTasksQuestPartWidget::InitTaskComboBoxWithAllTasks(UComboBoxString* const TaskComboBox)
{
	InitAllTaskAssetsMapIfEmpty();

	TaskComboBox->ClearOptions();
	for (const auto& Element : AllTaskAssets)
	{
		TaskComboBox->AddOption(Element.Key);
	}
}

void UTasksQuestPartWidget::AddTaskToArray(const FString& TaskName, const int32 InIndex)
{
	if(!PartTasks.IsValidIndex(InIndex))
		PartTasks.SetNumZeroed(InIndex + 1);
	
	if(AllTaskAssets[TaskName])
		PartTasks[InIndex] = NewObject<UQuestTask>(this, AllTaskAssets[TaskName]);
	else
		PartTasks[InIndex] = nullptr;
}

void UTasksQuestPartWidget::OnTaskComboBoxSelectionChangedCallback(FString SelectedItem,
                                                                   ESelectInfo::Type SelectionType)
{
	if(bIsTaskBoxInitialization)
		return;
	
	int32 i = 0;
	for (auto Child : TasksBox->GetAllChildren())
	{
		const auto TaskComboBox = GetTaskComboBoxOfTask(Child);
		if(TaskComboBox != nullptr && TaskComboBox->IsOpen())
		{
			break;
		}
			
		i += 1;
	}
	CHECK_ON_TRUE_JUST_RETURN(i == TasksBox->GetChildrenCount());
	
	InitAllTaskAssetsMapIfEmpty();
	
	AddTaskToArray(SelectedItem, i);
	
	InitTasksBox();
	OnChangeDelegate.Broadcast(this);
}

void UTasksQuestPartWidget::OnRemoveButtonPressedCallback()
{
	int32 i = 0;
	for (auto Child : TasksBox->GetAllChildren())
	{
		const auto RemoveButton = GetRemoveButtonOfTask(Child);
		if(RemoveButton != nullptr && RemoveButton->IsPressed())
		{
			break;
		}
			
		i += 1;
	}

	if(PartTasks.IsValidIndex(i))
	{
		PartTasks.RemoveAt(i);
		InitTasksBox();
		OnChangeDelegate.Broadcast(this);
	}
}

void UTasksQuestPartWidget::OnAddTaskButtonClickedCallback()
{
	int32 i = 0;
	for (auto Child : TasksBox->GetAllChildren())
	{
		if(Child->GetVisibility() == ESlateVisibility::Collapsed)
		{
			Child->SetVisibility(ESlateVisibility::Visible);
			break;
		}
			
		i += 1;
	}

	AddTaskToArray(TASK_NONE_NAME, i);
	InitTasksBox();
	OnChangeDelegate.Broadcast(this);
}


TMap<FString, UClass*> UTasksQuestPartWidget::AllTaskAssets = {};

void UTasksQuestPartWidget::InitAllTaskAssetsMap()
{
	TArray<FAssetData> AssetDataArray;
	CHECK_ON_TRUE_JUST_RETURN(!UAssetManager::Get()./*UNRIAL IDI NAHUY*/GetAssetRegistry().GetAssetsByPath(TEXT("/Game"), AssetDataArray, true, false));
	AllTaskAssets.Empty();
	AllTaskAssets.Add(TASK_NONE_NAME, nullptr);

	for (const auto& AssetData : AssetDataArray)
	{
		const auto Asset = Cast<UBlueprint>(AssetData.GetAsset());
		if(!Asset)
			continue;
		
		const auto AssetClass = Asset->GeneratedClass.Get();
		if(!AssetClass->IsChildOf(UQuestTask::StaticClass()))
			continue;
		
		const auto QuestTaskDefObj = AssetClass->GetDefaultObject<UQuestTask>();
		CHECK_ON_TRUE_DO_TASK(!QuestTaskDefObj, continue;);

		AllTaskAssets.Add(QuestTaskDefObj->Title.ToString(), AssetClass);
	}
}

void UTasksQuestPartWidget::InitAllTaskAssetsMapIfEmpty()
{
	if(AllTaskAssets.Num() == 0)
		InitAllTaskAssetsMap();
}
