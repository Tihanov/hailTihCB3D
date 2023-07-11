// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddActionDelegate,
	class UActionManagerComponent*, ActionManager,
	AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveActionDelegate,
	class UActionManagerComponent*, ActionManager,
	AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRefreshActionDelegate,
	class UActionManagerComponent*, ActionManager,
	AActor*, Actor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UActionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionManagerComponent();
protected:
	virtual void BeginPlay() override;

public: /*delegates*/
	UPROPERTY(BlueprintAssignable, Category=Delegates, DisplayName = "OnAddAction")
		FOnAddActionDelegate OnAddActionDelegate;
	UPROPERTY(BlueprintAssignable, Category=Delegates, DisplayName = "OnRemoveAction")
		FOnRemoveActionDelegate OnRemoveActionDelegate;
	UPROPERTY(BlueprintAssignable, Category=Delegates, DisplayName = "OnRefreshAction")
		FOnRefreshActionDelegate OnRefreshActionDelegate;
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Update)
		float ScanRate = 0.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Update)
		float EndMul = 40.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Update)
		float ScanCapsuleRadius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Update)
		float ScanCapsuleHeight;

protected:
	UPROPERTY(BlueprintReadOnly)
		TSet<AActor*> ScannedActors;
	
	void Scan();

private:
	FTimerHandle ScanTimerHandle;
	AActor* CastedOwner = nullptr;
};
