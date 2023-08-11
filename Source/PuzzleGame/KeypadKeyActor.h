// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActors.h"
#include "KeypadKeyActor.generated.h"

UENUM(BlueprintType)
enum class EKeysPresent : uint8 {
	Key_0 = 0 UMETA(DisplayName = "0 Key"),
	Key_1 UMETA(DisplayName = "1 Key"),
	Key_2 UMETA(DisplayName = "2 Key"),
	Key_3 UMETA(DisplayName = "3 Key"),
	Key_4 UMETA(DisplayName = "4 Key"),
	Key_5 UMETA(DisplayName = "5 Key"),
	Key_6 UMETA(DisplayName = "6 Key"),
	Key_7 UMETA(DisplayName = "7 Key"),
	Key_8 UMETA(DisplayName = "8 Key"),
	Key_9 UMETA(DisplayName = "9 Key"),
	Key_Back UMETA(DisplayName = "Back Key"),
	Key_Enter UMETA(DisplayName = "Enter Key"),
};


/**
 * 
 */
UCLASS()
class PUZZLEGAME_API AKeypadKeyActor : public AInteractableActors
{
	GENERATED_BODY()
	
public:
	AKeypadKeyActor();

	void Init(EKeysPresent InKeyNo);

	//AInteractableActors interface
	virtual void OnInteract() override;
	//End of AInteractableActors interface

	static void GetKeyLocalOffset(FVector& OutVector, const EKeysPresent& InKey);
	static void GetKeyMesh(FString& OutVector, const EKeysPresent& InKey);

	EKeysPresent GetKeyNo() const { return KeyNo; }
	void SetKeyNo(const EKeysPresent& InKey ) { KeyNo =InKey; }

protected:

	static const TMap<EKeysPresent, FString> KeyMeshInfo;
	static const TMap<EKeysPresent, FVector> KeyLocalOffsetInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category=Game)
	EKeysPresent KeyNo;
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	// End of APawn interface

};
