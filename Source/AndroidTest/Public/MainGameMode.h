// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

class ATimeManager;

UCLASS()
class ANDROIDTEST_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	virtual void PreInitializeComponents() override;

	UFUNCTION(BlueprintPure, Category=Classes)
		ATimeManager* GetTimeManager() const;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Classes)
		TSubclassOf<ATimeManager> TimeManagerClass;

private:
	ATimeManager* TimeManager;
};
