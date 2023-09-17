// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Utils/PoiCallbacks/TurnAsActorCallback.h"

#include "Log.h"
#include "Ai/Npc/NpcAiController.h"

void UTurnAsActorCallback::Execute_Implementation(AAiPointOfInterest* PointOfInterest)
{
	Pawn = PointOfInterest->GetAiController()->GetPawn();

	Pawn->GetWorld()->GetTimerManager().SetTimer(TurnAsActorTimerHandle, this,
	                                             &UTurnAsActorCallback::TurnAsActorCallback, RotationDelay);
}

void UTurnAsActorCallback::TurnAsActorCallback()
{
	const auto Actor = InActor.Get();
	if(!Actor)
	{
		ULog::Error("Cant dereference InActor", LO_Both);
		return;
	}

	Pawn->SetActorRotation(Actor->GetActorRotation());
}
