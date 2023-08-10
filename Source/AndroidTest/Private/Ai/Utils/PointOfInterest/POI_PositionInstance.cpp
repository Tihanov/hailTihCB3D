// Fill out your copyright notice in the Description page of Project Settings.


#include "AndroidTest\Public\Ai\Utils\PointOfInterest\POI_PositionInstance.h"

#include "Ai/Villager/VillagerAiController.h"
#include "Components/SphereComponent.h"

UPOI_PositionInstance::UPOI_PositionInstance()
{
	POI_ToSpawnClass = APOI_Position::StaticClass();
}



APOI_Position::APOI_Position()
{
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APOI_Position::Init_Implementation(UAiPointOfInterestInstance* Instance, AVillagerAiController* AiController)
{
	Super::Init_Implementation(Instance, AiController);

	const auto Inst = GetInstance<UPOI_PositionInstance>();

	SphereCollisionComponent->InitSphereRadius(Inst->CollisionRadius);
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionComponent->SetCollisionProfileName("OverlapAll", true);
	SphereCollisionComponent->SetCollisionObjectType(ECC_WorldStatic);
	
	if(Inst->PositionSource == EPOI_PositionInstancePositionSource::Location)
	{
		SetActorLocation(Inst->Location);
		return;
	}
	if(Inst->PositionSource == EPOI_PositionInstancePositionSource::Actor)
	{
		check(Inst->Actor.IsValid());
		if(Inst->bAttachToActor)
		{
			auto Rules = FAttachmentTransformRules{
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepRelative,
				EAttachmentRule::KeepRelative,
				false};

			this->AttachToActor(Inst->Actor.Get(), Rules);
			return;
		}
		
		this->SetActorLocation(Inst->Actor.Get()->GetActorLocation());
		return;
	}

	check(false);
}

bool APOI_Position::IsComplete_Implementation() const
{
	return bIsComplete;
}

bool APOI_Position::IsArrived_Implementation() const
{
	return bIsArrived;	
}

void APOI_Position::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(Cast<APawn>(OtherActor) != GetAiController()->GetPawn())
		return;

	OnArrived();
	
	const auto Inst = GetInstance<UPOI_PositionInstance>();
	if(Inst->OnArrivedTimeout == 0.f)
	{
		OnComplete();
		return;
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		this,
		&APOI_Position::OnComplete,
		Inst->OnArrivedTimeout);
}

void APOI_Position::OnComplete()
{
	Super::OnComplete();
	
	bIsComplete = true;
}

void APOI_Position::OnArrived()
{
	Super::OnArrived();
	
	bIsArrived = true;

}
