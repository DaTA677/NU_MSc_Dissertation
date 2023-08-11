// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectibleItems.generated.h"

class UInteractComponent;
class APuzzleGameCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEGAME_API ACollectibleItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleItems();


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The rendering mesh
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	//Enables input to interact with the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractComponent* InteractComponent;

	//Function to execute when InteractAction is called
	UFUNCTION(BlueprintCallable,Category="Interaction")
	void Collect(APuzzleGameCharacter* Character);



	//Flag to check if the item is collected
	uint8 bIsCollected =false;

	UPROPERTY(EditAnywhere)
	float Lifetime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "UpdateAttributes")
	void SetIsCollected(const uint8& bCollected);

	UFUNCTION(BlueprintCallable, Category = "UpdateAttributes")
	UStaticMeshComponent* GetStaticMeshComponent();

};
