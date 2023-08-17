// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

EMainCharacterState AMainCharacter::GetCharacterState() const
{
	return CharacterState;
}

void AMainCharacter::SetCharacterState(EMainCharacterState CharacterState_)
{
	if(CharacterState_ == CharacterState)
		return;
	CharacterState = CharacterState_;
	OnCharacterStateChangeDelegate.Broadcast(CharacterState);
}


