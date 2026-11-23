// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// defines to UE that this is an Enum we can use in Blueprints
// specifically the "BlueprintType" Specifier signals that
UENUM(BlueprintType)
// enums are just integers. We derive from uint8 for memory-specific reasons (we confine it to 1 Byte; usually 256 slots are enough). You could also use uint16 or int32
// UE usually needs the last two slots for engine-internal assignments, so keep it well below 255!
enum class EEnemyState : uint8
{
	// breakdown of the syntax:
	// None > the enum type we can use in Code
	// UMETA > a macro for UE to define meta data
	// DisplayName = "None" > what we see in UE, e.g. in Blueprints when using this as a Variable. This is the name in the dropdown to choose the type

	None UMETA(DisplayName = "None"),
	Patrol UMETA(DisplayName = "Patrol"),
	Stalk UMETA(DisplayName = "Stalk"),
	Chase UMETA(DisplayName = "Chase"),
	Catch UMETA(DisplayName = "Catch"),
};
