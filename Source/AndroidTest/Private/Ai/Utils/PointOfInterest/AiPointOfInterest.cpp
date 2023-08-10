// Fill out your copyright notice in the Description page of Project Settings.

#include "AndroidTest/Public/Ai/Utils/PointOfInterest/AiPointOfInterest.h"
#include "Ai/Villager/VillagerAiController.h"

UAiPointOfInterestInstance::UAiPointOfInterestInstance()
{
	POI_ToSpawnClass = AAiPointOfInterest::StaticClass();
}



void AAiPointOfInterest::Init_Implementation(UAiPointOfInterestInstance* Instance, AVillagerAiController* AiController)
{
	check(Instance);
	InstanceOfSelf = Instance;

	check(AiController);
	VillagerAiController = AiController;
}

bool AAiPointOfInterest::IsComplete_Implementation() const
{
	return true;
}

bool AAiPointOfInterest::IsArrived_Implementation() const
{
	return true;
}

UAiPointOfInterestInstance* AAiPointOfInterest::GetInstance() const
{
	return InstanceOfSelf;
}

AVillagerAiController* AAiPointOfInterest::GetAiController() const
{
	return VillagerAiController;
}

void AAiPointOfInterest::OnArrived()
{
	const auto Pawn = GetAiController()->GetPawn();
	const auto Inst = GetInstance();
	if(IsValid(Inst->OnArrivedAnimation))
	{
		const auto SkeletalMesh
			= Cast<USkeletalMeshComponent>(Pawn->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		SkeletalMesh->GetAnimInstance()->Montage_Play(Inst->OnArrivedAnimation);
	}

	if(Inst->bOnArrivedChangeTransform)
		Pawn->SetActorTransform(Inst->OnArrivedTransform);

	if(Inst->bOnArrivedUseCallback)
		Inst->OnArrivedCallback->Execute(this);
}

void AAiPointOfInterest::OnComplete()
{
	const auto Inst = GetInstance();
	if(IsValid(Inst->OnArrivedAnimation))
	{
		const auto Pawn = GetAiController()->GetPawn();
		const auto SkeletalMesh
			= Cast<USkeletalMeshComponent>(Pawn->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		SkeletalMesh->GetAnimInstance()->Montage_Stop(1.f);
	}

	if(Inst->bOnCompleteUseCallback)
		Inst->OnCompleteCallback->Execute(this);
}





void UPoiCallback::Execute_Implementation(AAiPointOfInterest* PointOfInterest)
{
	// need to be realized in child class
}
