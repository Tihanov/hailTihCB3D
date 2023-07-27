// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemBaseActor.h"

#include "Inventory/InventoryComponent.h"
#include "Log.h"
#include "MainGameState.h"
#include "Action/ActionManagerComponent.h"

AInventoryItemBaseActor::AInventoryItemBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("RootMesh");
	SetRootComponent(RootMeshComponent);
}

void AInventoryItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

	if(InitRowName != "None")
		InitWithRowName(InitRowName, InitCountOf);
}

void AInventoryItemBaseActor::Init(UInventoryItemDefaultInfo* InitItemInfo)
{
	if(!InitItemInfo)
	{
		ULog::Error(TEXT("InitItemInfo == nullptr"), LO_Both);
		return;
	}
	ItemInfo = InitItemInfo;
	
	RootMeshComponent->SetStaticMesh(ItemInfo->Info.Mesh);
	SetActorScale3D( ItemInfo->Info.Other.Scale );
}

void AInventoryItemBaseActor::InitWithRowName(FName RowName, int32 CountOf)
{
	const auto GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
	check(GameState);
	InvDataTable = &GameState->InventoryDataTable;
	
	if (!InvDataTable || RowName == TEXT("None"))
		return;

	auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
	if(!Row)
	{
		ULog::Error("Cannt find row", LO_Both);
		return;
	}
	RootMeshComponent->SetStaticMesh(Row->Mesh);
	SetActorScale3D( Row->Other.Scale );

	ItemInfo = UInventoryItemDefaultInfo::Create(RowName, CountOf, *Row, Row->Other.InfoClass);
	check(ItemInfo);	
}

EActionType AInventoryItemBaseActor::GetActionType_Implementation()
{
	return EActionType::PickUp;	
}

void AInventoryItemBaseActor::DoAction_Implementation(AActor* CausedBy)
{
	check(CausedBy);
	UInventoryComponent* InvComp = Cast<UInventoryComponent>(CausedBy->GetComponentByClass(UInventoryComponent::StaticClass()));
	if(APlayerController* PlayerController = Cast<APlayerController>(CausedBy); !InvComp && PlayerController)
	{
		const auto ControlledPawn = PlayerController->GetPawn();
		check(ControlledPawn);
		InvComp = Cast<UInventoryComponent>(ControlledPawn->GetComponentByClass(UInventoryComponent::StaticClass()));
		check(InvComp);
	}
	else
		ULog::Error(
			FString::Printf(TEXT("method DoAction(ActionInterface, AInventoryItemBaseActor); Line: %i"), __LINE__),
			LO_Console);
	
	InvComp->PickUpItem(this);
	if( this && !this->IsActorBeingDestroyed())
	{
		const auto ActComp = Cast<UActionManagerComponent>(CausedBy->GetComponentByClass(UActionManagerComponent::StaticClass()));
		ActComp->OnRefreshActionDelegate.Broadcast(ActComp, this);
	}
}

FText AInventoryItemBaseActor::GetDisplayDescription_Implementation() const
{
	if(ItemInfo->Count > 1)
		return FText::Format(FText::FromString("{0} x{1}"), ItemInfo->Info.DisplayName, ItemInfo->Count);
	return FText::Format(FText::FromString("{0}"), ItemInfo->Info.DisplayName);
}

UTexture2D* AInventoryItemBaseActor::GetIco_Implementation() const
{
	return ItemInfo->Info.Ico;
}

bool AInventoryItemBaseActor::CanDoAction_Implementation() const
{
	return bCanBePickUpped;	
}
