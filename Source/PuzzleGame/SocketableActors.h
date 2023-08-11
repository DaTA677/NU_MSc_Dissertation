// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "SocketableActors.generated.h"

class APuzzleGameCharacter;
/**
 * Class to store pick up items in the world
 */
UCLASS()
class PUZZLEGAME_API ASocketableActors : public AInteractableActors
{
	GENERATED_BODY()
	
public:
	//Sets default values
	ASocketableActors();


	/**Store a Pickup item */
	UFUNCTION(BlueprintCallable,Category="Game")
	uint8 StorePickableItem(AInteractableActors* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Game")
	AInteractableActors* RemovePickableItem();

	virtual void OnInteract() override;

	void Init(int SocketNo);
	uint8 IsSocketed()const { return bSocketed; }
	void SetSocketed(const uint8& InSocketed) { bSocketed = InSocketed; }
protected:
	/**Tracks if item socketed or not*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Game")
	uint8 bSocketed;
	
	/**Stored Item*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Game")
	AInteractableActors* SocketedItem;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**Collision Set for highlight*/
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/**Collision end for highlight*/
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
