// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerController.h"

#include "Log.h"
#include "Ai/Utils/NpcUtils.h"
#include "DlgSystem/DlgContext.h"
#include "DlgSystem/DlgDialogue.h"
#include "DlgSystem/DlgManager.h"


AMainPlayerController::AMainPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(ENpcTeamId::NTI_MainPlayer);
}

void AMainPlayerController::StartDialogue(UDlgDialogue* Dialogue, TArray<AActor*> Participants)
{
	check(Dialogue);

	Participants.Add(this);
	OnDialogueAboutToStartDelegate.Broadcast(this, {Participants});
	TArray<UObject*> Temp;
	for (const auto Participant : Participants)
		Temp.Add(Cast<UObject>(Participant));	
	DlgContext = UDlgManager::StartDialogue(Dialogue, Temp);
	if(!DlgContext)
	{
		ULog::Warning("Cant start dialogue", LO_Both);
		return;
	}
	
	OnDialogueStartedDelegate.Broadcast(this, {Participants} ,DlgContext);
}

void AMainPlayerController::SelectDialogueOption(int OptionIndex)
{
	if(!DlgContext || !DlgContext->IsValidOptionIndex(OptionIndex))
		return;
	const auto LastOption = DlgContext->GetOption(OptionIndex);
	if(DlgContext->ChooseOption(OptionIndex))
		OnDialogueUpdatedDelegate.Broadcast(this, LastOption, DlgContext);
	else
	{
		DlgContext = nullptr;
		OnDialogueFinishedDelegate.Broadcast(this);
	}
}

void AMainPlayerController::OpenChest_Implementation(AChestCollision* ChestCollision)
{
}

FText AMainPlayerController::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return DlgDisplayName;
}

FName AMainPlayerController::GetParticipantName_Implementation() const
{
	return DlgSystemName;
}

