// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyStateTypes.h"

#include "SlendermanCharacter.generated.h"


UCLASS()
class SLENDERMANPROJECT_API ASlendermanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlendermanCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	EEnemyState CurrentState = EEnemyState::Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	float CatchRadius = 200.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
