// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCollectableActor.generated.h"

class UPickupComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUZZLEGAME_API AMyCollectableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCollectableActor();

	UFUNCTION(BlueprintCallable)
	void Jump(float bVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Static mesh for rendering
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UPickupComponent* PickUp;

	UPROPERTY(EditAnywhere)
	uint8 IsLaunched;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
