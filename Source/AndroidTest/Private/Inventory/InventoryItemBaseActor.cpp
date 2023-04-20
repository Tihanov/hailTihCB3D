// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemBaseActor.h"

#include "Inventory/InventoryComponent.h"
#include "Log.h"

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

	if (!InvDataTable || RowName == TEXT("None"))
		return;

	auto Row = InvDataTable->FindRow<FInvItemDataTable>(RowName, "");
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

FName AInventoryItemBaseActor::PickUp(APlayerController* PlayerController)
{
	ULog::Info("Picked", LO_Viewport);
	return RowName;
}

void AInventoryItemBaseActor::Init(FInventoryItemInitStruct InitStruct)
{
	InvDataTable = InitStruct.DataTable;
	RowName = InitStruct.Name;
	CountOf = InitStruct.CountOf;

	if (!InvDataTable)
		return;

	auto Row = InvDataTable->FindRow<FInvItemDataTable>(RowName, "");
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

