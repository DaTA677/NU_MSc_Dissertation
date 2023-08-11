// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"
#include "PuzzleGameCharacter.h"
#include "Components/BoxComponent.h"

APickableActor::APickableActor()
{
	InteractionLayer = EInteractionLayer::PICKUP;
}

void APickableActor::OnInteract()
{
	if (MainCharacter && bNearCharacter) {
		//UE_LOG(LogTemp, Warning, TEXT("Interacted with PickableActor"));
		MainCharacter->AddToInventory(this);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &APickableActor::OnBoxBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &APickableActor::OnBoxEndOverlap);
}

void APickableActor::OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APuzzleGameCharacter* PuzzleCharacter = Cast<APuzzleGameCharacter>(OtherActor)) {
		MainCharacter = PuzzleCharacter;
		bNearCharacter = true;
		SetActorTickEnabled(true);
	}
}

void APickableActor::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter!=nullptr) {
		MainCharacter = nullptr;
		bNearCharacter = false;
		if (HighlightMesh) {
			HighlightMesh->SetVisibility(false);
		}
		SetActorTickEnabled(false);
	}
}

void APickableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Not yet implemented;
}
