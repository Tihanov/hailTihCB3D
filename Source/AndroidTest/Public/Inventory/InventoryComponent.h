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

USTRUCT(BlueprintType)
struct FInvArrayOfItemsProxyStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		TArray<FInvItemArray> Array;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUpDelegate, FName, ItemRowName, int32, CountOf);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemThrowOutDelegate, AInventoryItemBaseActor*, SpawnedItem, int32, InventoryIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotsUpdatedDelegate,
	class UInventoryComponent*, InventoryComponent,
	FInvArrayOfItemsProxyStruct, WeaponSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipWeaponDelegate,
	class UInventoryComponent*, InventoryComponent,
	FInvItemArray, WeaponInfo,
	bool, IsNull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnequipWeaponDelegate,
	class UInventoryComponent*, InventoryComponent);


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

	TStaticArray<TOptional<FInvItemArray>, 5> WeaponSlots;
	TOptional<int32> EquippedWeaponSlotIndex;
	
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

	UFUNCTION(BlueprintPure)
		FInvItemArray GetItemOnIndex(
			int32 Index,
			UPARAM(DisplayName = "DoesExist?")bool& DoesExist) const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetWeaponToSlot(int32 SlotIndex, const FInvItemArray& Weapon);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void RemoveWeaponFromSlot(int32 SlotIndex);
	UFUNCTION(BlueprintPure, Category = "Weapon")
		UPARAM(DisplayName = "Weapon")FInvItemArray
			GetWeaponFromSlot(int32 SlotIndex,
			UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& DoesWeaponSetInSlot) const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
		TArray<FInvItemArray> GetWeaponSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void EquipWeaponFromSlot(int32 SlotIndex);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UnequipWeapon();
	UFUNCTION(BlueprintPure, Category = "Weapon")
		UPARAM(DisplayName = "IndexOfSlot") int32
			GetEquippedWeaponSlot(
			UPARAM(DisplayName = "IsSomeWeaponSlotEquipped?") bool& IsSomeWeaponSlotEquipped) const;
	UFUNCTION(BlueprintPure, Category = "Weapon")
		FInvItemArray GetEquippedWeaponInfo(
			UPARAM(DisplayName = "DoesWeaponSetInSlot?")bool& IsWeaponEquipped) const;
	

public: /* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemPickedUp")
		FOnItemPickedUpDelegate OnItemPickedUpDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnItemThrowOut")
		FOnItemThrowOutDelegate OnItemThrowOutDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnWeaponSlotsUpdated")
		FOnWeaponSlotsUpdatedDelegate OnWeaponSlotsUpdatedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnEquipWeapon")
		FOnEquipWeaponDelegate OnEquipWeaponDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnUnequipWeapon")
		FOnUnequipWeaponDelegate OnUnequipWeaponDelegate;
};
