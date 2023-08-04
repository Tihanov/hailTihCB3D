// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySlotsComponent.generated.h"

class UInventoryItemDefaultInfo;

UENUM(BlueprintType)
enum class ESlotType: uint8
{
	Weapon		UMETA(DisplayName = "Weapon")
};

USTRUCT(BlueprintType)
struct FItemsAndEquippedItemIndex
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 EquippedItemIndex = -1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<UInventoryItemDefaultInfo*> Array;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotsUpdatesDelegate,
	ESlotType, SlotType,
	FItemsAndEquippedItemIndex, Items);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipsFromSlotDelegate,
	ESlotType, SlotType,
	FItemsAndEquippedItemIndex, Items);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnequipsFromSlotDelegate,
	ESlotType, SlotType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UInventorySlotsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventorySlotsComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Slots")
		void SetCountOfItemsOnSlot(ESlotType SlotType, int32 Count);
	UFUNCTION(BlueprintCallable, Category = "Slots")
		void SetItemIntoSlot(ESlotType SlotType, int32 Index, UInventoryItemDefaultInfo* Item);
	UFUNCTION(BlueprintCallable, Category = "Slots")
		void UnsetItemFromSlot(ESlotType SlotType, int32 Index);
	UFUNCTION(BlueprintPure, Category = "Slots")
		FItemsAndEquippedItemIndex GetItemsFromSlotsOfType(ESlotType SlotType) const;

	UFUNCTION(BlueprintCallable, Category = "Slots|Equip")
		void EquipItemFromSlot(ESlotType SlotType, int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Slots|Equip")
		void UnequipItemFromSlot(ESlotType SlotType);
	UFUNCTION(BlueprintPure, Category = "Slots|Equip")
		UInventoryItemDefaultInfo* GetEquippedItemFromSlotsOfType(ESlotType SlotType) const;
	UFUNCTION(BlueprintPure, Category = "Slots|Equip")
		int32 GetEquippedSlotIndexOfSlotType(ESlotType SlotType) const;

public: // DELEGATES:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate", DisplayName = "OnSlotsUpdates")
		FOnSlotsUpdatesDelegate OnSlotsUpdatesDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate", DisplayName = "OnItemEquipsFromSlot")
		FOnItemEquipsFromSlotDelegate OnItemEquipsFromSlotDelegate;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegate", DisplayName = "OnItemUnequipsFromSlot")
		FOnItemUnequipsFromSlotDelegate OnItemUnequipsFromSlotDelegate;

protected:
	TMap<ESlotType, FItemsAndEquippedItemIndex> Slots;

private:
	UFUNCTION()
		void OnItemFromInventoryTrashed(UInventoryItemDefaultInfo* ItemInfo);
};
