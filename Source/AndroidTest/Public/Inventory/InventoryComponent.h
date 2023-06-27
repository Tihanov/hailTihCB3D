// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inventory/InventoryStructures.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUpDelegate, FName, ItemRowName, int32, CountOf);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemThrowOutDelegate, AInventoryItemBaseActor*, SpawnedItem, int32, InventoryIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSetItemInHandDelegate, int32, Index, FName, ItemRowName, int32, CountOf);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnsetHandItemDelegate);

USTRUCT(BlueprintType)
struct FInvItemArray
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FName RowName;
	UPROPERTY(BlueprintReadOnly) int32 Count;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDROIDTEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite)
		TArray<FInvItemArray> InventoryArray;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		UDataTable* InvDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float MaxWeight = 100.f;
	UPROPERTY(BlueprintReadOnly)
		float Weight = 0.f;

	int IndexOfInHandItem = -1;
	
public:
	// Direct add item to inventory
	UFUNCTION(BlueprintCallable)
		UPARAM(DisplayName="CountOfNotAddedItems")int32
		AddItem(FName RowName, int32 CountOfItems);
	// Take item from the world, add to inventory, delete if CountOf == 0, change CountOf	
	UFUNCTION(BlueprintCallable)
		void PickUpItem(AInventoryItemBaseActor* ItemActor);

	// Direct delete item from the inventory
	UFUNCTION(BlueprintCallable)
		bool TrashItem(int32 Index, int32 Count, FName& RowName);
	// Throw item out to the level and delete it from the inventory
	UFUNCTION(BlueprintCallable)
		void ThrowOutItem(int32 Index, int32 Count);
	
	UFUNCTION(BlueprintPure)
		int32 GetSize() const;
	UFUNCTION(BlueprintPure)
		int32 GetCountOfItems() const;

	UFUNCTION(BlueprintCallable)
		void SetItemInHand(int32 Index);
	UFUNCTION(BlueprintCallable)
		void UnsetHandItem();
	UFUNCTION(BlueprintPure)
		int32 GetIndexOfInHandItem(bool& bIsInHandItemSet) const;

public: /* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemPickedUp")
		FOnItemPickedUpDelegate OnItemPickedUpDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemThrowOut")
		FOnItemThrowOutDelegate OnItemThrowOutDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnSetItemInHand")
		FOnSetItemInHandDelegate OnSetItemInHandDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnUnsetHandItem")
		FOnUnsetHandItemDelegate OnUnsetHandItemDelegate;
};
