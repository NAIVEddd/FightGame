// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum WeaponState
{
	WEAPON_Droped		UMETA(DisplayName = "Droped"),
	WEAPON_Equiped		UMETA(DisplayName = "Equiped")
};
