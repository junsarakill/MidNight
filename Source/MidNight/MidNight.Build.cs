// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MidNight : ModuleRules
{
	public MidNight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
