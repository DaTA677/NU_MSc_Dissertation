// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleItems.h"
#include "InteractComponent.h"
#include "PuzzleGameCharacter.h"



// Sets default values
ACollectibleItems::ACollectibleItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	InteractComponent = CreateDefaultSubobject<UInteractComponent>("Interaction");
	InteractComponent->SetupAttachment(StaticMesh);

	Lifetime = 2.0f;

	bIsCollected = false;

}

// Called when the game starts or when spawned
void ACollectibleItems::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectibleItems::Collect(APuzzleGameCharacter* Character)
{
	if (!bIsCollected) {
		//The item is collected
		bIsCollected = true;

		UE_LOG(LogTemp, Warning, TEXT("Item sent to Inventory"));
		//Adding the item to the characters inventory
		//Character->AddToInventory(this);

		//Initiating end animation
		//Jumping the character
		StaticMesh->AddImpulse({ 0.0f,0.0f,2500.0f });
		
	}
	
}

void ACollectibleItems::SetIsCollected(const uint8& bCollected)
{
	bIsCollected = bCollected;
}

UStaticMeshComponent* ACollectibleItems::GetStaticMeshComponent()
{
	return StaticMesh;
}


// Called every frame
void ACollectibleItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

