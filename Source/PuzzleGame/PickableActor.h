// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "PickableActor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API APickableActor : public AInteractableActors
{
	GENERATED_BODY()
	
public:
	APickableActor();

	virtual void OnInteract() override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
