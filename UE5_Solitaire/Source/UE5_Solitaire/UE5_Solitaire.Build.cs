// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UE5_Solitaire : ModuleRules
{
	public UE5_Solitaire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Paper2D" });

        PrivateDependencyModuleNames.AddRange(new string[] {
           // "ChunkDownloader"
        });

		//PublicIncludePaths.AddRange(new string[] {
		//	"UE5_Solitaire/Public/GameLogic/",
  //          "UE5_Solitaire/Public/SimpleFramework/",

  //          "UE5_Solitaire/GameLogic/",
  //          "UE5_Solitaire/SimpleFramework/",
  //      });

		//PrivateIncludePaths.AddRange(new string[] {
  //          "UE5_Solitaire/Private/GameLogic/",
  //          "UE5_Solitaire/Private/SimpleFramework/",
  //          "UE5_Solitaire/GameLogic/",
  //          "UE5_Solitaire/SimpleFramework/",
  //      });

		//InternalIncludePaths.AddRange(new string[] {
		//           "GameLogic",
		//      });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
