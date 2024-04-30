// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YousomAssesment : ModuleRules
{
	public YousomAssesment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG", "GameplayTasks" });
    }
}
