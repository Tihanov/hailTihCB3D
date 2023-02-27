// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/DataTable.h"

#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EInvItemType : uint8
{
	DEFAULT				UMETA(Display = "Default")
};

USTRUCT(BlueprintType)
struct FInvItemDataTableOptional
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector				Scale = { 1,1,1 };
};

USTRUCT(BlueprintType)
struct FInvItemDataTable : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText				DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText				Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EInvItemType		Type			= EInvItemType::DEFAULT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) class UStaticMesh*	Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) class UTexture2D*	Ico;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float				WeightKg		= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float				Price			= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int					MaxStackCount	= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AActor> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FInvItemDataTableOptional Other;
};

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

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadWrite)
		TArray<FInvItemArray> InventoryArray;
	UPROPERTY(EditDefaultsOnly)
		UDataTable* InvDataTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float MaxWeight = 100.f;
	UPROPERTY(BlueprintReadOnly)
		float Weight = 0.f;
public:
	UFUNCTION(BlueprintCallable)
		bool AddItem(FName RowName);
	UFUNCTION(BlueprintPure)
		int32 GetSize() const;
	UFUNCTION(BlueprintPure)
		int32 GetCountOfItems() const;
};
