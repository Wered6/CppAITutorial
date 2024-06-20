// Copyright Epic Games, Inc. All Rights Reserved.

#include "CppAITutorialGameMode.h"
#include "CppAITutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACppAITutorialGameMode::ACppAITutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
