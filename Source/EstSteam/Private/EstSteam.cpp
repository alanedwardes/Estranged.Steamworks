#include "EstSteam.h"
#include "Misc/CoreDelegates.h"
#include "GenericPlatform/GenericPlatformProcess.h"

// Steamworks
#pragma push_macro("hResult")
#undef hResult
#include "steam/steam_api.h"
#pragma pop_macro("hResult")

#define LOCTEXT_NAMESPACE "FEstSteamModule"

void FEstSteamModule::StartupModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.BindRaw(this, &FEstSteamModule::HandleLaunchURL);
}

void FEstSteamModule::ShutdownModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.Unbind();
}

void FEstSteamModule::HandleLaunchURL(const FString& URL, FString* Error)
{
	bool bHandled = false;

	if (SteamAPI_IsSteamRunning())
	{
		ISteamFriends* SteamFriendsPtr = SteamFriends();
		if (SteamFriendsPtr)
		{
			SteamFriendsPtr->ActivateGameOverlayToWebPage(TCHAR_TO_UTF8(*URL));
			bHandled = true;
		}
	}

	if (!bHandled)
	{
		FPlatformProcess::LaunchFileInDefaultExternalApplication(*URL);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEstSteamModule, EstSteam)
