// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;

public class PushwooshSampleTarget : TargetRules
{
	public PushwooshSampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "PushwooshSample" } );
	}
}
