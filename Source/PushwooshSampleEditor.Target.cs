// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;

public class PushwooshSampleEditorTarget : TargetRules
{
	public PushwooshSampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange(new string[] { "PushwooshSample" });
	}
}
