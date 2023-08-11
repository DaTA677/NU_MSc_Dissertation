// Fill out your copyright notice in the Description page of Project Settings.


#include "ExaminableActors.h"
#include "PuzzleGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TrialGameUI.h"
#include "Components/EditableTextBox.h"


AExaminableActors::AExaminableActors()
{
	ExamineCamera = CreateDefaultSubobject<UCameraComponent>("Examine Camera");

	ExamineCamera->SetupAttachment(InteractionVolume);

	InteractionLayer = EInteractionLayer::EXAMINE;
}

void AExaminableActors::OnInteract()
{
	Interact();
}

void AExaminableActors::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AExaminableActors::OnBoxBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AExaminableActors::OnBoxEndOverlap);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ExamineMappingContext, 1);
		}
	}
}

void AExaminableActors::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExaminableActors::Move);
		//MoveBack
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Triggered, this, &AExaminableActors::Back);
	}
}

void AExaminableActors::Move(const FInputActionValue& Value)
{
	if(Controller != nullptr) {
		FVector MoveDirection = Value.Get<FVector>();

		FVector DDirection = FVector(0, MoveDirection.Y, 0);
		DDirection.Normalize();
		FQuat Rotation = FQuat(DDirection, 1.57);
		RenderedMesh->AddRelativeRotation(Rotation,true);

		DDirection = FVector(0,0, MoveDirection.X);
		DDirection.Normalize();
		Rotation = FQuat(DDirection, 1.57);
		RenderedMesh->AddRelativeRotation(Rotation, true);

		DDirection = FVector(MoveDirection.Z, 0,0);
		DDirection.Normalize();
		Rotation = FQuat(DDirection, 1.57);
		RenderedMesh->AddRelativeRotation(Rotation, true);

		//UE_LOG(LogTemp, Warning, TEXT("Updated movement:%f,%f"),MoveDirection.X,MoveDirection.Y);
	}
}

void AExaminableActors::Back(const FInputActionValue& Value)
{
	if (Controller != nullptr) {
		if (MainCharacter != nullptr) {
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->RemoveMappingContext(ExamineMappingContext);
				}
				PlayerController->bShowMouseCursor = false;

				InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AExaminableActors::OnBoxBeginOverlap);
				SetActorTickEnabled(true);
				if (MainCharacter->HUD) {
					MainCharacter->HUD->UpdateInstructions(FText::FromString(MainCharacter->PlayerInstructions), true);
					MainCharacter->HUD->LookCursor->SetRenderOpacity(1.0f);
				}
				Controller->Possess(MainCharacter);
				
			}
		}
	}
}

void AExaminableActors::Interact()
{
	if (MainCharacter && bNearCharacter) {
		if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController())) {
			PlayerController->Possess(this);
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(ExamineMappingContext, 2);
			}

			PlayerController->bShowMouseCursor = false;
			InteractionVolume->OnComponentBeginOverlap.Remove(this, FName(TEXT("OnBoxBeginOverlap")));
			SetActorTickEnabled(false);
			HighlightMesh->SetVisibility(false);
			if (MainCharacter->HUD) {
				MainCharacter->HUD->UpdateInstructions(FText::FromString(ExaminableCubeInstructions),true);
				MainCharacter->HUD->LookCursor->SetRenderOpacity(0.0f);
			}
		}
	}
}

void AExaminableActors::OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuzzleGameCharacter* Character = Cast<APuzzleGameCharacter>(OtherActor);

	if (Character != nullptr) {
		MainCharacter = Character;
		bNearCharacter = true;
		SetActorTickEnabled(true);
	}

}

void AExaminableActors::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter != nullptr) {
		bNearCharacter = false;
		MainCharacter = nullptr;
		if (HighlightMesh) {
			HighlightMesh->SetVisibility(false);
		}
	}

}

void AExaminableActors::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	bNearCharacter = false;
	if (MainCharacter == nullptr && Controller==nullptr)
	{

		return;
	}

	/*if (MainCharacter != nullptr) {
		if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController())) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(InteractMappingContext);
			}
		}
	}*/

	if (Controller != nullptr) {
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(ExamineMappingContext);
			}
		}
	}

}
