// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMainCharacterState : uint8
{
	Default,
	SafeZone
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChangeDelegate,
	EMainCharacterState, CharacterState);

UCLASS(Blueprintable, BlueprintType)
class ANDROIDTEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void SetCharacterState(EMainCharacterState CharacterState_);
	UFUNCTION(BlueprintPure, Category="Character")
		EMainCharacterState GetCharacterState() const;

public:
	/*delegates:*/
	UPROPERTY(BlueprintAssignable, Category = "Delegates", DisplayName = "OnCharacterStateChange")
		FOnCharacterStateChangeDelegate OnCharacterStateChangeDelegate;
	
protected:
	EMainCharacterState CharacterState;

};
