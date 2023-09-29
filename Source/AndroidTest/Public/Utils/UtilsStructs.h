// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/Object.h"
#include "UtilsStructs.generated.h"

UENUM(BlueprintType)
enum ETeamId
{
	TI_MainPlayer		= 0,
	TI_DefaultNpc		= 1,
	TI_DefaultEnemy		= 2,
	TI_DefaultVillager	= 3,


	TI_MAX				UMETA(Hidden)
};

ETeamAttitude::Type DefaultTeamIdSolver(FGenericTeamId A, FGenericTeamId B);
ETeamAttitude::Type MainTeamIdSolver(FGenericTeamId A, FGenericTeamId B);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANDROIDTEST_API UTeamIdComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ETeamId GetTeamId() const { return TeamId; }
	void SetTeamId(ETeamId InTeamId) { TeamId = InTeamId; }

	bool IsEnemyWith(ETeamId InTeamId) const;
	bool IsFriendWith(ETeamId InTeamId) const;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Options")
		TEnumAsByte<ETeamId> TeamId = TI_DefaultNpc;
};

