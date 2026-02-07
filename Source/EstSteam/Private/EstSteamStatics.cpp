#include "EstSteamStatics.h"
#include "GameFramework/PlayerState.h"

#include "steam/steam_api.h"

DEFINE_FUNCTION(UEstSteamStatics::execGetSteamPlayerName)
{
	P_FINISH;
	FString& Result = *(FString*)RESULT_PARAM;

	ISteamFriends* SteamFriendsPtr = SteamFriends();
	if (SteamFriendsPtr)
	{
		const char* PersonaName = SteamFriendsPtr->GetPersonaName();
		if (PersonaName)
		{
			Result = FString(UTF8_TO_TCHAR(PersonaName));
			return;
		}
	}

	Result = FString();
}
