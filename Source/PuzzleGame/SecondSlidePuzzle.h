// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlidePuzzle.h"
#include "CollectibleItems.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SecondSlidePuzzle.generated.h"

class UChildActorComponent;
class APuzzleGameCharacter;

UCLASS()
class PUZZLEGAME_API ASecondSlidePuzzle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASecondSlidePuzzle();


	//Attributes of puzzle
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SlideFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Sockets")
		TArray<UChildActorComponent*> SlideTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sockets")
		TArray<FSocketData> Tiles;

	UPROPERTY(EditAnywhere)
		UCameraComponent* SlidePuzzleCameraComponent;

	UPROPERTY(EditAnywhere)
		UTexture2D* TextureToBeUsed;

	UPROPERTY(EditAnywhere)
		UInteractComponent* SlidePuzzleInteractComponent;

	UFUNCTION(BlueprintCallable)
		void OnInteract(APuzzleGameCharacter* Character, AActor* Actor);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* SlidePuzzletMappingContext;

	/** Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SelectAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** MoveBack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BackAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;
	
	/** Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;
	
	/** UseItem Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* UseItemAction;



private:
	int16 SelectedTile;
	uint16 TileSize;
	int16 TilesPerRow;
	int16 TotalTiles;
	uint16 TileHeight =16;

	APuzzleGameCharacter* Character;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);
	void Back(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void UseItem(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


	void SetupPuzzle();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UCameraComponent* GetSlidePuzzleCameraComponent() { return SlidePuzzleCameraComponent; }
		
};
