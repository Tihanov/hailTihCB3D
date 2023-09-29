// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/ANpcEnemyController.h"

#include "Log.h"
#include "Ai/Enemy/NpcPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"


AANpcEnemyController::AANpcEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyPerceptionComponent = CreateDefaultSubobject<UNpcPerceptionComponent>("EnemyPerceptionComponent");
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

void AANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	TArray<AActor*> Actors;
	GetEnemyPerceptionComponent()->GetHostileActors(Actors);
	if(Actors.Num() == 0)
	{
		ChaiseActor(nullptr);
		return;
	}

	float Dist;
	const auto ToChaise = UGameplayStatics::FindNearestActor(GetPawn()->GetActorLocation(), Actors, Dist);
	ChaiseActor(ToChaise);
}

void AANpcEnemyController::PawnDamageCallback(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, HitLocation, HitLocation);
}


