// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TryToDriveTarget : TargetRules
{
	public TryToDriveTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		bUsesSteam = true;
        // ”казываем им€ выходного файла

        ExtraModuleNames.AddRange( new string[] { "TryToDrive" } );
	}
}
