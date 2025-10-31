// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Arua : ModuleRules
{
    public Arua(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        // 프로젝트 경로 설정
        PublicIncludePaths.Add("Arua");

        PublicDependencyModuleNames.AddRange(new string[] {
            "AIModule",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "UMG",
            "Slate",
            "GameplayTags",
            "GameplayAbilities",
            "GameplayTasks",
            "AnimGraphRuntime",
            "NavigationSystem",
            "Niagara",
            "MovieScene",
            "MovieSceneTracks",
            "LevelSequence",
        });


        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
