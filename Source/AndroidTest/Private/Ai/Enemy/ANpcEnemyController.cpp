// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/Enemy/ANpcEnemyController.h"

#include "Log.h"
#include "Ai/Enemy/NpcPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


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

	ChaiseStressUpdate(DeltaTime);
}

AActor* AANpcEnemyController::GetCurrentNoticedActor(EHostileNoticeState& OutHostileNoticeState) const
{
	OutHostileNoticeState = HostileNoticeState;
	return CurrentNoticingActor.Get();
}

void AANpcEnemyController::NoticeActor(AActor* NewActor)
{
	if(!NewActor)
		SetChaiseStressProgress(0.f);
	else
		SetChaiseStressProgress(OnNoticedAdder);
	const auto OldActor = CurrentNoticingActor.Get();
	CurrentNoticingActor = NewActor;
	OnNoticeActorDelegate.Broadcast(this, NewActor, HostileNoticeState, OldActor);
	LastNoticingActor = OldActor;
}

const AActor* AANpcEnemyController::GetLastNoticedActor(bool& OutDoesExists) const
{
	OutDoesExists = LastNoticingActor.IsValid();
	return LastNoticingActor.Get();
}

void AANpcEnemyController::SetChaiseStressProgress(float InChaiseStressProgress)
{
	ChaiseStressProgress = FMath::Clamp(InChaiseStressProgress, 0.f, 1.f);
	if(ChaiseStressProgress == 0.f && GetHostileNoticeState() != EHostileNoticeState::None)
		SetHostileNoticeState(EHostileNoticeState::None);
	if(ChaiseStressProgress == 1.f && GetHostileNoticeState() != EHostileNoticeState::Chaise)
		SetHostileNoticeState(EHostileNoticeState::Chaise);
	if(ChaiseStressProgress != 0.f && ChaiseStressProgress != 1.f && GetHostileNoticeState() != EHostileNoticeState::Notice)
		SetHostileNoticeState(EHostileNoticeState::Notice);
}

void AANpcEnemyController::AddDeltaToChaiseStressProgress(float Delta)
{
	SetChaiseStressProgress( GetChaiseStressProgress() + Delta );
}

void AANpcEnemyController::SetHostileNoticeState(EHostileNoticeState InHostileNoticeState)
{
	HostileNoticeState = InHostileNoticeState;
	OnNoticeStateUpdateDelegate.Broadcast(this, CurrentNoticingActor.Get(), InHostileNoticeState);
}

void AANpcEnemyController::ActorPerceptionInfoUpdatedCallback(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(UpdateInfo.Target == CurrentNoticingActor.Get())
	{
		// If Actor was damaged
		if(UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
			AddDeltaToChaiseStressProgress(DamageAdder);
		return;
	}
	TArray<AActor*> Actors;
	GetEnemyPerceptionComponent()->GetHostileActors(Actors);
	if(Actors.Num() == 0)
	{
		NoticeActor(nullptr);
		return;
	}
	EHostileNoticeState Hns;
	if (const auto ChasingActor = GetCurrentNoticedActor(Hns);
		Hns != EHostileNoticeState::None && Actors.Find(ChasingActor) != INDEX_NONE)
		return;

	float Dist;
	const auto ToChaise = UGameplayStatics::FindNearestActor(GetPawn()->GetActorLocation(), Actors, Dist);
	NoticeActor(ToChaise);
}

void AANpcEnemyController::PawnDamageCallback(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, HitLocation, HitLocation);
}

void AANpcEnemyController::ChaiseStressUpdate(float DeltaTime)
{
	ULog::Number(GetChaiseStressProgress(), "ChaiseStressProgress: ", "", LO_Both);
	if (GetHostileNoticeState() != EHostileNoticeState::Notice || CurrentNoticingActor.IsNull())
		return;
	FActorPerceptionBlueprintInfo Info;
	if(!EnemyPerceptionComponent->GetActorsPerception(CurrentNoticingActor.Get(), Info))
	{
		AddDeltaToChaiseStressProgress(SightDiffEverySecond * DeltaTime);
		AddDeltaToChaiseStressProgress(HearingDiffEverySecond * DeltaTime);
		return;
	}
	const auto SensedStimuli = Info.LastSensedStimuli;
	TArray<FAISenseID> SensedStimuliIds;
	for (const auto& SensedStimul : SensedStimuli)
		SensedStimuliIds.Add(SensedStimul.Type);
	
	if(SensedStimuliIds.Contains(UAISense::GetSenseID(UAISense_Sight::StaticClass())))
		AddDeltaToChaiseStressProgress(SightAdderEverySecond * DeltaTime);
	else
		AddDeltaToChaiseStressProgress(SightDiffEverySecond * DeltaTime);
	if(SensedStimuliIds.Contains(UAISense::GetSenseID(UAISense_Hearing::StaticClass())))
		AddDeltaToChaiseStressProgress(HearingAdderEverySecond * DeltaTime);
	else
		AddDeltaToChaiseStressProgress(HearingDiffEverySecond * DeltaTime);
}


