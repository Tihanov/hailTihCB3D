// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/NpcEnemyController.h"

#include "Log.h"
#include "Ai/Enemy/NpcPerceptionComponent.h"
#include "Ai/Utils/Components/AIShootComponent.h"
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
	AIShootComponent = CreateDefaultSubobject<UAIShootComponent>("AIShootComponent");
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

void ANpcEnemyController::SetHostilePoint(FVector HostilePoint)
{
	SetFocalPoint(HostilePoint, EAIFocusPriority::Default);
	OnHostileActorUpdateDelegate.Broadcast(this, GetHostileActor(), GetHostilePoint());
}

void ANpcEnemyController::SetHostileActor(AActor* HostileActor)
{
	SetFocus(HostileActor, EAIFocusPriority::Default);
	OnHostileActorUpdateDelegate.Broadcast(this, GetHostileActor(), GetHostilePoint());
}

void ANpcEnemyController::SetHostilePointFromHostileActor()
{
	if(!GetHostileActor())
		return;
	const auto LastLoc = GetHostileActor()->GetActorLocation();
	ClearHostileActor();
	SetHostilePoint(LastLoc);
}

AActor* ANpcEnemyController::GetHostileActor() const
{
	return GetFocusActorForPriority(EAIFocusPriority::Default);
}

FVector ANpcEnemyController::GetHostilePoint() const
{
	return GetFocalPointForPriority(EAIFocusPriority::Default);
}

void ANpcEnemyController::ClearHostileActor()
{
	ClearFocus(EAIFocusPriority::Default);
	OnHostileActorUpdateDelegate.Broadcast(this, GetHostileActor(), GetHostilePoint());
}

FVector ANpcEnemyController::GetFocalPointOnActor(const AActor* Actor) const
{
	if( !Actor || GetHostileActor() != Actor )
		return Super::GetFocalPointOnActor(Actor);
	
	const FActorPerceptionInfo* Info;
	const auto SenseSightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	if(Info = EnemyPerceptionComponent->GetActorInfo(*GetFocusActor()); !Info || !Info->HasAnyCurrentStimulus() || !Info->IsSenseActive(SenseSightID))
		return Super::GetFocalPointOnActor(Actor);
	
	return Info->GetStimulusLocation( SenseSightID );
}

bool ANpcEnemyController::IsHigherPrioritiesSets(EAIFocusPriority::Type ThanThis) const
{
	for (int32 Index = FocusInformation.Priorities.Num() - 1; Index > ThanThis; --Index)
	{
		if(FocusPrioritySet(FocusInformation.Priorities[Index]))
			return true;
	}
	return false;
}

bool ANpcEnemyController::FocusPrioritySet(const FFocusKnowledge::FFocusItem& FocusItem)
{
	return FAISystem::IsValidLocation(FocusItem.Position) || FocusItem.Actor.IsValid();
}

void ANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(!UpdateInfo.Target.IsValid())
		return;
	if( IsSenseHostile() && UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()) )
	{
		if (GetHostileActor() && GetHostileActor() == UpdateInfo.Target.Get()
			&& !UpdateInfo.Stimulus.WasSuccessfullySensed())
		{
			SetHostilePointFromHostileActor();
			return;
		}
		if (!GetHostileActor() && UpdateInfo.Stimulus.WasSuccessfullySensed())
		{
			SetHostileActor(UpdateInfo.Target.Get());
		}
	}
	if( GetHostileActor() && UpdateInfo.Target == GetHostileActor())
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
	if (!GetHostileActor() || IsFreezeStressProgress())
	{
		if(StressProgress != 0.f)
			AddDeltaToStressProgress(NoPerceptionAdderEverySecond * DeltaTime);
		return;
	}
	const FActorPerceptionInfo* Info;
	if(Info = EnemyPerceptionComponent->GetActorInfo(*GetHostileActor()); !Info || !Info->HasAnyCurrentStimulus())
	{
		AddDeltaToStressProgress(NoPerceptionAdderEverySecond * DeltaTime);
		return;
	}
	const auto SensedStimuli = Info->LastSensedStimuli;

	const auto Distance = FVector::Distance(GetPawn()->GetActorLocation(), GetHostileActor()->GetActorLocation());
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
	}
	if(SensedStimuli.ContainsByPredicate( [](const FAIStimulus& Stimulus)-> bool {
		return Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()) && !Stimulus.IsExpired();}))
		AddDeltaToStressProgress(HearingStressAdder * DeltaTime);
}


