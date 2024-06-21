// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSDemoGameMode.h"
#include "EOSDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EOSPlayerController.h"
#include "EOSGameSession.h"

AEOSDemoGameMode::AEOSDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AEOSPlayerController::StaticClass();
	GameSessionClass = AEOSGameSession::StaticClass();
}
