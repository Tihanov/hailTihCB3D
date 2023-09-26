// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ChestComponent.h"

#include "Log.h"
#include "Npc/NpcBaseCharacter.h"
#include "Player/MainPlayerController.h"


UChestComponent::UChestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UChestComponent::BeginPlay()
{
	Super::BeginPlay();

	for(const auto& Item : InitInventory)
	{
		if(Item->Info.IsItemUnique)
			AddUniqueItem(Item);
		else 
			AddItem(Item->RowName, Item->Count);
	}
	InitInventory.SetNum(0);

	CreateChestCollision(bCreateChestCollision);
	InitChestName();
	InitChestInterfaceIco();
}

void UChestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(ChestCollision.IsValid())
	{
		ChestCollision->Destroy();
		ChestCollision.Reset();
	}
}

void UChestComponent::CreateChestCollision(bool bInCreateChestCollision)
{
	if(bInCreateChestCollision && ChestCollision.IsNull())
	{
		FActorSpawnParameters SpawnParameters { };
		SpawnParameters.Instigator = GetOwner<APawn>();
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto ChestCollision_ = GetWorld()->SpawnActor<AChestCollision>(SpawnParameters);
		const FAttachmentTransformRules AttachmentTransformRules{
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			false
		};
		
		ChestCollision_->AttachToActor(GetOwner(), AttachmentTransformRules, SocketName);
		ChestCollision = ChestCollision_;

		ChestCollision->InitFromComponent(this);
	}
	else if(!bInCreateChestCollision && ChestCollision.IsValid())
	{
		ChestCollision->Destroy();
		ChestCollision.Reset();
	}
}

void UChestComponent::InitChestName()
{
	if (!ChestName.IsEmpty())
		return;
	
	if(const auto Npc = GetOwner<ANpcBaseCharacter>(); Npc)
		ChestName = FText::FromString(Npc->DlgDisplayName.ToString());
	else		
		ChestName = FText::FromString(GetOwner()->GetName());
}

void UChestComponent::InitChestInterfaceIco()
{
#if WITH_EDITOR
	if(!ChestInterfaceIco)
		ChestInterfaceIco = LoadObject<UTexture2D>(
			nullptr, TEXT("/Game/Texture/Inventory/Dev/DevOpenChest.DevOpenChest"));
#endif
}


AChestCollision::AChestCollision()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
}

void AChestCollision::InitFromComponent(const UChestComponent* InInitFrom)
{
	InitFrom = InInitFrom;
	BoxComponent->InitBoxExtent(InInitFrom->GetChestCollisionExtend());
}

bool AChestCollision::CanDoAction_Implementation() const
{
	return true;
}

void AChestCollision::DoAction_Implementation(AActor* CausedBy)
{
	auto PlayerController = Cast<AMainPlayerController>(CausedBy);
	if(!PlayerController)
		PlayerController = CausedBy->GetOwner<AMainPlayerController>();

	if(!PlayerController)
	{
		ULog::Error("ChestComponent DoAction PlayerController == nullptr", LO_Both);
		return;
	}
	
	PlayerController->OpenChest(this);
}

FText AChestCollision::GetDisplayDescription_Implementation() const
{
	return InitFrom->GetChestName();
}

UTexture2D* AChestCollision::GetIco_Implementation() const
{
	return InitFrom->GetChestInterfaceIco();
}


