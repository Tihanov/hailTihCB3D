// Fill out your copyright notice in the Description page of Project Settings.

#include "AndroidTest/Public/Ai/Utils/PointOfInterest/AiPointOfInterest.h"

#include "Log.h"
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

	const auto Inst = GetInstance();
	if(Inst->bOnStartUseCallbacks)
		for(const auto& Callback : Inst->OnStartCallbacks)
			Callback->Execute(this);
}

bool AAiPointOfInterest::IsComplete_Implementation() const
{
	return bIsComplete;
}

bool AAiPointOfInterest::IsArrived_Implementation() const
{
	return bIsArrived;
}

EPoiCompleteCauser AAiPointOfInterest::GetCompleteCauser() const
{
	if(!IsComplete())
	{
		ULog::Error("GetCompleteCauser should use only after POI complete", LO_Both);
		return EPoiCompleteCauser::Undefined;
	}
	return CompleteCauser;
}

bool AAiPointOfInterest::IsCompleteByReasons(uint8 Reasons) const
{
	return (CompleteCauser & static_cast<EPoiCompleteCauser>(Reasons)) != EPoiCompleteCauser::NONE;
}

void AAiPointOfInterest::SetCompleteCauser(EPoiCompleteCauser Causer)
{
	if(IsComplete_Implementation())
		return;
	CompleteCauser = Causer;
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
	if(IsArrived_Implementation())
		return;
	bIsArrived = true;
	const auto Pawn = GetAiController()->GetPawn();
	const auto Inst = GetInstance();

	if(Inst->bOnArrivedUseCallbacks)
		for(const auto& Callback : Inst->OnArrivedCallbacks)
			Callback->Execute(this);
	
	if(IsValid(Inst->OnArrivedAnimation))
	{
		const auto SkeletalMesh
			= Cast<USkeletalMeshComponent>(Pawn->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		SkeletalMesh->GetAnimInstance()->Montage_Play(Inst->OnArrivedAnimation);
	}

	if(Inst->bOnArrivedChangeTransform)
		Pawn->SetActorTransform(Inst->OnArrivedTransform);
}

void AAiPointOfInterest::OnComplete()
{
	if(IsComplete_Implementation())
		return;
	bIsComplete = true;
	const auto Pawn = GetAiController()->GetPawn();
	const auto Inst = GetInstance();

	if(Inst->bOnCompleteUseCallbacks)
		for(const auto& Callback : Inst->OnCompleteCallbacks)
			Callback->Execute(this);

	if(Inst->bOnCompleteChangeTransform)
		Pawn->SetActorTransform(Inst->OnCompleteTransform);
	
	if(IsValid(Inst->OnArrivedAnimation))
	{
		const auto SkeletalMesh
			= Cast<USkeletalMeshComponent>(Pawn->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		SkeletalMesh->GetAnimInstance()->Montage_Stop(1.f);
	}
}





void UPoiCallback::Execute_Implementation(AAiPointOfInterest* PointOfInterest)
{
	// need to be realized in child class
}
