// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc/NpcCharacter.h"

#include "Log.h"
#include "Player/MainPlayerController.h"


ANpcCharacter::ANpcCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	QuestSignMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("QuestSign");
	QuestSignMeshComponent->SetupAttachment(GetRootComponent());
}

void ANpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetQuestStatus(QuestStatus);
}

FName ANpcCharacter::GetParticipantName_Implementation() const
{
	return DlgSystemName;
}
FText ANpcCharacter::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return DlgDisplayName;
}

EActionType ANpcCharacter::GetActionType_Implementation()
{
	return EActionType::NPCDialogue;
}

void ANpcCharacter::DoAction_Implementation(AActor* CausedBy)
{
	if(const auto PlayerController = Cast<AMainPlayerController>(CausedBy); PlayerController != nullptr)
	{
		PlayerController->StartDialogue(Dialogue, TArray<AActor*>{this});
	}
}

FText ANpcCharacter::GetDisplayDescription_Implementation() const
{
	return DlgDisplayName;
}

UTexture2D* ANpcCharacter::GetIco_Implementation() const
{
	return nullptr;
}

bool ANpcCharacter::CanDoAction_Implementation() const
{
	return bCanPlayerSpeakWith;
}

UDlgDialogue* ANpcCharacter::GetDialogue() const
{
	return Dialogue;
}
void ANpcCharacter::SetDialogue(UDlgDialogue* Dialogue_, TEnumAsByte<EQuestStatus> QuestStatus_)
{
	Dialogue = Dialogue_;
	SetQuestStatus(QuestStatus_);
}

TEnumAsByte<EQuestStatus> ANpcCharacter::GetQuestStatus() const
{
	return QuestStatus;
}
void ANpcCharacter::SetQuestStatus(TEnumAsByte<EQuestStatus> QuestStatus_)
{
	QuestStatus = QuestStatus_;
	switch (QuestStatus)
	{
	case QSNone: QuestSignMeshComponent->SetStaticMesh(QstNoneStaticMesh); break;
	case QSHaveQuest: QuestSignMeshComponent->SetStaticMesh(QstHaveQuestStaticMesh); break;
	case QSHaveQuestions: QuestSignMeshComponent->SetStaticMesh(QstHaveQuestionsStaticMesh); break;
	}
}
