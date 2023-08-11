// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondSlidePuzzle.h"
#include "Engine/StaticMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "CollectibleItems.h"
#include "SlideTileClass.h"
#include "Components/ChildActorComponent.h"
#include "InteractComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASecondSlidePuzzle::ASecondSlidePuzzle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SlideFrame = CreateDefaultSubobject<UStaticMeshComponent>("SlidePuzzle_frame");
	SetRootComponent(SlideFrame);

	FString MeshPath = "/Game/ImportedAssets/SM_Slidepuzzle_frame";
	UStaticMesh* SlideMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);

	if (SlideMesh) {
		SlideFrame->SetStaticMesh(SlideMesh);

	}
	TileSize = 40;
	TilesPerRow = 4;
	TotalTiles = TilesPerRow * TilesPerRow;

	SlidePuzzleCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SlidePuzzleCamera"));
	SlidePuzzleCameraComponent->SetupAttachment(SlideFrame);
	SlidePuzzleCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 140.0f)); // Position the camera
	SlidePuzzleCameraComponent->bUsePawnControlRotation = false;
	SlidePuzzleCameraComponent->SetRelativeRotation(FQuat(FRotator(-90.0, 180.0, 90.0)));


	FString TexPath = "/Game/Materials/Grid";
	TextureToBeUsed= LoadObject<UTexture2D>(nullptr, *TexPath);

	SlidePuzzleInteractComponent = CreateDefaultSubobject<UInteractComponent>("PuzzleInteractComponent");
	SlidePuzzleInteractComponent->SetSphereRadius(150);
	SlidePuzzleInteractComponent->SetupAttachment(SlideFrame);
	SlidePuzzleInteractComponent->OnInteract.AddDynamic(this, &ASecondSlidePuzzle::OnInteract);
	//SlidePuzzleInteractComponent->OnInteract

	SelectedTile = 0;

	SetupPuzzle();
	
}

void ASecondSlidePuzzle::OnInteract(APuzzleGameCharacter* InCharacter, AActor* InActor)
{

	if (this== Cast<ASecondSlidePuzzle>(InActor)) {
		if (APlayerController* PlayerController = Cast<APlayerController>(InCharacter->GetController())) {
			
			PlayerController->Possess(this);
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(SlidePuzzletMappingContext, 1);
			}

			PlayerController->bShowMouseCursor = true;
		}
	}
	this->Character = InCharacter;
	SlidePuzzleInteractComponent->OnInteract.RemoveAll(this);
}

// Called when the game starts or when spawned
void ASecondSlidePuzzle::BeginPlay()
{
	Super::BeginPlay();
	

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlidePuzzletMappingContext, 1);
		}
	}

	UE_LOG(LogTemp,Warning,TEXT("Path to mesh:%s"),*(SlideFrame->GetStaticMesh()->GetPathName()));
	
	/*for (int16 i = 0; i < 5; i++) {
		int16 num1 = rand() % 16;
		int16 num2 = rand() % 16;
		if (num1 == num2) {
			UE_LOG(LogTemp, Warning, TEXT("No swap"));
			continue;
		}
		SlideTiles[num1]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		SlideTiles[num2]->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		UE_LOG(LogTemp, Warning, TEXT("Swap occurred between %d and %d"), num1, num2);

		SlideTiles[num1]->AttachToComponent(SlideFrame, FAttachmentTransformRules::SnapToTargetIncludingScale, Tiles[num2].SocketName);
		SlideTiles[num2]->AttachToComponent(SlideFrame, FAttachmentTransformRules::SnapToTargetIncludingScale, Tiles[num1].SocketName);
		
	}*/
	
}

void ASecondSlidePuzzle::Move(const FInputActionValue& Value)
{
	if (SelectedTile >= 0 && SelectedTile < TotalTiles && SlideTiles[SelectedTile]) {
		FVector2D MoveDirection = Value.Get<FVector2D>();

		if (Controller != nullptr) {
			if (MoveDirection.X > 0) {
				if (SelectedTile % TilesPerRow < TilesPerRow - 1 && !SlideTiles[SelectedTile + 1]->GetChildActorClass()) {
					SlideTiles[SelectedTile]->AddLocalOffset(FVector(TileSize,0.0f, 0.f));
					SlideTiles[SelectedTile + 1]->AddLocalOffset(FVector(-TileSize, 0.0f, 0.0f));
					UChildActorComponent* temp = SlideTiles[SelectedTile + 1];
					SlideTiles[SelectedTile + 1] = SlideTiles[SelectedTile];
					SlideTiles[SelectedTile] = temp;
					SelectedTile++;
				}
			}
			if (MoveDirection.X < 0) {
				if (SelectedTile % TilesPerRow > 0 && !SlideTiles[SelectedTile - 1]->GetChildActorClass()) {
					SlideTiles[SelectedTile]->AddLocalOffset(FVector( -TileSize, 0.f,  0.f));
					SlideTiles[SelectedTile - 1]->AddLocalOffset(FVector( TileSize, 0.f,  0.f));
					UChildActorComponent* temp = SlideTiles[SelectedTile - 1];
					SlideTiles[SelectedTile - 1] = SlideTiles[SelectedTile];
					SlideTiles[SelectedTile] = temp;
					SelectedTile--;
				}
			}
			if (MoveDirection.Y > 0) {
				if (SelectedTile / TilesPerRow > 0 && !SlideTiles[SelectedTile - TilesPerRow]->GetChildActorClass()) {
					SlideTiles[SelectedTile]->AddLocalOffset(FVector(0.f, -TileSize, 0.f));
					SlideTiles[SelectedTile - TilesPerRow]->AddLocalOffset(FVector(0.f, TileSize, 0.f));
					UChildActorComponent* temp = SlideTiles[SelectedTile - TilesPerRow];
					SlideTiles[SelectedTile - TilesPerRow] = SlideTiles[SelectedTile];
					SlideTiles[SelectedTile] = temp;
					SelectedTile -= TilesPerRow;
				}
			}
			if (MoveDirection.Y < 0) {
				if (SelectedTile / TilesPerRow < TilesPerRow - 1 && !SlideTiles[SelectedTile + TilesPerRow]->GetChildActorClass()) {
					SlideTiles[SelectedTile]->AddLocalOffset(FVector(0.f,TileSize, 0.f));
					SlideTiles[SelectedTile + TilesPerRow]->AddLocalOffset(FVector(0.f,-TileSize, 0.f));
					UChildActorComponent* temp = SlideTiles[SelectedTile + TilesPerRow];
					SlideTiles[SelectedTile + TilesPerRow] = SlideTiles[SelectedTile];
					SlideTiles[SelectedTile] = temp;
					SelectedTile += TilesPerRow;
				}
			}
		}	
	}
}

void ASecondSlidePuzzle::Select(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("To be implemented:Select"));
}

void ASecondSlidePuzzle::Back(const FInputActionValue& Value)
{
	if (Controller != nullptr) {

		if (Character!= nullptr) {
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->RemoveMappingContext(SlidePuzzletMappingContext);
				}

				PlayerController->bShowMouseCursor = false;

				Controller->Possess(Character);
				SlidePuzzleInteractComponent->OnInteract.AddDynamic(this, &ASecondSlidePuzzle::OnInteract);
				//Controller->UnPossess();
			}
		}
	}
}

void ASecondSlidePuzzle::Interact(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("To be implemented:InteractItem"));
	if (Controller != nullptr && SelectedTile>=0 && SelectedTile<TotalTiles && SlideTiles[SelectedTile]->GetChildActorClass()) {
		/*Character->AddToInventory(Cast<ACollectibleItems>(SlideTiles[SelectedTile]->GetChildActor()));
		SlideTiles[SelectedTile]->DestroyChildActor();
		SlideTiles[SelectedTile]->SetChildActorClass(nullptr);*/
	}

}

void ASecondSlidePuzzle::UseItem(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("To be implemented:UseItem"));
	if (Controller != nullptr && SelectedTile >= 0 && SelectedTile < TotalTiles && !SlideTiles[SelectedTile]->GetChildActorClass()) {
		//SlidePuzzleInteractComponent->GetMainCharacter()->AddToInventory(Cast<ACollectibleItems>(SlideTiles[SelectedTile]->GetChildActor()));
		UE_LOG(LogTemp, Warning, TEXT("In UseItem"));
		if (Character) {
			UE_LOG(LogTemp, Warning, TEXT("In UseItem:if"));
			/*SlideTiles[SelectedTile]->SetChildActorClass(ASlideTileClass::StaticClass(), Character->GetCurrentItem());
			Character->RemoveCurrentItemFromInventory();*/

			//APlayerController* PlayerController = Cast<APlayerController>(GetController());
			//const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//const FVector SpawnLocation = GetActorLocation() + FVector(-60+(TileSize*(SelectedTile%4),-60+((SelectedTile/4)*TileSize),16));

			////Set Spawn Collision Handling Override
			//FActorSpawnParameters ActorSpawnParams;
			//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//// Spawn the projectile at the muzzle
			//CurrentItem->SetActorLocation(SpawnLocation);
			//CurrentItem->SetActorHiddenInGame(false);
			//CurrentItem->SetIsCollected(false);
		}
		
	}
}

void ASecondSlidePuzzle::Look(const FInputActionValue& Value)
{

}

void ASecondSlidePuzzle::SetupPuzzle()
{

	for (int32 index = 0; index < 16; index++) {
		FName SocketName = FName(FString::Printf(TEXT("Socket_%d"), index));
		UChildActorComponent* ChildActor = CreateDefaultSubobject<UChildActorComponent>(FName(FString::Printf(TEXT("Tiles_%d"), index)));
		if (index < 1) {
			ChildActor->SetChildActorClass(ASlideTileClass::StaticClass());
		}
		else {
			ChildActor->SetChildActorClass(nullptr);
		}
		ChildActor->SetupAttachment(SlideFrame, SocketName);
		SlideTiles.Add(ChildActor);
	}
}

// Called every frame
void ASecondSlidePuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASecondSlidePuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//PickupItem
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::Interact);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::Move);

		//Selecting
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::Select);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::Look);

		//Use Item
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::UseItem);

		//MoveBack
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Triggered, this, &ASecondSlidePuzzle::Back);
	}

}

