// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inventory/InventoryStructures.h"
#include "Inventory/InventoryItemDefaultInfo.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, UInventoryItemDefaultInfo*, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangedDelegate, UInventoryItemDefaultInfo*, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemTrashedDelegate, UInventoryItemDefaultInfo*, ItemInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDROIDTEST_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Components")
		class UInventorySlotsComponent* InventorySlots;
	
public:
	// Direct add item to inventory
	UFUNCTION(BlueprintCallable)
		UPARAM(DisplayName="CountOfNotAddedItems")int32
		AddItem(FName RowName, int32 CountOfItems);
	UFUNCTION(BlueprintCallable)
		UPARAM(DisplayName="IsBeenAdded?")bool
		AddUniqueItem(UInventoryItemDefaultInfo* ItemInfo);
	// Take item from the world, add to inventory, delete if CountOf == 0, change CountOf	
	UFUNCTION(BlueprintCallable)
		void PickUpItem(AInventoryItemBaseActor* ItemActor);

	// Direct delete item from the inventory
	UFUNCTION(BlueprintCallable)
		void TrashItem(UInventoryItemDefaultInfo* ItemStack, int32 CountToDel);
	// Throw item out to the level and delete it from the inventory
	UFUNCTION(BlueprintCallable)
		void ThrowOutItem(UInventoryItemDefaultInfo* ItemStack, int32 CountToDel);

	UFUNCTION(BlueprintPure)
		int32 GetCountOfSpecificItemsByInfo(UInventoryItemDefaultInfo* ItemInfo) const;
	
	UFUNCTION(BlueprintPure)
		int32 GetSize() const;
	UFUNCTION(BlueprintPure)
		int32 GetCountOfItems() const;


public: /* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemAdded")
		FOnItemAddedDelegate OnItemAddedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemChanged")
		FOnItemChangedDelegate OnItemChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemTrashed")
		FOnItemTrashedDelegate OnItemTrashedDelegate;

protected:
	UPROPERTY(BlueprintReadWrite)
		TArray<UInventoryItemDefaultInfo*> InventoryArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float MaxWeight = 100.f;
	UPROPERTY(BlueprintReadOnly)
		float Weight = 0.f;

private:
	UDataTable** InvDataTable = nullptr;
};
