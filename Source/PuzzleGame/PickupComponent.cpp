// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PuzzleGameCharacter.h"

void UPickupComponent::PickUp()
{
	if (MainCharacter != nullptr) {
		
		//MainCharacter->AddToInventory(this->GetAttachmentRootActor());
		OnComponentBeginOverlap.RemoveAll(this);
		GetAttachmentRootActor()->Destroy();
	}
}

UPickupComponent::UPickupComponent()
{
	SphereRadius = 32.f;
}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Item added to world"));
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPickupComponent::OnSphereEndOverlap);
}

void UPickupComponent::OnSphereBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuzzleGameCharacter* Character = Cast<APuzzleGameCharacter>(OtherActor);

	if (Character != nullptr) {
		MainCharacter = Character;
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(PickupMappingContext, 1);
			}
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
				EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &UPickupComponent::PickUp);
			}
		}
	}
}

void UPickupComponent::OnSphereEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(PickupMappingContext);
		}
	}
}
