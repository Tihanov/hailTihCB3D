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
	CountOf = 1;
}

void AInventoryItemBaseActor::BeginPlay()
{
	Super::BeginPlay();

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
}

void AInventoryItemBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventoryItemBaseActor::Init(FInventoryItemInitStruct InitStruct)
{
	const auto GameState = Cast<AMainGameState>(GetWorld()->GetGameState());
	check(GameState);
	InvDataTable = &GameState->InventoryDataTable;
	RowName = InitStruct.Name;
	CountOf = InitStruct.CountOf;

	if (!InvDataTable)
		return;

	auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
	if(!Row)
	{
		ULog::Error("Cannt find row", LO_Both);
		return;
	}
	RootMeshComponent->SetStaticMesh(Row->Mesh);
	SetActorScale3D( Row->Other.Scale );
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
	const auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
	return FText::Format(FText::FromString("{0} x{1}"), Row->DisplayName, CountOf);
}

UTexture2D* AInventoryItemBaseActor::GetIco_Implementation() const
{
	const auto Row = (*InvDataTable)->FindRow<FInvItemDataTable>(RowName, "");
	return Row->Ico;
}

bool AInventoryItemBaseActor::CanDoAction_Implementation() const
{
	return bCanBePickUpped;	
}
