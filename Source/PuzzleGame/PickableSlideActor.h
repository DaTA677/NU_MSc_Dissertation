// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "PickableSlideActor.generated.h"


/**
 * An PickableSlide that can be picked up by the player and used in the Slide Puzzle
 */
UCLASS()
class PUZZLEGAME_API APickableSlideActor : public AInteractableActors
{
	GENERATED_BODY()
	

public:
	APickableSlideActor();

	void Init( const FVector InLocation,int InMesh, class UMaterialInterface* InMaterial);

	virtual void OnInteract() override;

private:
	int16 SlideNo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickableSlide, meta = (AllowPrivateAccess = "true"))
	TArray<FString> MeshArray = {
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_new")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_2")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_3")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_4")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_5")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_6")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_7")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_8")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_9")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_10")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_11")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_12")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_13")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_14")),
	FString(TEXT("/Game/ImportedAssets/SM_Slidepuzzle_tile_15")),
	};
};
