// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleGameCharacter.h"
#include "PuzzleGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CollectibleItems.h"
#include "InteractableActors.h"
#include "TrialGameUI.h"


//////////////////////////////////////////////////////////////////////////
// APuzzleGameCharacter

APuzzleGameCharacter::APuzzleGameCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void APuzzleGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformLineTrace();
	
}

void APuzzleGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	//Testing Line Trace
	this->SetActorTickEnabled(true);
}

void APuzzleGameCharacter::PerformLineTrace()
{
	FHitResult HitResult;
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForwardDirection = FirstPersonCameraComponent->GetForwardVector();
	FVector EndLocation = StartLocation+ CameraForwardDirection* 250.f;
	FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
	

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility,TraceParams);

	if (HitResult.bBlockingHit) {
		if (AInteractableActors* HitInteractableActor = Cast<AInteractableActors>(HitResult.GetActor())) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit an interactable actor"));
			CurrentLookItem = HitInteractableActor;
			bHitAnything = true;
		}
		else {
			CurrentLookItem = nullptr;
			bHitAnything = false;
		}
	}
	else {
		CurrentLookItem = nullptr;
		bHitAnything = false;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void APuzzleGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Look);

		////Use Item
		//EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::UseItem);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::Interact);
	}
}


void APuzzleGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APuzzleGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APuzzleGameCharacter::Interact()
{
	if (CurrentLookItem && CurrentLookItem->IsNearCharacter()) {
		FString Name = CurrentLookItem->GetClass()->GetClassPathName().GetAssetName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Interacted with:%s"), *Name);
		CurrentLookItem->OnInteract();
	}
}

void APuzzleGameCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool APuzzleGameCharacter::GetHasRifle()
{
	return bHasRifle;
}

void APuzzleGameCharacter::AddToInventory(AInteractableActors* actor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Item To be added."));
	if (actor != nullptr) {
		Inventory.Add(actor);
		FString Name=GetDebugName(actor);
		//UE_LOG(LogTemp, Warning, TEXT("Item Added:%s"),*(Name));
		if (CurrentInventoryItem == nullptr) {
			CurrentInventoryItem = actor;
			HUD->UpdateInventoryText(FText::FromString(CurrentInventoryItem->GetInventoryText()),true);
			HUD->SetShowInventory(true);
			//UE_LOG(LogTemp, Warning, TEXT("Current Item Updated."));
		}
	}
}

void APuzzleGameCharacter::UseItem()
{
	if (CurrentInventoryItem != nullptr && Inventory.Num()>0) {
		
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(150.0f, 0.0f, 50.0f));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			CurrentInventoryItem->SetActorLocation(SpawnLocation);
			CurrentInventoryItem->SetActorHiddenInGame(false);
			CurrentInventoryItem->SetActorEnableCollision(true);
			//CurrentInventoryItem;
		}
		RemoveCurrentItemFromInventory();
		
	}
}

void APuzzleGameCharacter::EmptyInventory()
{
	while (Inventory.Num() > 0) {
		Inventory.RemoveAt(0);
	}
	UpdateCurrentItem();
}

void APuzzleGameCharacter::UpdateCurrentItem()
{
	if (Inventory.Num() > 0) {
		CurrentInventoryItem = Inventory[0];
		HUD->UpdateInventoryText(FText::FromString(CurrentInventoryItem->GetInventoryText()), true);
	}
	else {
		CurrentInventoryItem = nullptr;
		Inventory.Empty();
		HUD->SetShowInventory(false);
	}
}

void APuzzleGameCharacter::RemoveCurrentItemFromInventory()
{
	Inventory.Remove(CurrentInventoryItem);
	UpdateCurrentItem();
}
