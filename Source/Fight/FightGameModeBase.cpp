// Copyright Epic Games, Inc. All Rights Reserved.


#include "FightGameModeBase.h"
#include "Soldier.h"

AFightGameModeBase::AFightGameModeBase()
{
	//PlayerControllerClass = ASoldier::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn>
		PlayerPawnBPClass(TEXT("/Game/Character/Blueprints/BP_Soldier"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}