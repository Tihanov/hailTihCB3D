// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemOfInstanceMenu.generated.h"

class UDetailsView;
class UComboBoxString;
class UButton;

UCLASS(Abstract)
class ANDROIDTEST_API UItemOfInstanceMenu : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveButtonClickedDelegate, UItemOfInstanceMenu*, Item);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedDelegate, UItemOfInstanceMenu*, Item);
	
public:
	UFUNCTION(BlueprintCallable)
		virtual void Init(UClass* InBaseInstanceClass);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Init")
		void BP_Init(UClass* InBaseInstanceClass);

	void SetCurrentInstanceByTitle(FString Title);

	// do NOT call OnChangeDelegate
	void SetCurrentInstance(UObject* InCurrentInstance); 
	UObject* GetCurrentInstance() const { return CurrentInstance; };
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
		FOnRemoveButtonClickedDelegate OnRemoveButtonClickedDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnChangedDelegate OnChangedDelegate;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UButton* RemoveButton;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UComboBoxString* SubclassesComboBox;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UDetailsView* InstanceDetailsView;
	
private:
	UClass* BaseInstanceClass;
	UPROPERTY() UObject* CurrentInstance = nullptr;

private:
	UFUNCTION()
		void SubclassesComboBoxSelectionChangedCallback(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
		void RemoveButtonClickedCallback();
	
protected:
	using FTitleAndInstanceClassMap = TMap<FString, UClass*>;
	inline static TMap<UClass* /*BaseClass*/, FTitleAndInstanceClassMap> ClassesData;
	static void InitClassesDataForBaseClass(UClass* BaseClass);
	static bool ClassesDataForBaseClassInited(UClass* BaseClass);

	inline static const FString TASK_NONE_NAME = TEXT("None");  
};
