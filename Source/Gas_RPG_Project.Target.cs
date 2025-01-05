// Copyright Arthur "Zer0HeaD" Zakirov

using UnrealBuildTool;
using System.Collections.Generic;

public class Gas_RPG_ProjectTarget : TargetRules
{
	public Gas_RPG_ProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Gas_RPG_Project" } );
	}
}
