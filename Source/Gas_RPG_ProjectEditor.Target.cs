// Copyright Arthur "Zer0HeaD" Zakirov

using UnrealBuildTool;
using System.Collections.Generic;

public class Gas_RPG_ProjectEditorTarget : TargetRules
{
	public Gas_RPG_ProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Gas_RPG_Project" } );
	}
}
