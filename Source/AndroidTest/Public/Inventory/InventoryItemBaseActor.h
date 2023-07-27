// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"
#include "Action/ActionInterface.h"

#include "InventoryItemBaseActor.generated.h"

UCLASS()
class ANDROIDTEST_API AInventoryItemBaseActor
	: public AActor
	, public IActionInterface
{
	GENERATED_BODY()
	
public:	
	AInventoryItemBaseActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* RootMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "CanBePickUpped?",Category = "Options")
		bool bCanBePickUpped = true;

	UPROPERTY(BlueprintReadWrite)
		class UInventoryItemDefaultInfo* ItemInfo;

public:
	UFUNCTION(BlueprintCallable)
		void Init(UInventoryItemDefaultInfo* InitItemInfo);
	UFUNCTION(BlueprintCallable)
		void InitWithRowName(FName RowName, int32 CountOf);

	virtual EActionType GetActionType_Implementation() override;
	virtual void DoAction_Implementation(AActor* CausedBy) override;
	virtual FText GetDisplayDescription_Implementation() const override;
	virtual UTexture2D* GetIco_Implementation() const override;
	virtual bool CanDoAction_Implementation() const override;

private:
	UDataTable** InvDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options|Init", meta = (AllowPrivateAccess = true, ClampMin = 1, UIMin = 1, UIMax = 100))
		FName InitRowName = TEXT("None");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options|Init", meta = (AllowPrivateAccess = true))
		int32 InitCountOf = 1;
};
