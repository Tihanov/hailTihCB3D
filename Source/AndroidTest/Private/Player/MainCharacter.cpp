// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"

#include "Utils/UtilsStructs.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"PerceptionStimuliSourceComponent");
	PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
}

void AMainCharacter::SetCharacterState(EMainCharacterState CharacterState_)
{
	if(CharacterState_ == CharacterState)
		return;
	CharacterState = CharacterState_;
	OnCharacterStateChangeDelegate.Broadcast(CharacterState);
}

EMainCharacterState AMainCharacter::GetCharacterState() const
{
	return CharacterState;
}

FGenericTeamId AMainCharacter::GetGenericTeamId() const
{
	return {TI_MainPlayer};
}
