// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_enigmaGameMode.h"
#include "Project_enigmaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject_enigmaGameMode::AProject_enigmaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
