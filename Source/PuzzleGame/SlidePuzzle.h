// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TP_PickUpComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlidePuzzle.generated.h"

USTRUCT(BlueprintType)
struct FSocketData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
		int32 SocketIndex;
};

UCLASS()
class PUZZLEGAME_API ASlidePuzzle : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	ASlidePuzzle();
	
	
	//Attributes of puzzle
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SlideFrame;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent*> SlideTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
	TArray<FSocketData> Tiles;

	UPROPERTY(EditAnywhere)
	ACameraActor* camera;

	UTP_PickUpComponent* pickup;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
