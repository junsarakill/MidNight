using UnrealBuildTool;

public class MidNight : ModuleRules
{
	public MidNight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Json",
			"JsonUtilities",
			"HTTP",
			"PythonScriptPlugin" 
		});

		PublicIncludePaths.AddRange(new string[] {
			"C:/Program Files/Epic Games/UE_5.4/Engine/Plugins/Experimental/PythonScriptPlugin/Source/PythonScriptPlugin/Public"
		});
	}
}
