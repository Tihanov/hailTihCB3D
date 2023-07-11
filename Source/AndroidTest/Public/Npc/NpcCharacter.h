// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DlgSystem/DlgDialogue.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "AndroidTest/Public/Action/ActionInterface.h"
#include "NpcCharacter.generated.h"

class AStaticMeshActor;

UENUM(BlueprintType)
enum EQuestStatus
{
	QSNone			UMETA(DisplayName = "None"),
	QSHaveQuest		UMETA(DisplayName = "HaveQuest"),
	QSHaveQuestions UMETA(DisplayName = "HaveQuestions"),
};


UCLASS(BlueprintType, Blueprintable)
class ANDROIDTEST_API ANpcCharacter
	: public ACharacter
	, public IDlgDialogueParticipant
	, public IActionInterface
{
	GENERATED_BODY()

public:
	ANpcCharacter();

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
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Dialogue")
		UDlgDialogue* GetDialogue() const;
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
		void SetDialogue(UDlgDialogue* Dialogue_, TEnumAsByte<EQuestStatus> QuestStatus_);

	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Quest")
		TEnumAsByte<EQuestStatus> GetQuestStatus() const;
	UFUNCTION(BlueprintSetter, BlueprintCallable, Category = "Quest")
		void SetQuestStatus(TEnumAsByte<EQuestStatus> QuestStatus_);

	
	/*Vars*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Dialogue")
		FName DlgSystemName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
		FText DlgDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName = "CanPlayerSpeakWith?", Category = "Dialogue")
		bool bCanPlayerSpeakWith = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Quest|Mesh")
		UStaticMesh* QstHaveQuestStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Quest|Mesh")
		UStaticMesh* QstHaveQuestionsStaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "Quest|Mesh")
		UStaticMesh* QstNoneStaticMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components", DisplayName = "QuestSign")
		UStaticMeshComponent* QuestSignMeshComponent;
	
private:
	UPROPERTY(BlueprintGetter = GetDialogue, EditInstanceOnly, Category = "Dialogue")
		UDlgDialogue* Dialogue;
	UPROPERTY(BlueprintGetter = GetQuestStatus, BlueprintSetter = SetQuestStatus, EditInstanceOnly, Category = "Quest")
		TEnumAsByte<EQuestStatus> QuestStatus;
};
