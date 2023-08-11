// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "KeypadKeyActor.h"
#include <InputAction.h>
#include "KeypadActor.generated.h"


class UCameraComponent;



/**
 * An actor for Keypad in the world
 */
UCLASS()
class PUZZLEGAME_API AKeypadActor : public AInteractableActors
{
	GENERATED_BODY()
	
public:
	AKeypadActor();

	//AInteractableActors 
	virtual void OnInteract() override;
	//End of AInteractableActors

protected:
//Attributes for keypad operation
	/**Keypad number to communicate which door to open*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=Keypad)
	int KeypadNo;

	/**Keypad number to communicate which door to open*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad)
	int TimesTried;

	/**Boolean to check if the correct key has been entered*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad)
	bool bActivated;

	/**Delegate to broadast that activate has been changed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trigger)
	class UTriggerComponent* DoorTrigger;

	/**Array to store entered number*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		TArray<int> EnteredKeys;

	/**Actual code required*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		int RequiredCode;


//End of Attributes for keypad operation

//Visible components of Keypad
	/** First person camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* KeypadCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		TArray<UChildActorComponent*> Keys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		TArray <class UTextRenderComponent* > KeyTexts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* DisplayTextComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FString KeypadInstructions = "Arrow Keys - Select Keys\nE - Press key\nQ - Leave";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keypad, meta = (AllowPrivateAccess = "true"))
		TArray<FString> StoryText;

	int StoryTextCounter=0;
//End of visible components of Keypad
	
// I/O operations 
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* KeypadMappingContext;

	/** Back Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* BackAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;
	
	/** Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SelectAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* EnterAction;

	/** Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* DeleteAction;


//End of I/O operations

	/**Delegates the Key pressed to the corresponding function*/
	void PerformKeyOperation(EKeysPresent InKey);

	/**Function called when a number button is pressed*/
	void AddNumber(int Number);

	/**Function called when back button is pressed*/
	void RemoveNumber();

	/**Function called when enter button is pressed*/
	void CheckNumber();

	/**Updates Display text*/
	void UpdateDisplayText();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	// End of APawn interface

	/** Called for movement input */
	void Interact();

	/** Called for back input */
	void Back(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Select(const FInputActionValue& Value);

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	private:
		AKeypadKeyActor* CurrentLookKey;
};