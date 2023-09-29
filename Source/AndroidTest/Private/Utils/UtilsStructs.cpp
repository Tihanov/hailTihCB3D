// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/UtilsStructs.h"

/*
 * 0 - friend
 * 1 - natural
 * 2 - enemy
 */
int32 TeamAttitudeMap[TI_MAX][TI_MAX] = {
	/*MainPlayer:*/			{0,1,2,1},
	/*DefaultNpc:*/			{1,0,1,1},
	/*DefaultEnemy:*/		{2,1,0,1},
	/*DefaultVillager:*/	{0,0,1,0},
};

ETeamAttitude::Type DefaultTeamIdSolver(FGenericTeamId A, FGenericTeamId B)
{
	return A != B ? ETeamAttitude::Hostile : ETeamAttitude::Friendly;
}

ETeamAttitude::Type MainTeamIdSolver(FGenericTeamId A, FGenericTeamId B)
{
	if(A == FGenericTeamId::NoTeam || B == FGenericTeamId::NoTeam)
		return ETeamAttitude::Hostile;

	return static_cast<ETeamAttitude::Type>(TeamAttitudeMap[A.GetId()][B.GetId()]);
}

bool UTeamIdComponent::IsEnemyWith(ETeamId InTeamId) const
{
	return MainTeamIdSolver(GetTeamId(), InTeamId) == ETeamAttitude::Hostile;
}

bool UTeamIdComponent::IsFriendWith(ETeamId InTeamId) const
{
	return !IsEnemyWith(InTeamId);
}
