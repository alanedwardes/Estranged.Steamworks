#include "EstSteam.h"
#include "EstSteamStatics.h"
#include "Misc/CoreDelegates.h"
#include "GameFramework/PlayerState.h"
#include "GenericPlatform/GenericPlatformProcess.h"

FNativeFuncPtr OriginalGetPlayerNameFunc = nullptr;

#pragma push_macro("hResult")
#undef hResult
#include "steam/steam_api.h"
#pragma pop_macro("hResult")

void FEstSteamModule::StartupModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.BindRaw(this, &FEstSteamModule::HandleLaunchURL);

	UFunction* TargetFunc = APlayerState::StaticClass()->FindFunctionByName(FName("GetPlayerName"));
	if (TargetFunc)
	{
		OriginalGetPlayerNameFunc = TargetFunc->GetNativeFunc();
		TargetFunc->SetNativeFunc(&UEstSteamStatics::execGetSteamPlayerName);
	}
}

void FEstSteamModule::ShutdownModule()
{
	FCoreDelegates::LaunchCustomHandlerForURL.Unbind();
    
	if (OriginalGetPlayerNameFunc)
	{
		UFunction* TargetFunc = APlayerState::StaticClass()->FindFunctionByName(FName("GetPlayerName"));
		if (TargetFunc)
		{
			TargetFunc->SetNativeFunc(OriginalGetPlayerNameFunc);
		}
	}
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEstSteamModule, EstSteam)
