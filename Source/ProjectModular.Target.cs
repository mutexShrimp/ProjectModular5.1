// Copyright Shrimp Chen 2025 All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectModularTarget : TargetRules
{
	public ProjectModularTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("ProjectModular");
	}
}
