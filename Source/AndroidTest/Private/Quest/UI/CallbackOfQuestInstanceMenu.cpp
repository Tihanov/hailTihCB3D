// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/CallbackOfQuestInstanceMenu.h"

#include "Log.h"
#include "Components/VerticalBox.h"
#include "Quest/QuestAsset.h"
#include "Quest/QuestStructures.h"
#include "Quest/UI/ItemOfInstanceMenu.h"
#include "Utils/Utils.h"

UCallbackOfQuestInstanceMenu::UCallbackOfQuestInstanceMenu()
{
	BaseInstanceClass = UQuestCallback::StaticClass();
}

void UCallbackOfQuestInstanceMenu::Init(UQuestAsset* InQuestAsset, TArray<UQuestCallback*>& InQuestCallbacks)
{
	QuestCallbacks = &InQuestCallbacks;
	QuestAsset = InQuestAsset;
	
	TArray<UObject*> Instances;
	Instances.Reserve(QuestCallbacks->Num());
	for (const auto& Callback : *QuestCallbacks)
	{
		Instances.Add( DuplicateObject<UObject>(Callback, this) );
	}

	Super::Init(Instances);
}

void UCallbackOfQuestInstanceMenu::Save()
{
	const auto Items = ItemsBox->GetAllChildren();

	QuestCallbacks->Empty();
	for (const auto& Item : Items)
	{
		const auto ItmOfInstMenu = Cast<UItemOfInstanceMenu>(Item);
		CHECK_ON_TRUE_DO_TASK(!ItmOfInstMenu, continue;);

		const auto QuestTaskBuff = Cast<UQuestCallback>(ItmOfInstMenu->GetCurrentInstance());
		CHECK_ON_TRUE_DO_TASK(!QuestTaskBuff, continue;);

		const auto QuestTask = DuplicateObject<UQuestCallback>(QuestTaskBuff, QuestAsset);
		CHECK_ON_TRUE_DO_TASK(!QuestTask, continue;);
		
		QuestCallbacks->Add(QuestTask);
	} 
}
