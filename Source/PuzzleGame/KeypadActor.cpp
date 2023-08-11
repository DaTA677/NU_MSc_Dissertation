// Fill out your copyright notice in the Description page of Project Settings.


#include "KeypadActor.h"
#include "KeypadKeyActor.h"
#include "PuzzleGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "TriggerComponent.h"
#include "TrialGameUI.h"
#include "Components/EditableTextBox.h"
#include "TrialGameUI.h"

AKeypadActor::AKeypadActor()
{
	InteractionLayer = EInteractionLayer::EXAMINE;

	FString MeshPath = FString(TEXT("/Game/ImportedAssets/Keypad_frame"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(*MeshPath);
	if (Mesh.Succeeded()) {
		RenderedMesh->SetStaticMesh(Mesh.Object);
	}
	//Working
	bActivated = false;
	CurrentLookKey = nullptr;
	TimesTried = 0;

	DoorTrigger = CreateDefaultSubobject<UTriggerComponent>("Trigger");

	StoryText.Add(FString(TEXT("That's the correct code")));
	StoryText.Add(FString(TEXT("Try Again")));

	for (int i = 1; i < 10; i++) {
		FName KeyName = FName(FString::Printf(TEXT("Key_%d"), i));
		//Key
		UChildActorComponent* ChildActor = CreateDefaultSubobject<UChildActorComponent>(KeyName);
		ChildActor->SetChildActorClass(AKeypadKeyActor::StaticClass());
		//Cast<AKeypadKeyActor>(ChildActor->GetChildActor())->SetKeyNo((EKeysPresent)i);
		ChildActor->SetupAttachment(RenderedMesh,KeyName);
		//Key Text
		UTextRenderComponent* DisplayText = CreateDefaultSubobject<UTextRenderComponent>(FName((KeyName.ToString()) + FString::Printf(TEXT("_Text"))));
		DisplayText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
		DisplayText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
		DisplayText->SetText(FText::FromString(FString::Printf(TEXT("%d"), i)));
		DisplayText->AddLocalOffset(FVector(0.f, 0.f, 5.2f));
		DisplayText->AddLocalRotation(FQuat(FRotator(90.0, 0, 0)));
		DisplayText->SetupAttachment(ChildActor);

		this->Keys.Add(ChildActor);
		this->KeyTexts.Add(DisplayText);
	}

	FName KeyName = FName(FString::Printf(TEXT("Key_back")));
	//Key
	UChildActorComponent* ChildActor = CreateDefaultSubobject<UChildActorComponent>(KeyName);
	ChildActor->SetChildActorClass(AKeypadKeyActor::StaticClass());
	//Cast<AKeypadKeyActor>(ChildActor->GetChildActor())->SetKeyNo(EKeysPresent::Key_Back);
	ChildActor->SetupAttachment(RenderedMesh, KeyName);
	//Key Text
	UTextRenderComponent* DisplayText = CreateDefaultSubobject<UTextRenderComponent>(FName((KeyName.ToString()) + FString::Printf(TEXT("_Text"))));
	DisplayText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	DisplayText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DisplayText->SetText(FText::FromString(FString::Printf(TEXT("Back"))));
	DisplayText->AddLocalOffset(FVector(0.f, 0.f, 5.2f));
	DisplayText->AddLocalRotation(FQuat(FRotator(90.0, 0, 0)));
	DisplayText->WorldSize = 10.0f;
	DisplayText->SetupAttachment(ChildActor);

	this->Keys.Add(ChildActor);
	this->KeyTexts.Add(DisplayText);

	KeyName = FName(FString::Printf(TEXT("Key_0")));
	//Key
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(KeyName);
	ChildActor->SetChildActorClass(AKeypadKeyActor::StaticClass());
	//Cast<AKeypadKeyActor>(ChildActor->GetChildActor())->SetKeyNo(EKeysPresent::Key_Enter);
	ChildActor->SetupAttachment(RenderedMesh, KeyName);
	//Key Text
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(FName((KeyName.ToString()) + FString::Printf(TEXT("_Text"))));
	DisplayText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	DisplayText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DisplayText->SetText(FText::FromString(FString::Printf(TEXT("0"))));
	DisplayText->AddLocalOffset(FVector(0.f, 0.f, 5.2f));
	DisplayText->AddLocalRotation(FQuat(FRotator(90.0, 0, 0)));

	DisplayText->SetupAttachment(ChildActor);

	this->Keys.Add(ChildActor);
	this->KeyTexts.Add(DisplayText);

	KeyName = FName(FString::Printf(TEXT("Key_enter")));
	//Key
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(KeyName);
	ChildActor->SetChildActorClass(AKeypadKeyActor::StaticClass());
	//Cast<AKeypadKeyActor>(ChildActor->GetChildActor())->SetKeyNo(EKeysPresent::Key_Enter);
	ChildActor->SetupAttachment(RenderedMesh, KeyName);
	//Key Text
	DisplayText = CreateDefaultSubobject<UTextRenderComponent>(FName((KeyName.ToString()) + FString::Printf(TEXT("_Text"))));
	DisplayText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	DisplayText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DisplayText->SetText(FText::FromString(FString::Printf(TEXT("Enter"))));
	DisplayText->AddLocalOffset(FVector(0.f, 0.f, 5.2f));
	DisplayText->AddLocalRotation(FQuat(FRotator(90.0, 0, 0)));
	DisplayText->WorldSize = 10.0f;
	DisplayText->SetupAttachment(ChildActor);

	this->Keys.Add(ChildActor);
	this->KeyTexts.Add(DisplayText);

	this->DisplayTextComponent = CreateDefaultSubobject<UTextRenderComponent>(FName(TEXT("Display Text")));
	this->DisplayTextComponent->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	this->DisplayTextComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Right;
	this->DisplayTextComponent->SetText(FText::FromString(FString::Printf(TEXT("0"))));
	this->DisplayTextComponent->AddLocalOffset(FVector(-63.f, -33.f, 15.f));
	this->DisplayTextComponent->AddLocalRotation(FQuat(FRotator(90.0, 0.0, 0.0)));
	this->DisplayTextComponent->WorldSize = 16.0f;
	this->DisplayTextComponent->SetupAttachment(RenderedMesh);

	this->KeypadCamera = CreateDefaultSubobject<UCameraComponent>("Keypad Camera");
	this->KeypadCamera->bUsePawnControlRotation = false;
	this->KeypadCamera->SetRelativeLocation(FVector(0.f, 0.f, 140.0f)); // Position the camera
	this->KeypadCamera->SetupAttachment(InteractionVolume);
	this->KeypadCamera->SetRelativeRotation(FQuat(FRotator(-90.0, 0, 180.0)));

	EnteredKeys.Reserve(4);
}

void AKeypadActor::OnInteract()
{
	Interact();
}

void AKeypadActor::PerformKeyOperation(EKeysPresent InKey)
{
	UE_LOG(LogTemp, Warning, TEXT("Key pressed:%d"), (int)InKey);
	if ((InKey) < EKeysPresent::Key_Back) {
		AddNumber(int(InKey));
	}
	else {
		InKey == EKeysPresent::Key_Back ? RemoveNumber() : CheckNumber();
	}
}

void AKeypadActor::AddNumber(int Number)
{
	if (EnteredKeys.Num() < 4) {
		EnteredKeys.Add(Number);
		
		UpdateDisplayText();
	}
}

void AKeypadActor::RemoveNumber()
{
	if (bActivated || Controller == nullptr) return;
	if (EnteredKeys.Num() > 0) {
		EnteredKeys.RemoveAt(EnteredKeys.Num() - 1);
		UpdateDisplayText();
	}
}

void AKeypadActor::CheckNumber()
{
	if (bActivated || Controller == nullptr) return;

	if (EnteredKeys.Num() != 4) {
		return;
	}
	TimesTried++;
	int Sum = 0;
	for (int Number : EnteredKeys) {
		Sum = (Sum * 10) + Number;
	}
	if (Sum == RequiredCode) {
		bActivated = true;
		DoorTrigger->OnActivated.Broadcast(bActivated,KeypadNo,TimesTried);
		if (MainCharacter->HUD) {
			MainCharacter->HUD->UpdateStoryText(FText::FromString(StoryText[0]), true);
			MainCharacter->HUD->SetShowStory(true);
		}
	}
	else {
		for (int i = 3; i >(-1); i--) {
			EnteredKeys.RemoveAt(i);
		}
		UpdateDisplayText();
		if (MainCharacter->HUD) {
			if (StoryText.Num() > StoryTextCounter+2) {
				MainCharacter->HUD->UpdateStoryText(FText::FromString(StoryText[StoryTextCounter+2]), true, 5.0f);
				StoryTextCounter++;
			}
			else {
				MainCharacter->HUD->UpdateStoryText(FText::FromString(StoryText[1]), true);
			}
			MainCharacter->HUD->SetShowStory(true);
		}
	}
}

void AKeypadActor::UpdateDisplayText()
{
	FString Sum = FString::Printf(TEXT(""));
	for (int Number : EnteredKeys) {
		Sum = FString::Printf(TEXT("%s%d"),*Sum,Number);
	}
	
	DisplayTextComponent->SetText(FText::FromString(Sum));
	//UE_LOG(LogTemp, Warning, TEXT("Number Sum:%d"), Sum);
}

void AKeypadActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AKeypadActor::OnBoxBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AKeypadActor::OnBoxEndOverlap);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(KeypadMappingContext, 2);
		}
	}
}

void AKeypadActor::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InInputComponent))
	{
		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKeypadActor::Look);
		//MoveBack
		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Triggered, this, &AKeypadActor::Back);
		//Select
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &AKeypadActor::Select);
		//Delete
		EnhancedInputComponent->BindAction(DeleteAction, ETriggerEvent::Triggered, this, &AKeypadActor::RemoveNumber);
		//Enter
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Triggered, this, &AKeypadActor::CheckNumber);
	}
}

void AKeypadActor::Interact()
{
	if (MainCharacter && bNearCharacter) {
		if (APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController())) {
			PlayerController->Possess(this);
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(KeypadMappingContext, 2);
			}
			PlayerController->bShowMouseCursor = false;
			InteractionVolume->OnComponentBeginOverlap.Remove(this, FName(TEXT("OnBoxBeginOverlap")));
			SetActorTickEnabled(false);
			//UE_LOG(LogTemp, Warning, TEXT("ActorTickDisabled"));
			if (this->HighlightMesh) {
				this->HighlightMesh->SetVisibility(false);
			}
			if (!CurrentLookKey) {
				CurrentLookKey = Cast<AKeypadKeyActor>(Keys[0]->GetChildActor());
				CurrentLookKey->SetHighLightMeshVisibility(true);
			}
			if (MainCharacter->HUD) {
				MainCharacter->HUD->UpdateInstructions(FText::FromString(KeypadInstructions), true);
				MainCharacter->HUD->LookCursor->SetRenderOpacity(0.0f);
			}

		}
	}
}

void AKeypadActor::Back(const FInputActionValue& Value)
{

	if (Controller != nullptr) {
		if (MainCharacter != nullptr) {
			if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
				{
					Subsystem->RemoveMappingContext(KeypadMappingContext);
				}
				PlayerController->bShowMouseCursor = false;
				InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AKeypadActor::OnBoxBeginOverlap);
				if (HighlightMesh) {
					HighlightMesh->SetCollisionEnabled(RenderedMesh->GetCollisionEnabled());
				}
				SetActorTickEnabled(true);
				if (CurrentLookKey) CurrentLookKey->SetHighLightMeshVisibility(false);
				CurrentLookKey = nullptr;
				if (MainCharacter->HUD) {
					MainCharacter->HUD->UpdateInstructions(FText::FromString(MainCharacter->PlayerInstructions), true);
					MainCharacter->HUD->LookCursor->SetRenderOpacity(1.0f);
				}
				Controller->Possess(MainCharacter);
			}
		}
	}
}

void AKeypadActor::Look(const FInputActionValue& Value)
{
	if (Controller) {
			FVector2D MoveComponent = Value.Get<FVector2D>();
			if (CurrentLookKey) {
				int CurrentKeyNo = (int)CurrentLookKey->GetKeyNo();
				if (MoveComponent.X > 0) {
					if ((CurrentKeyNo != 0 && CurrentKeyNo % 3 != 0)) {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
					else if (CurrentKeyNo == 0){
						CurrentLookKey->SetHighLightMeshVisibility(false);
						CurrentLookKey = Cast<AKeypadKeyActor>(Keys[11]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
				}
				if ((MoveComponent.X < 0) && (CurrentKeyNo % 3 != 1)) {
					if (CurrentKeyNo != 0) {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						if (CurrentKeyNo != 11)
							CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo-2]->GetChildActor());
						else
							CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo - 1]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
					else {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						CurrentLookKey = Cast<AKeypadKeyActor>(Keys[9]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
				}
				if (MoveComponent.Y > 0) {
					if ((CurrentKeyNo != 0 && (CurrentKeyNo - 1) / 3 != 0)) {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						if(CurrentKeyNo!=11)
							CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo-4]->GetChildActor());
						else
							CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo - 3]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
					else if (CurrentKeyNo == 0) {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						CurrentLookKey = Cast<AKeypadKeyActor>(Keys[7]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
					}
				}
				if (MoveComponent.Y < 0 && ((CurrentKeyNo - 1) / 3 != 3) && CurrentKeyNo!=0) {
						CurrentLookKey->SetHighLightMeshVisibility(false);
						CurrentLookKey = Cast<AKeypadKeyActor>(Keys[CurrentKeyNo + 2]->GetChildActor());
						CurrentLookKey->SetHighLightMeshVisibility(true);
				}
			}
		//}
	}
}

void AKeypadActor::Select(const FInputActionValue& Value)
{
	if (Controller && !bActivated) {
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
			/*FHitResult HitResult;
			if (PlayerController && PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult)) {
				FString Name = HitResult.GetComponent()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("HitActor:%s"),*Name);
				if (AKeypadKeyActor* HitActor = Cast<AKeypadKeyActor>(HitResult.GetActor())) {
					PerformKeyOperation(HitActor->GetKeyNo());
				}
			}*/
			if (CurrentLookKey) {
				PerformKeyOperation(CurrentLookKey->GetKeyNo());
			}
		}
	}
}

void AKeypadActor::OnBoxBeginOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuzzleGameCharacter* Character = Cast<APuzzleGameCharacter>(OtherActor);

	if (Character != nullptr) {
		MainCharacter = Character;
		bNearCharacter = true;
		SetActorTickEnabled(true);
	}

}

void AKeypadActor::OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter != nullptr) {
		bNearCharacter = false;
		MainCharacter = nullptr;
		if (HighlightMesh) {
			HighlightMesh->SetVisibility(false);
		}
	}
}

void AKeypadActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
				Subsystem->RemoveMappingContext(KeypadMappingContext);
			}
		}
	}
}
