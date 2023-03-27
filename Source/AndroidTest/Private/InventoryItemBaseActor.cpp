// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemBaseActor.h"

#include "InventoryComponent.h"
#include "Log.h"

AInventoryItemBaseActor::AInventoryItemBaseActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("RootMesh");
	SetRootComponent(RootMeshComponent);
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

void AInventoryItemBaseActor::Init(FName Name, UDataTable* DataTable)
{
	InvDataTable = DataTable;
	RowName = Name;

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

