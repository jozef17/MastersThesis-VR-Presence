// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DP : ModuleRules
{
	public DP(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Link Kinect v2 SDK & LM
        PublicAdditionalLibraries.Add("C:\\Program Files\\Microsoft SDKs\\Kinect\\v2.0_1409\\Lib\\x64\\Kinect20.Face.lib");
        PublicAdditionalLibraries.Add("C:\\Program Files\\Microsoft SDKs\\Kinect\\v2.0_1409\\Lib\\x64\\Kinect20.Fusion.lib");
        PublicAdditionalLibraries.Add("C:\\Program Files\\Microsoft SDKs\\Kinect\\v2.0_1409\\Lib\\x64\\Kinect20.lib");
        PublicAdditionalLibraries.Add("C:\\Program Files\\Microsoft SDKs\\Kinect\\v2.0_1409\\Lib\\x64\\Kinect20.VisualGestureBuilder.lib");
        PublicAdditionalLibraries.Add("C:\\LeapSDK\\lib\\x64\\Leap.lib");

        PublicIncludePaths.Add("C:\\Program Files\\Microsoft SDKs\\Kinect\\v2.0_1409\\inc");
        PublicIncludePaths.Add("C:\\LeapSDK\\include");
    }
}
