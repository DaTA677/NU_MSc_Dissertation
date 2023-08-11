// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMoveComponent::EnableMovement(bool bShouldMove)
{
	MoveEnabled = bShouldMove;
	SetComponentTickEnabled(MoveEnabled);
}

void UMoveComponent::ResetMovement()
{
	CurrentDistance = 0.0f;
	SetRelativeLocation(StartRelativeLoc);
}

void UMoveComponent::SetMoveDirection(int bDir)
{
	MoveDirection = bDir > 1 ? 1 : -1;
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	StartRelativeLoc = this->GetRelativeLocation();

	MoveOffsetNorm = MoveOffset;
	MoveOffsetNorm.Normalize();
	MaxDistance = MoveOffset.Length();

	//Check if movement is required
	SetComponentTickEnabled(MoveEnabled);

}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	CurrentDistance += Speed * DeltaTime * MoveDirection;

	if (CurrentDistance >= MaxDistance || CurrentDistance < 0.0f) {
		//Change Direction
		MoveDirection *= -1;
		//Clamp current distance
		CurrentDistance = FMath::Clamp(CurrentDistance, 0.0f, MaxDistance);

		//Send the delegate
		OnEndPointReached.Broadcast(CurrentDistance >= MaxDistance);
	}

	SetRelativeLocation(StartRelativeLoc + CurrentDistance * MoveOffsetNorm);
}

