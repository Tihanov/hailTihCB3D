// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/CallbackInstanceMenus/TypesOfCallbacksInstanceMenu.h"

#include "Log.h"
#include "Components/VerticalBox.h"
#include "Quest/QuestAsset.h"
#include "Quest/UI/ItemOfInstanceMenu.h"
#include "Utils/Utils.h"

void UBeforeCallbackOfTaskInstanceMenu::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	auto& Callbacks = InQuestAsset->Parts[InIndex].BeforeCallbacks;
	Super::Init(InQuestAsset, Callbacks);
}

void UAfterCallbackOfTaskInstanceMenu::Init(UQuestAsset* InQuestAsset, int32 InIndex)
{
	auto& Callbacks = InQuestAsset->Parts[InIndex].AfterCallbacks;
	Super::Init(InQuestAsset, Callbacks);
}
