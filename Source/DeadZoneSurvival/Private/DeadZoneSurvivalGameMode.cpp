// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadZoneSurvival/Public/DeadZoneSurvivalGameMode.h"
#include "DeadZoneSurvival/Public/DeadZoneSurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeadZoneSurvivalGameMode::ADeadZoneSurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
