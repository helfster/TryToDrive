// Copyright Hubber Studios 2023. All Rights Reserved.

using UnrealBuildTool;

public class SaveFileEditor : ModuleRules
{
    public SaveFileEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]{
            "Core", "CoreUObject", "Engine", "Projects", "InputCore", "UnrealEd", "ToolMenus",
            "Slate", "SlateCore", "DesktopPlatform", "WorkspaceMenuStructure"
        });
    }
}
