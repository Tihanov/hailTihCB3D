// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/UI/ItemOfInstanceMenu.h"

#include "Log.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/DetailsView.h"
#include "Engine/AssetManager.h"
#include "Quest/TitleInterface.h"
#include "Utils/Utils.h"

void UItemOfInstanceMenu::Init(UClass* InBaseInstanceClass)
{
	BaseInstanceClass = InBaseInstanceClass;
	InitClassesDataForBaseClass(BaseInstanceClass);
	
	const auto& TtlAndInstClasses = ClassesData[BaseInstanceClass];
	for (const auto&[Title, _] : TtlAndInstClasses)
	{
		SubclassesComboBox->AddOption(Title);
	}
	
	SubclassesComboBox->OnSelectionChanged.AddDynamic(this, &UItemOfInstanceMenu::SubclassesComboBoxSelectionChangedCallback);
	RemoveButton->OnClicked.AddDynamic(this, &UItemOfInstanceMenu::RemoveButtonClickedCallback);

	BP_Init(InBaseInstanceClass);
}

void UItemOfInstanceMenu::SetCurrentInstanceByTitle(FString Title)
{
	CHECK_ON_TRUE_JUST_RETURN(!ClassesDataForBaseClassInited(BaseInstanceClass));
	
	SubclassesComboBox->SetSelectedOption(Title);

	const auto InstanceClassPtr = ClassesData[BaseInstanceClass].Find(Title);
	CHECK_ON_TRUE_JUST_RETURN(!InstanceClassPtr);

	const auto InstanceClass = *InstanceClassPtr;
	const auto Instance = InstanceClass == nullptr
		? nullptr
		: NewObject<UObject>(this, InstanceClass);
	
	CurrentInstance = Instance;
	InstanceDetailsView->SetObject(Instance);

	OnChangedDelegate.Broadcast(this);
}

void UItemOfInstanceMenu::SetCurrentInstance(UObject* InCurrentInstance)
{
	CHECK_ON_TRUE_JUST_RETURN(!ClassesDataForBaseClassInited(BaseInstanceClass));
	
	const auto TitleInterface = Cast<ITitleInterface>(InCurrentInstance);
	CHECK_ON_TRUE_JUST_RETURN(!TitleInterface);
	const auto Title = TitleInterface->GetTitle().ToString();
	
	SubclassesComboBox->SetSelectedOption(Title);

	CurrentInstance = InCurrentInstance;
	InstanceDetailsView->SetObject(InCurrentInstance);
	
	OnChangedDelegate.Broadcast(this);
}

void UItemOfInstanceMenu::SubclassesComboBoxSelectionChangedCallback(FString SelectedItem,
	ESelectInfo::Type SelectionType)
{
	SetCurrentInstanceByTitle(SelectedItem);
}

void UItemOfInstanceMenu::RemoveButtonClickedCallback()
{
	OnRemoveButtonClickedDelegate.Broadcast(this);
}

void UItemOfInstanceMenu::InitClassesDataForBaseClass(UClass* BaseClass)
{
	if(ClassesData.Contains(BaseClass))
		return;

	auto& TtlAndInstClasses = ClassesData.Add(BaseClass);
	TtlAndInstClasses.Add(TASK_NONE_NAME, nullptr);

	TArray<FAssetData> AssetDataArray;
	CHECK_ON_TRUE_JUST_RETURN(!UAssetManager::Get().GetAssetRegistry().GetAssetsByPath(TEXT("/Game"), AssetDataArray, true, false));
	for (const auto& AssetData : AssetDataArray)
	{
		const auto Asset = Cast<UBlueprint>(AssetData.GetAsset());
		if(!Asset)
			continue;
		
		const auto AssetClass = Asset->GeneratedClass.Get();
		if(!AssetClass || !AssetClass->IsChildOf(BaseClass))
			continue;
		
		const auto QuestTaskDefObj = Cast<ITitleInterface>(AssetClass->GetDefaultObject());
		CHECK_ON_TRUE_DO_TASK(!QuestTaskDefObj, continue;);

		TtlAndInstClasses.Add(QuestTaskDefObj->GetTitle().ToString(), AssetClass);
	}
}

bool UItemOfInstanceMenu::ClassesDataForBaseClassInited(UClass* BaseClass)
{
	return ClassesData.Contains(BaseClass);
}
