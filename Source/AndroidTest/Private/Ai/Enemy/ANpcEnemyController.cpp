// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/ANpcEnemyController.h"

#include "Log.h"
#include "Perception/AIPerceptionComponent.h"


AANpcEnemyController::AANpcEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
}

void AANpcEnemyController::BeginPlay()
{
	Super::BeginPlay();

	EnemyPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(
		this,
		&AANpcEnemyController::ActorPerceptionInfoUpdatedCallback);
}

void AANpcEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChasingActorCheck(DeltaTime);
}

AActor* AANpcEnemyController::GetCurrentChasingActor(bool& OutIsChasingActor) const
{
	OutIsChasingActor = CurrentChasingActor.IsValid();
	return CurrentChasingActor.Get();
}

void AANpcEnemyController::ChaiseActor(AActor* NewActor)
{
	const auto OldActor = CurrentChasingActor.Get();
	CurrentChasingActor = NewActor;
	OnChaseNewActorDelegate.Broadcast(NewActor, OldActor);
}

void AANpcEnemyController::ChasingActorCheck(float DeltaTime)
{
	if(CurrentChasingActor.IsNull())
		return;
	const auto Info = EnemyPerceptionComponent->GetActorInfo(*CurrentChasingActor.Get());
	if(Info == nullptr)
		return;
	if(!Info->HasAnyKnownStimulus())
		ChaiseActor(nullptr);
}

void AANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(CurrentChasingActor.IsNull() && UpdateInfo.Target.IsValid())
		ChaiseActor(UpdateInfo.Target.Get());
}

