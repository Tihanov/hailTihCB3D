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
#include "Engine/AssetManager.h"

void UTasksQuestPartWidget::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	QuestAsset = InQuestAsset;
	Index = InIndex;
	
	InitPartTasksArray();

	InitTasksBox();

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

UComboBoxString* UTasksQuestPartWidget::GetTaskComboBoxOfTask(UWidget* TaskWidget) const
{
	const auto Task = Cast<UVerticalBox>(TaskWidget);
	CHECK_ON_TRUE_RETURN(Task == nullptr, nullptr);
	
	const auto TempWidget = Cast<UGridPanel>(Task->GetChildAt(0));
	CHECK_ON_TRUE_RETURN(TempWidget == nullptr, nullptr);
	
	return Cast<UComboBoxString>(TempWidget->GetChildAt(INDEX_OF_TASK_COMBO_BOX));
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
	int32 i = 0;
	for (auto Child : TasksBox->GetAllChildren())
	{
		CHECK_ON_TRUE_DO_TASK(!PartTasks.IsValidIndex(i) || PartTasks[i] == nullptr, continue;);

		const auto TaskComboBox = GetTaskComboBoxOfTask(Child);
		CHECK_ON_TRUE_DO_TASK(TaskComboBox == nullptr, continue;);

		InitTaskComboBoxWithAllTasks(TaskComboBox);
		TaskComboBox->SetSelectedOption(PartTasks[i]->Title.ToString());
		
		const auto DetailsView = GetDetailsViewOfTask(Child);
		CHECK_ON_TRUE_DO_TASK(DetailsView == nullptr, continue;);
		DetailsView->SetObject(PartTasks[i]);

		i += 1;
	}
}

void UTasksQuestPartWidget::InitTaskComboBoxWithAllTasks(UComboBoxString* const TaskComboBox)
{
	// UNRIAL IDI NAHUY

	TArray<FAssetData> AssetDataArray;
	CHECK_ON_TRUE_JUST_RETURN(!UAssetManager::Get().GetAssetRegistry().GetAssetsByPath(TEXT("/Game"), AssetDataArray, true, false));
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
		
		TaskComboBox->AddOption(QuestTaskDefObj->Title.ToString());
	}
}
