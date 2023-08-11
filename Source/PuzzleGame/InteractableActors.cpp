// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActors.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PuzzleGameCharacter.h"

// Sets default values
AInteractableActors::AInteractableActors()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryText = (FString::Printf(TEXT("Null")));

	this->InteractionVolume = CreateDefaultSubobject<UBoxComponent>("Interaction Volume");
	this->HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>("Highlight Mesh");;
	this->RenderedMesh = CreateDefaultSubobject<UStaticMeshComponent>("Rendered Mesh");
	this->RootComponent = InteractionVolume;

	this->RenderedMesh->SetupAttachment(InteractionVolume);
	this->HighlightMesh->SetupAttachment(InteractionVolume);

}

// Called when the game starts or when spawned
void AInteractableActors::BeginPlay()
{
	Super::BeginPlay();	
	SetActorTickEnabled(false);
}

// Called every frame
void AInteractableActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bNearCharacter && MainCharacter &&  HighlightMesh ) {
		if ((MainCharacter->GetCurrentLookItem() == this)) {

			HighlightMesh->SetVisibility(true);
		}
		else if (HighlightMesh->IsVisible()) {
			HighlightMesh->SetVisibility(false);
		}
	}
	

}

// Called to bind functionality to input
void AInteractableActors::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void AInteractableActors::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (APuzzleGameCharacter* PuzzleCharacter = Cast<APuzzleGameCharacter>(OtherActor)) {
//		bNearCharacter = true;
//		MainCharacter = PuzzleCharacter;
//		SetActorTickEnabled(true);
//	}
//}
//
//void AInteractableActors::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (APuzzleGameCharacter* PuzzleCharacter = Cast<APuzzleGameCharacter>(OtherActor)) {
//		if (MainCharacter && HighlightMesh) {
//			HighlightMesh->SetVisibility(false);
//			bNearCharacter = false;
//			MainCharacter = nullptr;
//			SetActorTickEnabled(false);
//		}
//	}
//	
//}

