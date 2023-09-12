// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthPointsComponent.generated.h"

class UHealthPointsComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthPointsMaxChangedDelegate,
	UHealthPointsComponent*, HpComponent,
	float, CurrHp,
	float, PrevHpMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDeathDelegate,
	UHealthPointsComponent*, HpComponent);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDROIDTEST_API UHealthPointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthPointsComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintGetter, BlueprintPure, Category=HealthPoints)
		float GetHealthPoints() const;
	UFUNCTION(BlueprintSetter, BlueprintCallable, Category=HealthPoints)
		void SetHealthPoints(float Hp);

	UFUNCTION(BlueprintGetter, BlueprintPure, Category=HealthPoints)
		float GetMaxHealthPoints() const;
	UFUNCTION(BlueprintSetter, BlueprintCallable, Category=HealthPoints)
		void SetMaxHealthPoints(float MaxHp);

	UFUNCTION(BlueprintPure, Category=HealthPoints)
		bool IsDead() const;

	/*Delegates*/
	UPROPERTY(BlueprintAssignable, Category=HealthPoints, DisplayName = "OnHealthPointsMaxChanged")
		FOnHealthPointsMaxChangedDelegate OnHealthPointsMaxChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category=HealthPoints, DisplayName = "OnPawnDeath")
		FOnPawnDeathDelegate OnPawnDeathDelegate;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=HealthPoints, DisplayName="UseDamageTimeout?")
		bool bUseTakeDamageTimeout;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=HealthPoints, meta=(EditCondition = "bUseTakeDamageTimeout"))
		float TakeDamageTimeout;
protected:
	UPROPERTY(BlueprintGetter=GetMaxHealthPoints, BlueprintSetter=SetMaxHealthPoints, EditAnywhere, Category=HealthPoints, meta=(ClampMin=0, UIMax = 100, AllowPrivateAccess=true))
		float MaxHealthPoints;
	UPROPERTY(BlueprintGetter=GetHealthPoints, BlueprintSetter=SetHealthPoints, Category=HealthPoints, meta=(AllowPrivateAccess=true))
		float HealthPoints;

private:
	FTimerHandle TakeDamageTimeoutHandler;
	bool bIsDead = false;
	
private:
	UFUNCTION()
		void OnTakeAnyDamagePureHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void CheckOnDeath();
};
