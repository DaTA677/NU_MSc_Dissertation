// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MoveComponent.generated.h"

UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveComponentEndPointReachedSignature, bool, IsTopEndpoint);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEGAME_API UMoveComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

	UFUNCTION(BlueprintCallable)
	void EnableMovement(bool bShouldMove);

	UFUNCTION(BlueprintCallable)
	void ResetMovement();

	UFUNCTION(BlueprintCallable)
		void SetMoveDirection(int bDir);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//End location
	UPROPERTY(EditAnywhere)
		FVector MoveOffset;
	
	//speed of movement
	UPROPERTY(EditAnywhere)
		float Speed = 1.0f;

	//Should the component move
	UPROPERTY(EditAnywhere)
	bool MoveEnabled = true;

	UPROPERTY(BlueprintAssignable)
	FOnMoveComponentEndPointReachedSignature OnEndPointReached;

private:

	friend class FMoveComponentVisualizer;

	FVector StartRelativeLoc;
	FVector MoveOffsetNorm;
	


	float CurrentDistance=0.0f;
	float MaxDistance=0.f;
	int8 MoveDirection=1;



};
