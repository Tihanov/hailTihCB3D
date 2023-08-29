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

	if(CurrentChasingActor.IsValid())
		ULog::ObjectName(CurrentChasingActor.Get(), false, LO_Both);
	else
		ULog::Info("nullptr", LO_Both);

	ChasingActorCheck(DeltaTime);
}

AActor* AANpcEnemyController::GetCurrentChasingActor(bool& OutIsChasingActor) const
{
	OutIsChasingActor = CurrentChasingActor.IsNull();
	return CurrentChasingActor.Get();
}

void AANpcEnemyController::ChasingActorCheck(float DeltaTime)
{
	if(CurrentChasingActor.IsNull())
		return;
	const auto Info = EnemyPerceptionComponent->GetActorInfo(*CurrentChasingActor.Get());
	if(Info == nullptr)
		return;
	if(!Info->HasAnyKnownStimulus())
		CurrentChasingActor.Reset();
}

void AANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(CurrentChasingActor.IsNull() && UpdateInfo.Target.IsValid())
		CurrentChasingActor = UpdateInfo.Target.Get();
}

