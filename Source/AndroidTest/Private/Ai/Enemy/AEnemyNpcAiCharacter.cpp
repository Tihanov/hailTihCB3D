// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/AEnemyNpcAiCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/UtilsStructs.h"
#include "Perception/AIPerceptionComponent.h"

AAEnemyNpcAiCharacter::AAEnemyNpcAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

void AAEnemyNpcAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this,
		&AAEnemyNpcAiCharacter::ActorPerceptionUpdateCallBack);
}

void AAEnemyNpcAiCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FGenericTeamId AAEnemyNpcAiCharacter::GetGenericTeamId() const
{
	return TI_DefaultEnemy;
}

AActor* AAEnemyNpcAiCharacter::GetFocusedActor() const
{
	return FocusedActor;
}

void AAEnemyNpcAiCharacter::SetFocusedActor(AActor* InFocusedActor)
{
	OnFocusedActorChanged.Broadcast(InFocusedActor);
	this->FocusedActor = InFocusedActor;
}

FVector AAEnemyNpcAiCharacter::GetLastPositionOfFocusedActor() const
{
	return LastPositionOfFocusedActor;
}

void AAEnemyNpcAiCharacter::ActorPerceptionUpdateCallBack(const FActorPerceptionUpdateInfo& Info)
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

