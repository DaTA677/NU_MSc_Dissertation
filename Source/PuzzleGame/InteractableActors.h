// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InteractableActors.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class APuzzleGameCharacter;

UENUM(BlueprintType)
enum class EInteractionLayer : uint8 {
	
	WORLD=0 UMETA(DisplayName="Interaction Object"),
	PICKUP UMETA(DisplayName="Pick up Object"),
	SLIDEPUZZLE UMETA(DisplayName = "Slide puzzle"),
	SLOT UMETA(DisplayName = "Slots"),
	EXAMINE UMETA(DisplayName = "Examinable Object"),
	FINISH UMETA(DisplayName = "End Game Object"),
	MAXLAYER,

	InteractionWORLD_ = 1<<(uint8)WORLD|1<<(uint8)PICKUP,
	InteractionPICKUP_ = 1<<(uint8)WORLD,
	InteractionSLIDEPUZZLE_ = 1<<(uint8)SLIDEPUZZLE,
	InteractionSLOT_ = 1 << (uint8)PICKUP | 1 << (uint8)FINISH,
	InteractionEXAMINE_= 1<<(uint8)EXAMINE,
	InteractionFINISH_ = 1<<(uint8)FINISH
};

const uint8 Interactions[(size_t)EInteractionLayer::MAXLAYER] = {
	(uint8)EInteractionLayer::InteractionWORLD_,
	(uint8)EInteractionLayer::InteractionPICKUP_,
	(uint8)EInteractionLayer::InteractionSLIDEPUZZLE_,
	(uint8)EInteractionLayer::InteractionSLOT_,
	(uint8)EInteractionLayer::InteractionEXAMINE_,
	(uint8)EInteractionLayer::InteractionFINISH_,
};

static inline bool DoInteractionLayersOverlap(EInteractionLayer& a, EInteractionLayer& b) {
	return Interactions[(uint8)a] & (1 << (uint8)b);
}

UCLASS(Abstract, ClassGroup = (Custom))
class PUZZLEGAME_API AInteractableActors : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInteractableActors();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Interaction")
		UBoxComponent* InteractionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RenderedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* HighlightMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Interaction")
		EInteractionLayer InteractionLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		uint8 bNearCharacter=false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APuzzleGameCharacter* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		FString InventoryText;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EInteractionLayer GetIneteractionLayer() const { return InteractionLayer; }
	void SetInteractionLayer(const EInteractionLayer& InInteractionLayer) { InteractionLayer = InInteractionLayer; }

	void SetMainCharacter(APuzzleGameCharacter* InCharacter) { MainCharacter = InCharacter; }
	APuzzleGameCharacter* GetMainCharacter() { return MainCharacter; }

	uint8 IsNearCharacter()const { return bNearCharacter; }
	void SetNearCharacter(const uint8& bInNearCharacter) { bNearCharacter = bInNearCharacter; }

	UStaticMeshComponent* GetHighlightMesh() { return HighlightMesh; }
	void SetHighLightMeshVisibility(const uint8& InbVisibility) { HighlightMesh->SetVisibility((bool)InbVisibility); }

	FString GetInventoryText()const { return InventoryText; }
	virtual void OnInteract() PURE_VIRTUAL(AInteractableActors::OnInteract, );

	///**Collision Set for highlight*/
	//UFUNCTION()
	//	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	///**Collision end for highlight*/
	//UFUNCTION()
	//	void OnBoxEndOverlap(UPrimitiveComponent* OVerlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
