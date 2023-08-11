// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableSlidePuzzleActor.h"
#include "Camera/CameraComponent.h"
#include "PuzzleGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TrialGameUI.h"
#include "SocketableActors.h"
#include "Components/EditableTextBox.h"
#include "InteractableActors.h"
#include "PickableSlideActor.h"
#include "Kismet/GameplayStatics.h"
#include <random>

AInteractableSlidePuzzleActor::AInteractableSlidePuzzleActor()
{
	SlideCamera = CreateDefaultSubobject<UCameraComponent>("Slide Camera");

	SlideCamera->SetupAttachment(InteractionVolume);

	InteractionLayer = EInteractionLayer::EXAMINE;

	this->SlideTiles.Reserve(16);

	SelectedTile = -1;
	CurrentSocket = nullptr;

}

void AInteractableSlidePuzzleActor::OnInteract()
{
	if (MainCharacter && bNearCharacter) {
		if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController())) {
			PlayerController->Possess(this);
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(SlidePuzzleMappingContext, 2);
				Subsystem->RemoveMappingContext(MainCharacter->GetDefaultMappingContext());
			}
			PlayerController->bShowMouseCursor = false;
			InteractionVolume->OnComponentBeginOverlap.Remove(this, FName(TEXT("OnBoxBeginOverlap")));
			SetActorTickEnabled(false);
			//UE_LOG(LogTemp, Warning, TEXT("ActorTickDisabled"));
			if (this->HighlightMesh) {
				this->HighlightMesh->SetVisibility(false);
			}
			if (CurrentSocket) CurrentSocket->SetHighLightMeshVisibility(false);
			SelectedTile = 0;
			CurrentSocket = SlideTiles[SelectedTile];
			//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
			UpdateCurrentSocketProperties();
			CurrentSocket->SetHighLightMeshVisibility(true);
			if (MainCharacter->HUD) {
				MainCharacter->HUD->UpdateInstructions(FText::FromString(SlidePuzzleInstructions), true);
				MainCharacter->HUD->LookCursor->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void AInteractableSlidePuzzleActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractableSlidePuzzleActor::OnBoxBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AInteractableSlidePuzzleActor::OnBoxEndOverlap);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlidePuzzleMappingContext, 2);
		}
	}

	
	for (int i = 0; i < 16; i++) {
		FName SlideSocketName = FName(FString::Printf(TEXT("Socket_%d"), 15 - i));
		ASocketableActors* SlideSocket = GetWorld()->SpawnActorDeferred<ASocketableActors>(ASocketableActors::StaticClass(), FTransform::Identity, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SlideSocket->Init(i);
		UGameplayStatics::FinishSpawningActor(SlideSocket, GetTransform());
		SlideSocket->AttachToComponent(RenderedMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SlideSocketName);
		this->SlideTiles[i]=SlideSocket;
	}

	for (int i = 0; i < 15; i++)
	{
			APickableSlideActor* SlideTile = GetWorld()->SpawnActorDeferred<APickableSlideActor>(APickableSlideActor::StaticClass(), FTransform::Identity, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			SlideTile->Init(FVector(), i, SlideMaterial);
			UGameplayStatics::FinishSpawningActor(SlideTile, GetTransform());
			SlideTiles[i]->StorePickableItem(SlideTile);
	}

	int lowerBound = 0;
	int upperBound = 15;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution distribution(lowerBound, upperBound);

	for (int i = 0; i < 5; i++) {
		int RandomInteger_1 = distribution(gen);
		int RandomInteger_2 = distribution(gen);
		if (RandomInteger_1 == RandomInteger_2) continue;

		AInteractableActors* tile = SlideTiles[RandomInteger_1]->RemovePickableItem();
		SlideTiles[RandomInteger_1]->StorePickableItem(SlideTiles[RandomInteger_2]->RemovePickableItem());
		SlideTiles[RandomInteger_2]->StorePickableItem(tile);

	}
	/*for (int i = 0; i < 15; i++) {
		ASocketableActors* Socket = Cast<ASocketableActors>(SlideTiles[i]->GetChildActor());
		Socket->SetInteractionLayer(EInteractionLayer::SLIDEPUZZLE);
		APickableSlideActor* SlideTile = GetWorld()->SpawnActorDeferred<APickableSlideActor>(APickableSlideActor::StaticClass(), FTransform::Identity, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SlideTile->Init(FVector(), i, FString());
		UGameplayStatics::FinishSpawningActor(SlideTile, GetTransform());
		Socket->StorePickableItem(SlideTile);
		}

	}*/
}

void AInteractableSlidePuzzleActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInteractableSlidePuzzleActor::Look);
		//MoveBack
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Triggered, this, &AInteractableSlidePuzzleActor::Back);
		//Select
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInteractableSlidePuzzleActor::Move);
		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AInteractableSlidePuzzleActor::Interact);
	}
}

void AInteractableSlidePuzzleActor::Move(const FInputActionValue& Value)
{
	if (Controller) {
		FVector2D MoveComponent = Value.Get<FVector2D>();
		if (CurrentSocket && SelectedTile != -1 && CurrentSocket->IsSocketed()) {	
			if (MoveComponent.X > 0 && (SelectedTile % 4 != 3) && CheckIfEmptySocket(SelectedTile+1)) {
				SelectedTile++;
				CurrentSocket->SetHighLightMeshVisibility(false);
				AInteractableActors* TileToMove= CurrentSocket->RemovePickableItem();
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->StorePickableItem(TileToMove);
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
			if ((MoveComponent.X < 0) && (SelectedTile % 4 != 0) && CheckIfEmptySocket(SelectedTile - 1)) {
				SelectedTile--;
				CurrentSocket->SetHighLightMeshVisibility(false);
				AInteractableActors* TileToMove = CurrentSocket->RemovePickableItem();
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->StorePickableItem(TileToMove);
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
			if (MoveComponent.Y > 0 && SelectedTile / 4 != 0 && CheckIfEmptySocket(SelectedTile - 4)) {
				SelectedTile -= 4;
				CurrentSocket->SetHighLightMeshVisibility(false);
				AInteractableActors* TileToMove = CurrentSocket->RemovePickableItem();
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->StorePickableItem(TileToMove);
				CurrentSocket->SetHighLightMeshVisibility(true);

			}
			if (MoveComponent.Y < 0 && ((SelectedTile) / 4 != 3) && CheckIfEmptySocket(SelectedTile + 4)) {
				SelectedTile += 4;
				CurrentSocket->SetHighLightMeshVisibility(false);
				AInteractableActors* TileToMove = CurrentSocket->RemovePickableItem();
				CurrentSocket = SlideTiles[SelectedTile];
				UpdateCurrentSocketProperties();
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				CurrentSocket->StorePickableItem(TileToMove);
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
		}
	}
}

void AInteractableSlidePuzzleActor::Back(const FInputActionValue& Value)
{
	if (Controller != nullptr && MainCharacter != nullptr) {
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(SlidePuzzleMappingContext);
				Subsystem->AddMappingContext(MainCharacter->GetDefaultMappingContext(),0);
			}
			PlayerController->bShowMouseCursor = false;
			InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AInteractableSlidePuzzleActor::OnBoxBeginOverlap);
			if (HighlightMesh) {
				HighlightMesh->SetCollisionEnabled(RenderedMesh->GetCollisionEnabled());
			}
			SetActorTickEnabled(true);
			if (CurrentSocket) { 
				CurrentSocket->SetHighLightMeshVisibility(false); 
			}
			CurrentSocket = nullptr;
			SelectedTile = -1;
			if (MainCharacter->HUD) {
				MainCharacter->HUD->UpdateInstructions(FText::FromString(MainCharacter->PlayerInstructions), true);
				MainCharacter->HUD->LookCursor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			Controller->Possess(MainCharacter);
		}
	}

}

void AInteractableSlidePuzzleActor::Interact()
{
	if (Controller != nullptr && this->MainCharacter != nullptr && SelectedTile!=-1 && CurrentSocket) {
		if (CurrentSocket->IsSocketed()) {
			AInteractableActors* Tile = CurrentSocket->RemovePickableItem();
			if (Tile != nullptr) {
				this->MainCharacter->AddToInventory(Tile);
				Tile->SetActorHiddenInGame(true);
			}
			//SetActorEnableCollision(false);
		}
		else {
			if (CurrentSocket->StorePickableItem(MainCharacter->GetCurrentItem())) {
				MainCharacter->RemoveCurrentItemFromInventory();
			}
		}
	}
}

void AInteractableSlidePuzzleActor::Look(const FInputActionValue& Value)
{
	if (Controller) {
		FVector2D MoveComponent = Value.Get<FVector2D>();
		if (CurrentSocket && SelectedTile!=-1) {
			if (MoveComponent.X > 0 && (SelectedTile % 4 != 3)) {				
				SelectedTile++;
				CurrentSocket->SetHighLightMeshVisibility(false);
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
			if ((MoveComponent.X < 0) && (SelectedTile % 4 != 0)) {
				SelectedTile--;
				CurrentSocket->SetHighLightMeshVisibility(false);
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
			if (MoveComponent.Y > 0 && SelectedTile /4 != 0) {
				SelectedTile -= 4;
				CurrentSocket->SetHighLightMeshVisibility(false);
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->SetHighLightMeshVisibility(true);
				
			}
			if (MoveComponent.Y < 0 && ((SelectedTile) / 4 != 3)) {
				SelectedTile += 4;
				CurrentSocket->SetHighLightMeshVisibility(false);
				CurrentSocket = SlideTiles[SelectedTile];
				//CurrentSocket = Cast<ASocketableActors>(SlideTiles[SelectedTile]->GetChildActor());
				UpdateCurrentSocketProperties();
				CurrentSocket->SetHighLightMeshVisibility(true);
			}
		}
	}
}

uint8 AInteractableSlidePuzzleActor::CheckIfEmptySocket(int SocketId)
{
	ASocketableActors* SocketToCheck = SlideTiles[SocketId];
	//ASocketableActors* SocketToCheck = Cast<ASocketableActors>(SlideTiles[SocketId]->GetChildActor());
	if (SocketToCheck && !SocketToCheck->IsSocketed()) {
		return true;
	}
	return false;
}

void AInteractableSlidePuzzleActor::OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuzzleGameCharacter* Character = Cast<APuzzleGameCharacter>(OtherActor);

	if (Character != nullptr) {
		MainCharacter = Character;
		bNearCharacter = true;
		SetActorTickEnabled(true);
	}
}

void AInteractableSlidePuzzleActor::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter != nullptr) {
		bNearCharacter = false;
		MainCharacter = nullptr;
		if (HighlightMesh) {
			HighlightMesh->SetVisibility(false);
		}
	}
}

void AInteractableSlidePuzzleActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bNearCharacter = false;
	if (MainCharacter == nullptr && Controller == nullptr)
	{
		return;
	}

	if (Controller != nullptr) {
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(SlidePuzzleMappingContext);
			}
		}
	}
}

void AInteractableSlidePuzzleActor::UpdateCurrentSocketProperties()
{
	if (CurrentSocket) {
		CurrentSocket->SetNearCharacter(true);
		CurrentSocket->SetMainCharacter(MainCharacter);
	}
	
}
