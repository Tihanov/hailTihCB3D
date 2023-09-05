// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPoints/HealthPointsComponent.h"


UHealthPointsComponent::UHealthPointsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthPointsComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthPoints = MaxHealthPoints;

	auto Owner = GetOwner();
	if(const auto Controller = Cast<AController>(Owner); Controller)
	{
		Controller->GetPawn()->OnTakeAnyDamage.AddDynamic(this, &UHealthPointsComponent::OnTakeAnyDamagePureHandler);
		return;
	}
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthPointsComponent::OnTakeAnyDamagePureHandler);
}

float UHealthPointsComponent::GetHealthPoints() const
{
	return HealthPoints;
}
void UHealthPointsComponent::SetHealthPoints(float Hp)
{
	const auto Prev = HealthPoints;
	HealthPoints = FMath::Clamp(Hp, 0, MaxHealthPoints);
}

float UHealthPointsComponent::GetMaxHealthPoints() const
{
	return MaxHealthPoints;
}
void UHealthPointsComponent::SetMaxHealthPoints(float MaxHp)
{
	const auto Prev = MaxHealthPoints;
	MaxHealthPoints = MaxHp;
	OnHealthPointsMaxChangedDelegate.Broadcast(this, MaxHealthPoints, Prev);	
}

bool UHealthPointsComponent::IsDead() const
{
	return bIsDead;
}

void UHealthPointsComponent::OnTakeAnyDamagePureHandler(AActor* DamagedActor, float Damage,
                                                        const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	auto& TimeManager = GetWorld()->GetTimerManager();

	if(bUseTakeDamageTimeout)
	{
		if(TimeManager.GetTimerElapsed(TakeDamageTimeoutHandler) != -1)
			return;

		TimeManager.SetTimer(
						   TakeDamageTimeoutHandler,TakeDamageTimeout,false,TakeDamageTimeout);
	}
	
	SetHealthPoints(GetHealthPoints() - Damage);
	CheckOnDeath();
}

void UHealthPointsComponent::CheckOnDeath()
{
	if(HealthPoints <= 0 && !bIsDead)
	{
		bIsDead = true;
		OnPawnDeathDelegate.Broadcast(this);
	}
}

