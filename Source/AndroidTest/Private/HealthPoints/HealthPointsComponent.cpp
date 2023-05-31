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
	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthPointsComponent::OnTakeAnyDamagePureHandler);
}

float UHealthPointsComponent::GetHealthPoints() const
{
	return HealthPoints;
}
void UHealthPointsComponent::SetHealthPoints(float Hp)
{
	const auto Prev = HealthPoints;
	HealthPoints = Hp;
	OnHealthPointsChangedDelegate.Broadcast(this, HealthPoints, Prev);	
}

float UHealthPointsComponent::GetMaxHealthPoints() const
{
	return MaxHealthPoints;
}
void UHealthPointsComponent::SetMaxHealthPoints(float MaxHp)
{
	const auto Prev = HealthPoints;
	MaxHealthPoints = MaxHp;
	OnHealthPointsMaxChangedDelegate.Broadcast(this, HealthPoints, Prev);	
}

bool UHealthPointsComponent::IsDead() const
{
	return HealthPoints <= 0;
}

void UHealthPointsComponent::OnTakeAnyDamagePureHandler(AActor* DamagedActor, float Damage,
                                                        const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	auto& TimeManager = GetWorld()->GetTimerManager();

	if(TimeManager.GetTimerElapsed(DamageTimeoutHandler) != -1)
		return;

	TimeManager.SetTimer(
               		DamageTimeoutHandler,DamageTimeout,false,DamageTimeout);
	
	SetHealthPoints(GetHealthPoints() - Damage);
	CheckOnDeath();
}

void UHealthPointsComponent::CheckOnDeath()
{
	if(HealthPoints <= 0)
	{
		OnPawnDeathDelegate.Broadcast(this);
	}
}

