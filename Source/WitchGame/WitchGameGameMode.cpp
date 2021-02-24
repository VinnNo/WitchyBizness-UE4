// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WitchGameGameMode.h"
#include "WitchGameCharacter.h"

AWitchGameGameMode::AWitchGameGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AWitchGameCharacter::StaticClass();	
}
