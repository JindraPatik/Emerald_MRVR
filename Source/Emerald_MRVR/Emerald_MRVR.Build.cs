// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Emerald_MRVR : ModuleRules
{
	public Emerald_MRVR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
			"HeadMountedDisplay",
			"OculusXRInput",
			"OculusXRHMD", 
			"XRBase",
			"AIModule",
			"GameplayTags",
			"Niagara",
			"NiagaraCore",
			"OculusInteraction", 
			"OculusInteractionLibrary",
			"OpenXRMsftHandInteraction",
			"OpenXRHandTracking",
			"MixedRealityCaptureFramework",
			"SimpleHMD",
			"OpenXR",
			"MRUtilityKit"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
