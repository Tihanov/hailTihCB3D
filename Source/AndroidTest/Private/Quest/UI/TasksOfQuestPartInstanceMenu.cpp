// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/TasksOfQuestPartInstanceMenu.h"

#include "Log.h"
#include "Components/VerticalBox.h"
#include "Quest/QuestAsset.h"
#include "Quest/QuestTask.h"
#include "Quest/UI/ItemOfInstanceMenu.h"
#include "Utils/Utils.h"

UTasksOfQuestPartInstanceMenu::UTasksOfQuestPartInstanceMenu()
{
	BaseInstanceClass = UQuestTask::StaticClass();
}

void UTasksOfQuestPartInstanceMenu::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	QuestAsset = InQuestAsset;
	Index = InIndex;

	const auto& Tasks = QuestAsset->Parts[Index].Tasks;
	TArray<UObject*> Instances;
	Instances.Reserve(Tasks.Num());
	for (const auto& Task : Tasks)
	{
		Instances.Add( DuplicateObject<UObject>(Task, this) );
	}

	Super::Init(Instances);
}

void UTasksOfQuestPartInstanceMenu::Save()
{
	const auto Items = ItemsBox->GetAllChildren();
	auto& Tasks = QuestAsset->Parts[Index].Tasks;

	Tasks.Empty();
	for (const auto& Item : Items)
	{
		const auto ItmOfInstMenu = Cast<UItemOfInstanceMenu>(Item);
		CHECK_ON_TRUE_DO_TASK(!ItmOfInstMenu, continue;);

		const auto QuestTask = Cast<UQuestTask>(ItmOfInstMenu->GetCurrentInstance());
		CHECK_ON_TRUE_DO_TASK(!QuestTask, continue;);

		Tasks.Add(QuestTask);
	} 
}
