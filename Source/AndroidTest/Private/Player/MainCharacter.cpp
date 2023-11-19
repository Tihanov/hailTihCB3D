// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
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
	if (CharacterState_ == CharacterState)
		return;
	CharacterState = CharacterState_;
	OnCharacterStateChangeDelegate.Broadcast(CharacterState);
}

EMainCharacterState AMainCharacter::GetCharacterState() const
{
	return CharacterState;
}

UAISense_Sight::EVisibilityResult AMainCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
                                                                FVector& OutSeenLocation,
                                                                int32& OutNumberOfLoSChecksPerformed,
                                                                int32& OutNumberOfAsyncLosCheckRequested,
                                                                float& OutSightStrength, int32* UserData,
                                                                const FOnPendingVisibilityQueryProcessedDelegate*
                                                                Delegate)

{
	if (SightSockets.Num() == 0)
	{
		return IAISightTargetInterface::CanBeSeenFrom(Context, OutSeenLocation, OutNumberOfLoSChecksPerformed,
		                                              OutNumberOfAsyncLosCheckRequested, OutSightStrength, UserData,
		                                              Delegate);
	}

	for (const auto& SightSocket : SightSockets)
	{
		const auto HeadLocation = GetMesh()->GetSocketLocation(SightSocket);
		FHitResult HitResult;
		const bool bTraceSuccess = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Context.ObserverLocation, HeadLocation,
																  TraceTypeQuery1, false,
																  TArray<AActor*>{const_cast<AActor*>(Context.IgnoreActor)},
																  EDrawDebugTrace::None, HitResult, true);
		OutNumberOfLoSChecksPerformed += 1;
		if (bTraceSuccess && HitResult.GetActor())
		{
			OutSeenLocation = HeadLocation;
			OutSightStrength = 1;
			return UAISense_Sight::EVisibilityResult::Visible;
		}
	}
	OutSightStrength = 0;
	return UAISense_Sight::EVisibilityResult::NotVisible;
}
