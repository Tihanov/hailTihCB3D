// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/DEPRECATED_AEnemyNpcAiCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/UtilsStructs.h"
#include "Perception/AIPerceptionComponent.h"

ADEPRECATED_AEnemyNpcAiCharacter::ADEPRECATED_AEnemyNpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

void ADEPRECATED_AEnemyNpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this,
		&ADEPRECATED_AEnemyNpcAiCharacter::ActorPerceptionUpdateCallBack);
}

void ADEPRECATED_AEnemyNpcAiCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AActor* ADEPRECATED_AEnemyNpcAiCharacter::GetFocusedActor() const
{
	return FocusedActor;
}

void ADEPRECATED_AEnemyNpcAiCharacter::SetFocusedActor(AActor* InFocusedActor)
{
	OnFocusedActorChanged.Broadcast(InFocusedActor);
	this->FocusedActor = InFocusedActor;
}

FVector ADEPRECATED_AEnemyNpcAiCharacter::GetLastPositionOfFocusedActor() const
{
	return LastPositionOfFocusedActor;
}

void ADEPRECATED_AEnemyNpcAiCharacter::ActorPerceptionUpdateCallBack(const FActorPerceptionUpdateInfo& Info)
{
	if(Info.Stimulus.IsActive() && !GetFocusedActor())
	{
		ensure(Info.Target.IsValid());
		SetFocusedActor(Info.Target.Get());
	}
	else if(!Info.Stimulus.IsActive() && GetFocusedActor() == Info.Target.Get())
	{
		TArray<AActor*> PercActors; 
		PerceptionComponent->GetCurrentlyPerceivedActors(nullptr, PercActors);
		if(PercActors.Num() == 0)
		{
			SetFocusedActor(nullptr);
			ensure(Info.Target.IsValid());
			LastPositionOfFocusedActor = Info.Target.Get()->GetActorLocation();
			return;
		}
		float Distance;
		const auto ToFocus = UGameplayStatics::FindNearestActor(GetActorLocation(), PercActors, Distance);
		SetFocusedActor(ToFocus);
	}
}

