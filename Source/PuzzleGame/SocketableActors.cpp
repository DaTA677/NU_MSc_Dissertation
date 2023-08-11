// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketableActors.h"
#include "PuzzleGameCharacter.h"
#include "Components/BoxComponent.h"

ASocketableActors::ASocketableActors()
{
	InteractionLayer = EInteractionLayer::SLOT;

	bSocketed = false;

}

void ASocketableActors::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ASocketableActors::OnBoxBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ASocketableActors::OnBoxEndOverlap);
}

void ASocketableActors::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Nothing so far;
}

void ASocketableActors::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APuzzleGameCharacter* PuzzleCharacter = Cast<APuzzleGameCharacter>(OtherActor)) {
		//RenderedMesh->SetVisibility(true);
		bNearCharacter = true;
		MainCharacter = PuzzleCharacter;
		SetActorTickEnabled(true);
	}
}

void ASocketableActors::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter) {
		//RenderedMesh->SetVisibility(false);
		bNearCharacter = false;
		MainCharacter = nullptr;
		SetActorTickEnabled(false);
		if (HighlightMesh) {
			HighlightMesh->SetVisibility(false);
		}
	}
}

uint8 ASocketableActors::StorePickableItem(AInteractableActors* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Called:StorePickupbleItem"));
	if (!OtherActor || bSocketed) {
		return false;
	}

	EInteractionLayer Other = OtherActor->GetIneteractionLayer();
	UE_LOG(LogTemp, Warning, TEXT("Interaction Layers: %d,%d"),int(this->InteractionLayer),int(Other));

	if (!DoInteractionLayersOverlap(this->InteractionLayer, Other)) {
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Item can be socketed"));
	OtherActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	OtherActor->SetActorHiddenInGame(false);
	OtherActor->SetActorEnableCollision(true);
	SocketedItem = OtherActor;
	bSocketed = true;
	return true;
}

AInteractableActors* ASocketableActors::RemovePickableItem()
{
	if(!bSocketed)
		return nullptr;

	AInteractableActors* resultActor = SocketedItem;
	//SocketedItem->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	SocketedItem = nullptr;
	bSocketed = false;
	return resultActor;
}

void ASocketableActors::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("In SocketableActors.OnInteract()"));
	if (StorePickableItem(MainCharacter->GetCurrentItem())) {
		MainCharacter->RemoveCurrentItemFromInventory();
	}
}

void ASocketableActors::Init(int SocketNo)
{
	FString MeshPath = "/Engine/EngineMeshes/Cube";
	UStaticMesh* RenderedStaticMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	UStaticMesh* HighlightStaticMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);

	if (RenderedStaticMesh && HighlightStaticMesh) {
		HighlightMesh->SetStaticMesh(HighlightStaticMesh);
		HighlightMesh->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.075f));
		HighlightMesh->SetVisibility(false);
		
		RenderedMesh->SetStaticMesh(RenderedStaticMesh);
		RenderedMesh->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));

		FString RenderedMaterialPath = "/Game/Materials/SocketLocationMaterial";
		UMaterialInterface* RenderedNewMaterial = LoadObject < UMaterialInterface>(nullptr, *RenderedMaterialPath);
		if (RenderedNewMaterial) {
			RenderedMesh->SetMaterial(0, RenderedNewMaterial);
		}

		FString HighlightMaterialPath = "/Game/Materials/TransparentMaterial"; 
		UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, *HighlightMaterialPath);
		if (NewMaterial) {
			HighlightMesh->SetMaterial(0, NewMaterial);
		}

	}

	InventoryText = FString::Printf(TEXT("Slide Socket:%d"), SocketNo);
	InteractionLayer = EInteractionLayer::SLIDEPUZZLE;
}
