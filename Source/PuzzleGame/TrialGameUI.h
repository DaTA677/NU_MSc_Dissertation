// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrialGameUI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UTrialGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	/**Updates the HUD to  show the Inventory item*/
	void UpdateInventoryText(FText InText, bool bInShowInventory);
	/**Updates the HUD to  show the Story text*/
	void UpdateStoryText(FText InText, bool bInShowStoryText, float InStoryTextDelayTime = 2.0f);
	/**Updates the HUD to  show the Instructions text*/
	void UpdateInstructions(FText InText, bool bInShowInstructions);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UEditableTextBox* LookCursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "UI")
	bool bShowInventoryText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UMultiLineEditableTextBox* StoryText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, category = "UI")
	bool bShowStoryText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UMultiLineEditableTextBox* Instructions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "UI")
	bool bShowInstructionsText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "UI")
		float StoryTextDelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "UI")
		float CurrentTime;

	void SetShowInventory(bool bShow);
	void SetShowInstructions(bool bShow);
	void SetShowStory(bool bShow);
};
