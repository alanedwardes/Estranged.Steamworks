#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FEstSteamModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void HandleLaunchURL(const FString& URL, FString* Error);
};
