// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/ANpcEnemyController.h"

#include "Log.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"


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

void AANpcEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InPawn->OnTakePointDamage.AddDynamic(this, &AANpcEnemyController::PawnDamageCallback);
}

void AANpcEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChasingActorCheck(DeltaTime);
	//FocusCheck(DeltaTime);
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
	LastChasingActor = OldActor;
}

const AActor* AANpcEnemyController::GetLastChasingActor(bool& OutDoesExists) const
{
	OutDoesExists = LastChasingActor.IsValid();
	return LastChasingActor.Get();
}

void AANpcEnemyController::FocusOnChasingActor()
{
	bIsFocusingNow = true;
}

void AANpcEnemyController::StopFocusingOnChasingActor()
{
	bIsFocusingNow = false;
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

void AANpcEnemyController::FocusCheck(float DeltaTime)
{
	if (!bIsFocusingNow)
		return;
	if(CurrentChasingActor.IsNull())
	{
		StopFocusingOnChasingActor();
		return;
	}

	const auto TurnAngle = FMath::RadiansToDegrees(FMath::Acos(
		FVector::DotProduct(-GetPawn()->GetActorForwardVector(), CurrentChasingActor->GetActorForwardVector())));

	if(FMath::Abs(TurnAngle) > FMath::Abs(FocusTurnAngle))
	{
	}
}

void AANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(CurrentChasingActor.IsNull() && UpdateInfo.Target.IsValid())
		ChaiseActor(UpdateInfo.Target.Get());
}

void AANpcEnemyController::PawnDamageCallback(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, HitLocation, HitLocation);
}


