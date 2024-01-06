// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InstanceMenu.generated.h"

class UItemOfInstanceMenu;
class UQuestAsset;
class UVerticalBox;
class UButton;

UCLASS(Abstract)
class ANDROIDTEST_API UInstanceMenu : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeDelegate, UWidget*, Widget);
	
public:
	void Init(const TArray<UObject*>& Instances);

	UFUNCTION(BlueprintCallable)
		virtual void Save() PURE_VIRTUAL(UInstanceMenu::Save);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnChangeDelegate OnChangeDelegate;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UButton* AddNewItemButton;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
		UVerticalBox* ItemsBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UObject> BaseInstanceClass = UObject::StaticClass();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UItemOfInstanceMenu> ItemOfInstanceMenuClass;

private:
	UFUNCTION()
		void ItemChangedCallback(UWidget* Widget);
	UFUNCTION()
		void ItemRemoveButtonClickedCallback(UWidget* Widget);
	UFUNCTION()
		void AddNewItemButtonClickedCallback();

	UItemOfInstanceMenu* CreateAndInitItem();
};


