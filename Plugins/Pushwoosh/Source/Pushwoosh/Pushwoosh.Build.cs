// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Pushwoosh : ModuleRules
{
	public Pushwoosh(ReadOnlyTargetRules Target) : base(Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Public")
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Private")
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
				"Launch"
			}
			);
		

		PrivatePCHHeaderFile = "Private/PushwooshPrivatePCH.h";

		if (Target.Platform == UnrealTargetPlatform.IOS) {
			PublicAdditionalFrameworks.Add(
					new UEBuildFramework(
						"Pushwoosh",
						"../../lib/iOS/Pushwoosh.embeddedframework.zip"
					)
				);

			PublicAdditionalLibraries.Add("z");
			PublicAdditionalLibraries.Add("stdc++");
		}
		else if(Target.Platform == UnrealTargetPlatform.Android)
		{
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Pushwoosh_APL.xml"));
		}
	}
}
