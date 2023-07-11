// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainGameState.generated.h"

UCLASS()
class ANDROIDTEST_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMainGameState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|Debug|Weapon")
		TEnumAsByte<EDrawDebugTrace::Type> DebugWeaponTracersType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|Debug|Weapon")
		TEnumAsByte<EDrawDebugTrace::Type> DebugCameraTracersType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options|GameSettings|Game")
		bool EnableFpsCounter = true;
};
