// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/InstanceMenu.h"

#include "Components/VerticalBox.h"
#include "Quest/QuestAsset.h"
#include "Quest/UI/ItemOfInstanceMenu.h"

void UInstanceMenu::Init(const TArray<FString>& Titles)
{
	for (const auto& Title : Titles)
	{
		const auto Item = CreateWidget<UItemOfInstanceMenu>(this, ItemOfInstanceMenuClass);
		ItemsBox->AddChild(Item);
		Item->Init(BaseInstanceClass.Get());
		Item->SetCurrentInstanceByTitle(Title);

		// TODO On Change Delegates 
	} 
}

void UInstanceMenu::Init(const TArray<UObject*>& Instances)
{
	for (const auto& Instance : Instances)
	{
		const auto Item = CreateWidget<UItemOfInstanceMenu>(this, ItemOfInstanceMenuClass);
		ItemsBox->AddChild(Item);
		Item->Init(BaseInstanceClass.Get());
		Item->SetCurrentInstance(Instance);

		// TODO On Change Delegates 
	} 
}

void UInstanceMenu::Save()
{
}
