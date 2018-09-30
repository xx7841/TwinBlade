// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TwinBlade : ModuleRules {
	public TwinBlade(ReadOnlyTargetRules Target) 
        : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}