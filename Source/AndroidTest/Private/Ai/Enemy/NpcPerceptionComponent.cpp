// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/NpcPerceptionComponent.h"

#include "AIController.h"
#include "Utils/UtilsStructs.h"


UNpcPerceptionComponent::UNpcPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UNpcPerceptionComponent::GetHostileActors(TArray<AActor*>& OutActors) const
{
	const auto TempOwner = GetOwner();
	// TODO Natural actors that should set as hostile only if you gave him a hit
	GetFilteredActors([TempOwner](const FActorPerceptionInfo& Info) -> bool
	{
		if (!Info.HasAnyKnownStimulus())
			return false;
		if (Info.Target.IsStale())
			return false;
		const auto OtherTeamIdComponent = Info.Target->GetComponentByClass<UTeamIdComponent>();
		if(!OtherTeamIdComponent)
			return false;

		auto SelfTeamIdComponent = TempOwner->GetComponentByClass<UTeamIdComponent>();
		if (!SelfTeamIdComponent)
		{
			if(const auto Controller = Cast<AAIController>(TempOwner); Controller)
				SelfTeamIdComponent = Controller->GetPawn()->GetComponentByClass<UTeamIdComponent>();
			if(!SelfTeamIdComponent)
				return false;
		}

		return OtherTeamIdComponent->IsEnemyWith(SelfTeamIdComponent->GetTeamId());
	}, OutActors);
}


