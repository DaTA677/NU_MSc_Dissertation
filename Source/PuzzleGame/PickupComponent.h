// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PickupComponent.generated.h"

class APuzzleGameCharacter;
/**
 * Componenet to allow Actors to be added to characters inventory
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEGAME_API UPickupComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PickupMappingContext;

	/** PickUp Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PickupAction;

	/**Make the character interact with the component*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickUp();

	// Sets default values for this component's properties
	UPickupComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:

	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	APuzzleGameCharacter* MainCharacter;
	
};
