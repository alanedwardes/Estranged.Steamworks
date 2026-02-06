extern FNativeFuncPtr OriginalGetPlayerNameFunc;

#include "EstSteamStatics.h"
#include "GameFramework/PlayerState.h"

#include "steam/steam_api.h"

DEFINE_FUNCTION(UEstSteamStatics::execGetSteamPlayerName)
{
	ISteamFriends* SteamFriendsPtr = SteamFriends();
	if (SteamFriendsPtr)
	{
		const char* PersonaName = SteamFriendsPtr->GetPersonaName();
		if (PersonaName)
		{
			P_FINISH;
			P_NATIVE_BEGIN;
			FString& Result = *(FString*)RESULT_PARAM;
			Result = FString(UTF8_TO_TCHAR(PersonaName));
			P_NATIVE_END;
			return;
		}
	}

	if (OriginalGetPlayerNameFunc)
	{
		OriginalGetPlayerNameFunc(Context, Stack, RESULT_PARAM);
	}
	else
	{
		P_FINISH;
	}
}
