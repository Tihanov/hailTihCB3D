// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inventory/InventoryStructures.h"

#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInvItemArray
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FName RowName;
	UPROPERTY(BlueprintReadOnly) int32 Count;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUpDelegate, FName, ItemRowName, int32, CountOf);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemThrowOutDelegate, AInventoryItemBaseActor*, SpawnedItem, int32, InventoryIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponIndexSetToSlotDelegate,
	int32, SlotIndex,
	int32, SetWeaponIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponIndexRemovedFromSlotDelegate,
	int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipWeaponFromSlotDelegate,
	int32, EquippedWeaponIndex,
	int32, SlotIndex,
	FInvItemArray, Options);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnequipWeaponDelegate);



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

	// Contains indexes(for InventoryArray)
	TStaticArray<TOptional<int32>, 5> WeaponSlots;
	TOptional<int32> EquippedWeaponSlot;
	
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

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetWeaponToSlot(int32 SlotIndex, int32 WeaponIndex);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void RemoveWeaponFromSlot(int32 SlotIndex);
	UFUNCTION(BlueprintPure, Category = "Weapon")
		UPARAM(DisplayName = "WeaponIndex")int32
			GetWeaponIndexFromSlot(int32 SlotIndex,
			UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& DoesWeaponSetInSlot) const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
		TArray<int32> GetWeaponSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void EquipWeaponFromSlot(int32 SlotIndex);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnequipWeapon();
	UFUNCTION(BlueprintPure, Category = "Weapon")
		UPARAM(DisplayName = "IndexOfSlot") int32
			GetEquippedWeaponSlot(
			UPARAM(DisplayName = "IsSomeWeaponSlotEquipped?") bool& IsSomeWeaponSlotEquipped) const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
		UPARAM(DisplayName = "WeaponIndex")int32
			GetEquippedWeaponIndex(
			UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& IsWeaponEquipped) const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FInvItemArray GetWeaponInfoFromSlot(int32 SlotIndex,
		UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& DoesWeaponSetInSlot) const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
		FInvItemArray GetEquippedWeaponInfo(
			UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& IsWeaponEquipped) const;
	

public: /* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemPickedUp")
		FOnItemPickedUpDelegate OnItemPickedUpDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemThrowOut")
		FOnItemThrowOutDelegate OnItemThrowOutDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnWeaponIndexSetToSlot")
		FOnWeaponIndexSetToSlotDelegate OnWeaponIndexSetToSlotDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnWeaponIndexRemovedFromSlot")
		FOnWeaponIndexRemovedFromSlotDelegate OnWeaponIndexRemovedFromSlotDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnEquipWeaponFromSlot")
		FOnEquipWeaponFromSlotDelegate OnEquipWeaponFromSlotDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnUnequipWeapon")
		FOnUnequipWeaponDelegate OnUnequipWeaponDelegate;
};
