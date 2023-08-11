// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialGameUI.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include <Runtime/Core/Public/Misc/OutputDeviceNull.h>

void UTrialGameUI::UpdateInventoryText(FText InText, bool bInShowInventory)
{
	Inventory->SetText(InText);
	bShowInstructionsText = bInShowInventory;

}

void UTrialGameUI::UpdateStoryText(FText InText, bool bInShowStoryText, float InStoryTextDelayTime)
{
	StoryText->SetText(InText);
	StoryTextDelayTime = InStoryTextDelayTime;
}

void UTrialGameUI::UpdateInstructions(FText InText, bool bInShowInstructions)
{
	Instructions->SetText(InText);
}

void UTrialGameUI::SetShowInventory(bool bShow)
{
	bShowInventoryText = bShow;
	FEditorScriptExecutionGuard ScriptGuard; // Sets GAllowActorScriptExecutionInEditor to TRUE
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ShowInventory"),ar, NULL,true);
	
}

void UTrialGameUI::SetShowInstructions(bool bShow)
{
	bShowInstructionsText = bShow;
	FEditorScriptExecutionGuard ScriptGuard; // Sets GAllowActorScriptExecutionInEditor to TRUE
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ShowInstructions"), ar, NULL, true);
}

void UTrialGameUI::SetShowStory(bool bShow)
{
	bShowStoryText = bShow;
	FEditorScriptExecutionGuard ScriptGuard; // Sets GAllowActorScriptExecutionInEditor to TRUE
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ShowStory"), ar, NULL, true);
}


