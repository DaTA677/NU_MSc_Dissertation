// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectibleItems.h"
#include "SlideTileClass.generated.h"

UCLASS(BlueprintType)
class PUZZLEGAME_API ASlideTileClass : public ACollectibleItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlideTileClass();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterial* Material;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 TileNo;

	static uint16 TilesCreated;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UMaterial* GetMaterial() { return Material; }
};
