// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/InstanceMenu.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Quest/QuestAsset.h"
#include "Quest/UI/ItemOfInstanceMenu.h"

void UInstanceMenu::Init(const TArray<UObject*>& Instances)
{
	ItemsBox->ClearChildren();
	for (const auto& Instance : Instances)
	{
		const auto Item = CreateAndInitItem();
		Item->SetCurrentInstance(Instance);
		ItemsBox->AddChild(Item);
	}

	AddNewItemButton->OnClicked.AddDynamic(this, &UInstanceMenu::AddNewItemButtonClickedCallback);
}

void UInstanceMenu::ItemChangedCallback(UWidget* Widget)
{
	OnChangeDelegate.Broadcast(this);
}

void UInstanceMenu::ItemRemoveButtonClickedCallback(UWidget* Widget)
{
	ItemsBox->RemoveChild(Widget);
	OnChangeDelegate.Broadcast(this);
}

void UInstanceMenu::AddNewItemButtonClickedCallback()
{
	const auto Item = CreateAndInitItem();
	ItemsBox->AddChild(Item);
}

UItemOfInstanceMenu* UInstanceMenu::CreateAndInitItem()
{
	const auto Item = CreateWidget<UItemOfInstanceMenu>(this, ItemOfInstanceMenuClass);
	Item->Init(BaseInstanceClass.Get());

	Item->OnChangedDelegate.AddDynamic(this, &UInstanceMenu::ItemChangedCallback);
	Item->OnRemoveButtonClickedDelegate.AddDynamic(this, &UInstanceMenu::ItemRemoveButtonClickedCallback);

	return Item;
}
