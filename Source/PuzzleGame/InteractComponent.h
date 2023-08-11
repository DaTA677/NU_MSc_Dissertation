// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractComponent.generated.h"

class APuzzleGameCharacter;

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteract, APuzzleGameCharacter*, PickUpCharacter, AActor*, CurrentActor);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEGAME_API UInteractComponent : public USphereComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteract OnInteract;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InteractMappingContext;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/**Make the character interact with the component*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	

	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	APuzzleGameCharacter* GetMainCharacter() { return MainCharacter; }

	void SetNearCharacter(uint8 InbNearCharacter) { bNearCharacter = InbNearCharacter; }
private:

	uint8 bNearCharacter = false;
	APuzzleGameCharacter* MainCharacter;
};
