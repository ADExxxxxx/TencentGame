// Copyright Epic Games, Inc. All Rights Reserved.


#include "TGGameModeBase.h"
#include "TGBaseCharacter.h"
#include "TGPlayerController.h"
#include "TGPlayerState.h"
#include "UI/TGGameHUD.h"

ATGGameModeBase::ATGGameModeBase()
{
	DefaultPawnClass = ATGBaseCharacter::StaticClass();
	PlayerControllerClass = ATGPlayerController::StaticClass();
	HUDClass = ATGGameHUD::StaticClass();
	PlayerStateClass = ATGPlayerState::StaticClass();
}

void ATGGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ATGPlayerState>(KillerController->PlayerState) : nullptr;
	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
}