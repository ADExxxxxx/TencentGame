// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTGAME_API ATGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATGGameModeBase();

	void Killed(AController* KillerController, AController* VictimController);
};
