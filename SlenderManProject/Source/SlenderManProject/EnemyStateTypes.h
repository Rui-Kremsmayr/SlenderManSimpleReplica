// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None UMETA(DisplayName = "None"),
	Patrol UMETA(DisplayName = "Patrol"),
	Stalk UMETA(DisplayName = "Stalk"),
	Chase UMETA(DisplayName = "Chase"),
	Catch UMETA(DisplayName = "Catch"),
};
