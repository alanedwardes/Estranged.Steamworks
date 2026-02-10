#include "EstSteam.h"
#include "EstSteamStatics.h"
#include "Misc/CoreDelegates.h"
#include "GameFramework/PlayerState.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Misc/ConfigCacheIni.h"

#pragma push_macro("hResult")
#undef hResult
#include "steam/steam_api.h"
#pragma pop_macro("hResult")

#include "Misc/MessageDialog.h"

static void SendStatsRequest(const FString& FailureReason)
{
	FString StatsEndpoint;
	if (GConfig)
	{
		GConfig->GetString(TEXT("/Script/EstSteam.EstSteam"), TEXT("StatsEndpoint"), StatsEndpoint, GEngineIni);
	}

	if (StatsEndpoint.IsEmpty())
	{
		return;
	}

	FString Suffix = FString::Printf(TEXT("reason=%s"), *FailureReason);
	if (StatsEndpoint.Contains(TEXT("?")))
	{
		StatsEndpoint += TEXT("&") + Suffix;
	}
	else
	{
		StatsEndpoint += TEXT("?") + Suffix;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(StatsEndpoint);
	Request->SetTimeout(2.0f);
	Request->SetHeader(TEXT("Cache-Control"), TEXT("no-cache"));
	Request->SetHeader(TEXT("Pragma"), TEXT("no-cache"));
	Request->ProcessRequest();
}

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
			SendStatsRequest(TEXT("steam_not_running"));
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Steam is required to play.")));
			FPlatformMisc::RequestExit(false);
			return;
		}

		if (!SteamApps()->BIsSubscribed())
		{
			SendStatsRequest(TEXT("license_check_failed"));
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
