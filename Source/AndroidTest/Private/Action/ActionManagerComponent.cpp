// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ActionManagerComponent.h"

#include "Log.h"
#include "Action/ActionInterface.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetStringLibrary.h"


UActionManagerComponent::UActionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UActionManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CastedOwner = GetOwner();
	if(const auto PlayerControllerOwner = Cast<APlayerController>(CastedOwner);
		PlayerControllerOwner != nullptr)
		CastedOwner = PlayerControllerOwner->GetPawn();
	check(CastedOwner);
	GetWorld()->GetTimerManager().SetTimer(
		ScanTimerHandle, this, &UActionManagerComponent::Scan, ScanRate, true);
}

void UActionManagerComponent::Scan()
{
	TArray<FHitResult> Out;

	const FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(ScanCapsuleRadius, ScanCapsuleHeight);
	
	GetWorld()->SweepMultiByChannel(
		Out,
		CastedOwner->GetActorLocation(),
		CastedOwner->GetActorForwardVector() * EndMul + CastedOwner->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		CapsuleShape);

	decltype(ScannedActors) ScannedActorsTemp;
	for (auto& Result : Out)
	{
		const auto ToAddActor = Result.GetActor();
		if(const auto Casted = Cast<IActionInterface>(ToAddActor); Casted != nullptr)
		{
			bool bExists = false;
			ScannedActors.Add(ToAddActor, &bExists);
			ScannedActorsTemp.Add(ToAddActor);
			if(!bExists)
				OnAddActionDelegate.Broadcast(this, Casted);
		}
	}

	auto Dif = ScannedActors.Difference(ScannedActorsTemp);
	bool bIfHaveDif = false;
	for (const auto& ItElement : Dif)
	{
		bIfHaveDif = true;
		OnRemoveActionDelegate.Broadcast(this, Cast<IActionInterface>(ItElement));
	}

	if(bIfHaveDif)
		ScannedActors = MoveTemp(ScannedActorsTemp);
}


