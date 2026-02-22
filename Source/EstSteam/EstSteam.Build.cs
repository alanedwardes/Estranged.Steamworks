using UnrealBuildTool;

public class EstSteam : ModuleRules
{
    public EstSteam(ReadOnlyTargetRules Target) : base(Target)
    {
        IWYUSupport = IWYUSupport.Full;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "OnlineSubsystem",
                "OnlineSubsystemSteam",
                "Steamworks",
                "HTTP"
            }
        );
    }
}
