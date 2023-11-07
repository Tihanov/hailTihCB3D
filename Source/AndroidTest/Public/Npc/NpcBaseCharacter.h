// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DlgSystem/DlgDialogue.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "AndroidTest/Public/Action/ActionInterface.h"
#include "NpcBaseCharacter.generated.h"

class AStaticMeshActor;

UENUM(BlueprintType)
enum EQuestStatus
{
	QSNone			UMETA(DisplayName = "None"),
	QSHaveQuest		UMETA(DisplayName = "HaveQuest"),
	QSHaveQuestions UMETA(DisplayName = "HaveQuestions"),
};


UCLASS(BlueprintType, Blueprintable)
class ANDROIDTEST_API ANpcBaseCharacter
	: public ACharacter
	, public IDlgDialogueParticipant
	, public IActionInterface
{
	GENERATED_BODY()

public:
	ANpcBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

public:
	/*Implementation of IDlgDialogueParticipant*/
	virtual FName GetParticipantName_Implementation() const override;
	virtual FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override;
	
	/*Implementation of IActionInterface*/
	virtual EActionType GetActionType_Implementation() override;
	virtual void DoAction_Implementation(AActor* CausedBy) override;
	virtual FText GetDisplayDescription_Implementation() const override;
	virtual UTexture2D* GetIco_Implementation() const override;
	virtual bool CanDoAction_Implementation() const override;
	
	/*Getters, Setters*/
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Ai|Dialogue")
		UDlgDialogue* GetDialogue() const;
	UFUNCTION(BlueprintCallable, Category = "Ai|Dialogue")
		void SetDialogue(UDlgDialogue* Dialogue_, TEnumAsByte<EQuestStatus> QuestStatus_);

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Ai|Quest")
		TEnumAsByte<EQuestStatus> GetQuestStatus() const;
	UFUNCTION(BlueprintSetter, BlueprintCallable, Category = "Ai|Quest")
		void SetQuestStatus(TEnumAsByte<EQuestStatus> QuestStatus_);

	
	/*Vars*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ai|Dialogue")
		FName DlgSystemName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ai|Dialogue")
		FText DlgDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName = "CanPlayerSpeakWith?", Category = "Ai|Dialogue")
		bool bCanPlayerSpeakWith = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Ai|Quest|Mesh")
		UStaticMesh* QstHaveQuestStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Ai|Quest|Mesh")
		UStaticMesh* QstHaveQuestionsStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Ai|Quest|Mesh")
		UStaticMesh* QstNoneStaticMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components", DisplayName = "QuestSign")
		UStaticMeshComponent* QuestSignMeshComponent;
	
protected:
	UPROPERTY(BlueprintGetter = GetDialogue, EditInstanceOnly, Category = "Ai|Dialogue")
		UDlgDialogue* Dialogue;
	UPROPERTY(BlueprintGetter = GetQuestStatus, BlueprintSetter = SetQuestStatus, EditInstanceOnly, Category = "Ai|Quest")
		TEnumAsByte<EQuestStatus> QuestStatus;
};
