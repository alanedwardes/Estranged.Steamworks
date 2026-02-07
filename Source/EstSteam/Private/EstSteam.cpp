#include "EstSteam.h"
#include "EstSteamStatics.h"
#include "Misc/CoreDelegates.h"
#include "GameFramework/PlayerState.h"
#include "GenericPlatform/GenericPlatformProcess.h"

#pragma push_macro("hResult")
#undef hResult
#include "steam/steam_api.h"
#pragma pop_macro("hResult")

#include "Misc/MessageDialog.h"

void FEstSteamModule::StartupModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.BindRaw(this, &FEstSteamModule::HandleLaunchURL);

	// Check Steam license
	bool bShouldCheckLicense = false;
#if UE_BUILD_SHIPPING
	bShouldCheckLicense = true;
#else
	if (!GIsEditor)
	{
		bShouldCheckLicense = true;
	}
#endif

	if (bShouldCheckLicense)
	{
		if (SteamApps() == nullptr)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Steam is required to play.")));
			FPlatformMisc::RequestExit(false);
			return;
		}

		if (!SteamApps()->BIsSubscribed())
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No license found.")));
			FPlatformMisc::RequestExit(false);
			return;
		}
	}

	UFunction* TargetFunc = APlayerState::StaticClass()->FindFunctionByName(FName("GetPlayerName"));
	if (TargetFunc)
	{
		TargetFunc->SetNativeFunc(&UEstSteamStatics::execGetSteamPlayerName);
	}
}

void FEstSteamModule::ShutdownModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.Unbind();
}

void FEstSteamModule::HandleLaunchURL(const FString& URL, FString* Error)
{
	ISteamFriends* SteamFriendsPtr = SteamFriends();
	if (SteamFriendsPtr)
	{
		SteamFriendsPtr->ActivateGameOverlayToWebPage(TCHAR_TO_UTF8(*URL));
		return;
	}

	FPlatformProcess::LaunchFileInDefaultExternalApplication(*URL);
}
	
IMPLEMENT_MODULE(FEstSteamModule, EstSteam)
