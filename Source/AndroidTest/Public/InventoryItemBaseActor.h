// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"

#include "InventoryItemBaseActor.generated.h"

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
	UFUNCTION(BlueprintCallable)
		FName PickUp(class APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable)
		void Init(FName Name, UDataTable* DataTable);
};
