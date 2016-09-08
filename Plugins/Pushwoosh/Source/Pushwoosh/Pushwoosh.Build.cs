// Copyright 2016 Pushwoosh Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Pushwoosh : ModuleRules
{
	public Pushwoosh(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Pushwoosh/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Pushwoosh/Private",
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
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, BuildConfiguration.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "Pushwoosh_APL.xml")));
		}
	}
}
