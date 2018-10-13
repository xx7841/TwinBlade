// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TwinBladeGameMode.h"
#include "TwinBladeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATwinBladeGameMode::ATwinBladeGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PlayerCharacter/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}