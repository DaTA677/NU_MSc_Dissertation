// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "InteractableSlidePuzzleActor.generated.h"

/**
 * This is the base for creating the slide puzzle
 */
UCLASS()
class PUZZLEGAME_API AInteractableSlidePuzzleActor : public AInteractableActors
{
	GENERATED_BODY()
	
public:
	AInteractableSlidePuzzleActor();

	virtual void OnInteract() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//Visible Components
	/** First person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SlideCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
		class UMaterialInterface* SlideMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tiles, meta = (AllowPrivateAccess = "true"))
		TArray<class ASocketableActors*> SlideTiles;

//End of visible components
	
	//UI element
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Instructions, meta = (AllowPrivateAccess = "true"))
		FString SlidePuzzleInstructions = "Arrow Keys- Select tile\nWASD - Move tile\nQ - Leave";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
		int SelectedTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tiles, meta = (AllowPrivateAccess = "true"))
	class ASocketableActors* CurrentSocket;

//I/O operations
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* SlidePuzzleMappingContext;

	/** Back Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BackAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* InteractAction;
	
	/** Look Input Action used to select the tile */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;


	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for back input */
	void Back(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact();

	/** Called for look input */
	void Look(const FInputActionValue& Value);

	/**Checks if the socket at the supplied id is empty*/
	uint8 CheckIfEmptySocket(int SocketId);
//End of I/O operations


	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateCurrentSocketProperties();

};
