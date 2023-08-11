// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "PuzzleGameCharacter.h"
#include "GameFrameWork/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void UInteractComponent::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted"));
	if (MainCharacter != nullptr && bNearCharacter) {
		//OnComponentBeginOverlap.RemoveAll(this);
		OnInteract.Broadcast(MainCharacter,this->GetAttachmentRootActor());
	}
}

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	SphereRadius = 32.f;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnSphereEndOverlap);
}



void UInteractComponent::OnSphereBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuzzleGameCharacter* Character = Cast<APuzzleGameCharacter>(OtherActor);

	if (Character != nullptr) {
		MainCharacter = Character;
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(InteractMappingContext, 1);
			}
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UInteractComponent::Interact);
			}
		}
		bNearCharacter = true;
	}
}

void UInteractComponent::OnSphereEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (MainCharacter != nullptr) {
		if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(InteractMappingContext);
			}
		}
		bNearCharacter = false;
		MainCharacter = nullptr;
	}
	
	
	//MainCharacter = nullptr;
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bNearCharacter = false;
	if (MainCharacter == nullptr)
	{
		
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(InteractMappingContext);
		}
	}
}