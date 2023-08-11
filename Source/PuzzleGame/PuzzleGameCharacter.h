// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PuzzleGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class ACollectibleItems;
class AInteractableActors;

UCLASS(config=Game)
class APuzzleGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	///** Move Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* UseAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PauseAction;

	
	
public:
	APuzzleGameCharacter();

	// Called every frame.. Testing Line trace
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UTrialGameUI* HUD;

	UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		FString PlayerInstructions = "WASD - Move\nMouse - Look\nE - Interact";

protected:
	virtual void BeginPlay();

	void PerformLineTrace();

	/**To test line trace*/
	float time = 0.f;
	uint8 bHitAnything = false;


public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	/**Used to add a pickup object to inventory*/
	void AddToInventory(AInteractableActors* actor);

	/**Remove from inventroy*/
	UFUNCTION(BlueprintCallable, Category = Item)
	void UseItem();
	

	/**Remove from inventroy*/
	UFUNCTION(BlueprintCallable, Category = Item)
	void EmptyInventory();

	/**Update currentItem*/
	void UpdateCurrentItem();

	/**Remove the currentItem from Inventory*/
	void RemoveCurrentItemFromInventory();

	AInteractableActors* GetCurrentItem() const { return CurrentInventoryItem; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<AInteractableActors*> Inventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Items")
	AInteractableActors* CurrentInventoryItem;



protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/**Called to interact with items in the world*/
	void Interact();

	AInteractableActors* CurrentLookItem;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	AInteractableActors* GetCurrentLookItem() const { return CurrentLookItem; }
};

