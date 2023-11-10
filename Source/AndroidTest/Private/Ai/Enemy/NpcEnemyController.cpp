// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/NpcEnemyController.h"

#include "Log.h"
#include "Ai/Enemy/NpcPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


ANpcEnemyController::ANpcEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyPerceptionComponent = CreateDefaultSubobject<UNpcPerceptionComponent>("EnemyPerceptionComponent");
	SetPerceptionComponent(*EnemyPerceptionComponent);
}

void ANpcEnemyController::BeginPlay()
{
	Super::BeginPlay();

	EnemyPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(
		this,
		&ANpcEnemyController::ActorPerceptionInfoUpdatedCallback);
}

void ANpcEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InPawn->OnTakePointDamage.AddDynamic(this, &ANpcEnemyController::PawnDamageCallback);
}

void ANpcEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChaiseStressUpdate(DeltaTime);
}

AActor* ANpcEnemyController::GetTargetActor(bool& IsNull) const
{
	IsNull = TargetActor.IsNull();
	return TargetActor.Get();
}

void ANpcEnemyController::SetTargetActor(AActor* NewActor)
{
	if(NewActor == TargetActor.Get())
		return;
	const auto OldActor = TargetActor.Get();
	TargetActor = NewActor;
	OnTargetActorSetDelegate.Broadcast(this, NewActor);
	LastTargetActor = OldActor; 
}

const AActor* ANpcEnemyController::GetLastTargetActor(bool& OutDoesExist) const
{
	OutDoesExist = LastTargetActor.IsValid();
	return LastTargetActor.Get();
}

void ANpcEnemyController::SetStressProgress(float InChaiseStressProgress)
{
	if(IsFreezeStressProgress())
		return;
	StressProgress = FMath::Clamp(InChaiseStressProgress, 0.f, 1.f);
}

void ANpcEnemyController::AddDeltaToStressProgress(float Delta)
{
	SetStressProgress( GetStressProgress() + Delta );
}

void ANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(!UpdateInfo.Target.IsValid())
		return;
	if( GetTargetActor() && GetTargetActor() == UpdateInfo.Target.Get() && !UpdateInfo.Stimulus.WasSuccessfullySensed())
	{
		SetTargetActor(nullptr);
		return;
	}
	if( !GetTargetActor() && UpdateInfo.Stimulus.WasSuccessfullySensed())
		SetTargetActor(UpdateInfo.Target.Get());
	if( GetTargetActor() && UpdateInfo.Target == GetTargetActor())
	{
		// If Actor was damaged
		if(UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
			AddDeltaToStressProgress(DamageAdder);
	}
}

void ANpcEnemyController::PawnDamageCallback(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, HitLocation, HitLocation);
}

void ANpcEnemyController::ChaiseStressUpdate(float DeltaTime)
{
	if (TargetActor.IsNull() || IsFreezeStressProgress())
	{
		if(StressProgress != 0.f)
			AddDeltaToStressProgress(NoPerceptionAdderEverySecond * DeltaTime);
		return;
	}
	const FActorPerceptionInfo* Info;
	if(Info = EnemyPerceptionComponent->GetActorInfo(*TargetActor.Get()); !Info && !Info->HasAnyCurrentStimulus())
	{
		AddDeltaToStressProgress(NoPerceptionAdderEverySecond * DeltaTime);
		return;
	}
	const auto SensedStimuli = Info->LastSensedStimuli;

	const auto Distance = FVector::Distance(GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
	const auto SightConfig = Cast<UAISenseConfig_Sight>(
		GetEnemyPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID(UAISense_Sight::StaticClass())));
	const auto HearingConfig = Cast<UAISenseConfig_Hearing>(
		GetEnemyPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID(UAISense_Hearing::StaticClass())));
	
	const auto SightStressAdder =
		(DoInterpolationOnSightDependsOnDistance
			? UKismetMathLibrary::Lerp(SightAdderEverySecondMin, SightAdderEverySecondMax, Distance / SightConfig->SightRadius)
			: SightAdderEverySecondMax);
	const auto HearingStressAdder =
		(DoInterpolationOnHearingDependsOnDistance
			? UKismetMathLibrary::Lerp(HearingAdderEverySecondMin, HearingAdderEverySecondMax, Distance / HearingConfig->HearingRange)
			: HearingAdderEverySecondMax);

	if(SensedStimuli.ContainsByPredicate( [](const FAIStimulus& Stimulus)-> bool {
		return Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()) && !Stimulus.IsExpired();}))
	{
		AddDeltaToStressProgress(SightStressAdder * DeltaTime);
		ULog::Warning("Sight", LO_Both);
	}
	if(SensedStimuli.ContainsByPredicate( [](const FAIStimulus& Stimulus)-> bool {
		return Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()) && !Stimulus.IsExpired();}))
		AddDeltaToStressProgress(HearingStressAdder * DeltaTime);
}


