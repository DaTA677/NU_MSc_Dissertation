// Fill out your copyright notice in the Description page of Project Settings.


#include "KeypadKeyActor.h"
#include "Components/BoxComponent.h"

const TMap<EKeysPresent, FString> AKeypadKeyActor::KeyMeshInfo = {
	{EKeysPresent::Key_0,FString(TEXT("/Game/ImportedAssets/Keypad_key_0"))},
	{EKeysPresent::Key_1,FString(TEXT("/Game/ImportedAssets/Keypad_key_1"))},
	{EKeysPresent::Key_2,FString(TEXT("/Game/ImportedAssets/Keypad_key_2"))},
	{EKeysPresent::Key_3,FString(TEXT("/Game/ImportedAssets/Keypad_key_3"))},
	{EKeysPresent::Key_4,FString(TEXT("/Game/ImportedAssets/Keypad_key_4"))},
	{EKeysPresent::Key_5,FString(TEXT("/Game/ImportedAssets/Keypad_key_5"))},
	{EKeysPresent::Key_6,FString(TEXT("/Game/ImportedAssets/Keypad_key_6"))},
	{EKeysPresent::Key_7,FString(TEXT("/Game/ImportedAssets/Keypad_key_7"))},
	{EKeysPresent::Key_8,FString(TEXT("/Game/ImportedAssets/Keypad_key_8"))},
	{EKeysPresent::Key_9,FString(TEXT("/Game/ImportedAssets/Keypad_key_9"))},
	{EKeysPresent::Key_Back,FString(TEXT("/Game/ImportedAssets/Keypad_key_back"))},
	{EKeysPresent::Key_Enter,FString(TEXT("/Game/ImportedAssets/Keypad_key_enter"))},
};
const TMap<EKeysPresent, FVector> AKeypadKeyActor::KeyLocalOffsetInfo= {

};


AKeypadKeyActor::AKeypadKeyActor()
{
	HighlightMesh->SetVisibility(false);
	//InteractionVolume->SetBoxExtent(FVector());
}

void AKeypadKeyActor::Init(EKeysPresent InKeyNo)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(*KeyMeshInfo[InKeyNo]);

	if (Mesh.Succeeded()) {
		RenderedMesh->SetStaticMesh(Mesh.Object);
	}
	KeyNo = InKeyNo;

	
}

void AKeypadKeyActor::OnInteract()
{
	if (!HighlightMesh->IsVisible()) {
		HighlightMesh->SetVisibility(true);
	}
}

void AKeypadKeyActor::GetKeyLocalOffset(FVector& OutVector, const EKeysPresent& InKey)
{
	OutVector = KeyLocalOffsetInfo[InKey];
}

void AKeypadKeyActor::GetKeyMesh(FString& OutString, const EKeysPresent& InKey)
{
	OutString = KeyMeshInfo[InKey];
}

void AKeypadKeyActor::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);
}
