#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EstSteamStatics.generated.h"

UCLASS()
class UEstSteamStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    DECLARE_FUNCTION(execGetSteamPlayerName);
};
