// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Action/ActionInterface.h"
#include "Components/BoxComponent.h"
#include "ChestComponent.generated.h"

class UInventoryItemDefaultInfo;
class AChestCollision;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UChestComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UChestComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// TODO create spawn solution
	UFUNCTION(BlueprintCallable, Category = "Chest")
		void CreateChestCollision(bool bInCreateChestCollision);
	bool IsChestCollisionCreated() const { return ChestCollision.IsValid(); }

	FVector GetChestCollisionExtend() const { return ChestCollisionExtend; }
	
	FText GetChestName() const { return ChestName; }

	TSoftObjectPtr<AChestCollision> GetChestCollision() const { return ChestCollision; }

	UTexture2D* GetChestInterfaceIco() const { return ChestInterfaceIco; }

protected: /*Vars: */
	UPROPERTY(EditAnywhere, Category = "Chest", Instanced)
		TArray<UInventoryItemDefaultInfo*> InitInventory;
	UPROPERTY(EditAnywhere, Category = "Chest")
		bool bCreateChestCollision = false;
	UPROPERTY(EditAnywhere, Category = "Chest")
		FVector ChestCollisionExtend = FVector(20, 20, 20);
	UPROPERTY(EditAnywhere, Category = "Chest")
		FName SocketName = NAME_None;
	// If empty, will set automatically
	UPROPERTY(EditAnywhere, Category = "Chest")
		FText ChestName = FText::GetEmpty();
	UPROPERTY(EditAnywhere, Category = "Chest", AdvancedDisplay)
		UTexture2D* ChestInterfaceIco = nullptr;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Chest")
		TSoftObjectPtr<AChestCollision> ChestCollision;

private:
	void InitChestName();
	void InitChestInterfaceIco();
};


UCLASS()
class AChestCollision
	: public AActor
	, public IActionInterface
{
	GENERATED_BODY()

public:
	AChestCollision();

	void InitFromComponent(const UChestComponent* InInitFrom);

	virtual bool CanDoAction_Implementation() const override;
	virtual void DoAction_Implementation(AActor* CausedBy) override;
	virtual FText GetDisplayDescription_Implementation() const override;
	virtual UTexture2D* GetIco_Implementation() const override;

	UFUNCTION(BlueprintPure)
		const UChestComponent* InitedFrom() const { return InitFrom; }
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		UBoxComponent* BoxComponent;

protected:
	const UChestComponent* InitFrom;
};
