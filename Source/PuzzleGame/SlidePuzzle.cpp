// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidePuzzle.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values
ASlidePuzzle::ASlidePuzzle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SlideFrame=CreateDefaultSubobject<UStaticMeshComponent>("SlidePuzzle_frame");
	SetRootComponent(SlideFrame);

	FString MeshPath = "/Game/ImportedAssets/SM_Slidepuzzle_frame";
	UStaticMesh* SlideMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh '/../../../../../Assets/SM_Slidepuzzle_frame.fbx'"));*/
	if (SlideMesh) {
		SlideFrame->SetStaticMesh(SlideMesh);

	}

	for (int32 index = 0; index < 16; index++) {
		FSocketData Data;
		Data.SocketName = FName(FString::Printf(TEXT("Socket_%d"), index));
		Data.SocketIndex = index;
		Tiles.Add(Data);
		UStaticMeshSocket* NewSocket = NewObject<UStaticMeshSocket>(SlideFrame, UStaticMeshSocket::StaticClass(), Data.SocketName, RF_NoFlags, nullptr);
		//NewSocket->RelativeLocation = GetActorLocation() + FVector(0.f, 0.f, 10.f);
		SlideFrame->GetStaticMesh()->Sockets.Add(NewSocket);

		UStaticMeshComponent* tile = CreateDefaultSubobject<UStaticMeshComponent>(FName(FString::Printf(TEXT("Tile_%d"), index)));
		FString MeshPath1 = "/Game/ImportedAssets/SM_Slidepuzzle_tile";
		UStaticMesh* TileMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath1);
		float size = 37.f;

		if (TileMesh) {
			tile->SetStaticMesh(TileMesh);
			tile->AddLocalOffset(FVector((size*(index/4)), (size*(index%4)), 0.f));
			tile->SetupAttachment(SlideFrame, Data.SocketName);
		}
		SlideTiles.Add(tile);
	}
}

// Called when the game starts or when spawned
void ASlidePuzzle::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ASlidePuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlidePuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

