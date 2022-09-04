// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TencentGame : ModuleRules
{
	public TencentGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
			
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[] {
			"TencentGame/Public/Player", 
			"TencentGame/Public/Components",
			"TencentGame/Public/Test",
			"TencentGame/Public/Weapon",
			"TencentGame/Public/Animations",
			"TencentGame/Public/AI",
			"TencentGame/Public/AI/Tasks"
		});
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
