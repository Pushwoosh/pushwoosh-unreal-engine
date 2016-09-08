// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PushwooshSampleTarget : TargetRules
{
	public PushwooshSampleTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
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
