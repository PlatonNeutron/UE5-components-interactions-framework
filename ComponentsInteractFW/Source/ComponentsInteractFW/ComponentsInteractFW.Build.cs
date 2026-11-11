// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ComponentsInteractFW : ModuleRules
{
	public ComponentsInteractFW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ComponentsInteractFW",
			"ComponentsInteractFW/Variant_Platforming",
			"ComponentsInteractFW/Variant_Platforming/Animation",
			"ComponentsInteractFW/Variant_Combat",
			"ComponentsInteractFW/Variant_Combat/AI",
			"ComponentsInteractFW/Variant_Combat/Animation",
			"ComponentsInteractFW/Variant_Combat/Gameplay",
			"ComponentsInteractFW/Variant_Combat/Interfaces",
			"ComponentsInteractFW/Variant_Combat/UI",
			"ComponentsInteractFW/Variant_SideScrolling",
			"ComponentsInteractFW/Variant_SideScrolling/AI",
			"ComponentsInteractFW/Variant_SideScrolling/Gameplay",
			"ComponentsInteractFW/Variant_SideScrolling/Interfaces",
			"ComponentsInteractFW/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
