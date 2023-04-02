// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"

#include "InventoryItemBaseActor.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemInitStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UDataTable* DataTable;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 CountOf;
};

UCLASS()
class ANDROIDTEST_API AInventoryItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryItemBaseActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RootMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName RowName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UDataTable* InvDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CountOf;

public:
	UFUNCTION(BlueprintCallable)
		FName PickUp(class APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
		void Init(FInventoryItemInitStruct InitStruct);
};