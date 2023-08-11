// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableSlideActor.h"
#include "Components/BoxComponent.h"



APickableSlideActor::APickableSlideActor()
{
	InteractionVolume->InitBoxExtent(FVector(1.f,1.f,1.f));
	InteractionLayer = EInteractionLayer::SLIDEPUZZLE;

	//UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *(MeshMap[1]));
	//if (Mesh) {
	//	RenderedMesh->SetStaticMesh(Mesh);
	//	//UMaterialInterface* material=RenderedMesh->GetStaticMesh()->GetMaterial(0);
	//	FString MaterialPath = "/Game/Materials/TryoutMaterial";
	//	//UMaterialInstanceDynamic*DynamicMaterial = UMaterialInstanceDynamic::Create(RenderedMesh->GetMaterial(0),this)
	//	UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
	//	if (NewMaterial) {
	//		RenderedMesh->SetMaterial(0, NewMaterial);
	//	}
	
	//}

}

void APickableSlideActor::Init(const FVector InLocation,int InMesh, UMaterialInterface* InMaterial)
{
	
	//if (InMesh < MeshMap.Num()) {
		FString MeshString = MeshArray[InMesh];
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshString);
		if (Mesh) {
			RenderedMesh->SetStaticMesh(Mesh);
			//UMaterialInterface* material=RenderedMesh->GetStaticMesh()->GetMaterial(0);
			FString MaterialPath;
			if (!InMaterial) {
				MaterialPath = "/Game/Materials/TryoutMaterial";
				UMaterialInterface* NewMaterial = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
				if (NewMaterial) {
					RenderedMesh->SetMaterial(0, NewMaterial);
				}
			}
			else { 
				RenderedMesh->SetMaterial(0, InMaterial); 
			}
			
			
		}
		InventoryText = FString::Printf(TEXT("Slide Tile:%d"), InMesh + 1);
	//}
	
}

void APickableSlideActor::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("To be implemented: PickableSlideActor"));
}
