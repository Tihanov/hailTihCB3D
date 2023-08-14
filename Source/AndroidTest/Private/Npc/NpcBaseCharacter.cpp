// Fill out your copyright notice in the Description page of Project Settings.


#include "Npc/NpcBaseCharacter.h"

#include "Log.h"
#include "Player/MainPlayerController.h"


ANpcBaseCharacter::ANpcBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	QuestSignMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("QuestSign");
	QuestSignMeshComponent->SetupAttachment(GetRootComponent());
}

void ANpcBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetQuestStatus(QuestStatus);
}

FName ANpcBaseCharacter::GetParticipantName_Implementation() const
{
	return DlgSystemName;
}
FText ANpcBaseCharacter::GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const
{
	return DlgDisplayName;
}

EActionType ANpcBaseCharacter::GetActionType_Implementation()
{
	return EActionType::NPCDialogue;
}

void ANpcBaseCharacter::DoAction_Implementation(AActor* CausedBy)
{
	if(const auto PlayerController = Cast<AMainPlayerController>(CausedBy); PlayerController != nullptr)
	{
		PlayerController->StartDialogue(Dialogue, TArray<AActor*>{this});
	}
}

FText ANpcBaseCharacter::GetDisplayDescription_Implementation() const
{
	return DlgDisplayName;
}

UTexture2D* ANpcBaseCharacter::GetIco_Implementation() const
{
	return nullptr;
}

bool ANpcBaseCharacter::CanDoAction_Implementation() const
{
	return bCanPlayerSpeakWith;
}

UDlgDialogue* ANpcBaseCharacter::GetDialogue() const
{
	return Dialogue;
}
void ANpcBaseCharacter::SetDialogue(UDlgDialogue* Dialogue_, TEnumAsByte<EQuestStatus> QuestStatus_)
{
	Dialogue = Dialogue_;
	SetQuestStatus(QuestStatus_);
}

TEnumAsByte<EQuestStatus> ANpcBaseCharacter::GetQuestStatus() const
{
	return QuestStatus;
}
void ANpcBaseCharacter::SetQuestStatus(TEnumAsByte<EQuestStatus> QuestStatus_)
{
	QuestStatus = QuestStatus_;
	switch (QuestStatus)
	{
	case QSNone: QuestSignMeshComponent->SetStaticMesh(QstNoneStaticMesh); break;
	case QSHaveQuest: QuestSignMeshComponent->SetStaticMesh(QstHaveQuestStaticMesh); break;
	case QSHaveQuestions: QuestSignMeshComponent->SetStaticMesh(QstHaveQuestionsStaticMesh); break;
	}
}
