// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PushwooshSampleEditorTarget : TargetRules
{
	public PushwooshSampleEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "PushwooshSample" } );
	}
}
