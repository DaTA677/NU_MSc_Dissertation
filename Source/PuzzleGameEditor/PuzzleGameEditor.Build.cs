// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PuzzleGameEditor : ModuleRules
{
	public PuzzleGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UnrealEd","PuzzleGame", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[]{ });
	}
}
