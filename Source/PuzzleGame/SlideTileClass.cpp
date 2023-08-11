// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideTileClass.h"
#include "Components/StaticMeshComponent.h"

uint16 ASlideTileClass::TilesCreated = 0;
// Sets default values
ASlideTileClass::ASlideTileClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StaticMesh;

	FString MeshPath= "/Game/ImportedAssets/SM_Slidepuzzle_tile_new";
	UStaticMesh * TileMesh= LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (TileMesh) {
		StaticMesh->SetStaticMesh(TileMesh);
	}


	TileNo = TilesCreated;
	TilesCreated++;

	//UE_LOG(LogTemp, Warning, TEXT("TileNo:: %d"), TileNo);

}

// Called when the game starts or when spawned
void ASlideTileClass::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
}

// Called every frame
void ASlideTileClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

