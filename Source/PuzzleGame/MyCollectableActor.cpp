// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCollectableActor.h"
#include "Components/StaticMeshComponent.h" 
#include "PickupComponent.h" 

// Sets default values
AMyCollectableActor::AMyCollectableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	PickUp = CreateDefaultSubobject<UPickupComponent>(TEXT("PickUp"));
	PickUp->SetupAttachment(StaticMesh);

	IsLaunched = false;
}

void AMyCollectableActor::Jump(float Velocity)
{
	if (!IsLaunched) {
		StaticMesh->AddImpulse({ 0.0f,0.0f,Velocity });

		IsLaunched = true;

		SetActorTickEnabled(true);
	}
}

// Called when the game starts or when spawned
void AMyCollectableActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

// Called every frame
void AMyCollectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

