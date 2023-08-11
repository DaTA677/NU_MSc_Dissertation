// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "ExaminableActors.generated.h"


class UCameraComponent;
class APuzzleGameCharacter;
/**
 * These actors can be picked up by the player to view them 
 */
UCLASS(ClassGroup = (Custom))
class PUZZLEGAME_API AExaminableActors : public AInteractableActors
{
	GENERATED_BODY()
	
public:

	AExaminableActors();

	virtual void OnInteract() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** First person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ExamineCamera;

	

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ExamineMappingContext;


	/** Back Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BackAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		FString ExaminableCubeInstructions = "Arrow keys - Change cube side\nZ - Rotate cube anti-clockwise\nC - Rotate cube clockwise\nQ - Leave";
	//APuzzleGameCharacter* MainCharacter;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for back input */
	void Back(const FInputActionValue& Value);

	void Interact();

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
