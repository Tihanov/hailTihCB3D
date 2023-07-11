// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DlgSystem/DlgDialogueParticipant.h"
#include "MainPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FProxyStructArrayOfParticipants
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
		TArray<AActor*> Participants;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueAboutToStartDelegate,
	APlayerController*, PlayerController,
	FProxyStructArrayOfParticipants, Participants);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueStartedDelegate,
	APlayerController*, PlayerController,
	FProxyStructArrayOfParticipants, Participants,
	UDlgContext*, DlgContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueUpdatedDelegate,
	APlayerController*, PlayerController,
	UDlgContext*, DlgContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueFinishedDelegate,
	APlayerController*, PlayerController);

UCLASS()
class ANDROIDTEST_API AMainPlayerController
	: public APlayerController
	, public IDlgDialogueParticipant
{
	GENERATED_BODY()

public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category=Dialogue)
		void StartDialogue(class UDlgDialogue* Dialogue, TArray<AActor*> Participants);
	UFUNCTION(BlueprintCallable, Category=Dialogue)
		void SelectDialogueOption(int OptionIndex);

	/*Implementation of IDlgDialogueParticipant*/
	virtual FText GetParticipantDisplayName_Implementation(FName ActiveSpeaker) const override;
	virtual FName GetParticipantName_Implementation() const override;

public: /*DELEGATES:*/
	UPROPERTY(BlueprintAssignable, Category=Delegate, DisplayName="OnDialogueAboutToStart")
		FOnDialogueAboutToStartDelegate OnDialogueAboutToStartDelegate;
	UPROPERTY(BlueprintAssignable, Category=Delegate, DisplayName="OnDialogueStarted")
		FOnDialogueStartedDelegate OnDialogueStartedDelegate;
	UPROPERTY(BlueprintAssignable, Category=Delegate, DisplayName="OnDialogueUpdated")
		FOnDialogueUpdatedDelegate OnDialogueUpdatedDelegate;
	UPROPERTY(BlueprintAssignable, Category=Delegate, DisplayName="OnDialogueFinished")
		FOnDialogueFinishedDelegate OnDialogueFinishedDelegate;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Dialogue)
		FName DlgSystemName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Dialogue)
		FText DlgDisplayName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Dialogue)
		UDlgContext* DlgContext;
};
