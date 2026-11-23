// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyStateTypes.h"
#include "SlendermanCharacter.h"

// !! the ".generated.h" header include needs to always be last! All other includes should be before this one
#include "SlendermanAIController.generated.h"


UCLASS()
// the AI Controller is our character's brain. We have all logic of how it should behave in here
class SLENDERMANPROJECT_API ASlendermanAIController : public AAIController
{
	GENERATED_BODY()


public:
	virtual void Tick(float DeltaSeconds) override;
	

protected:
	// called once the character is possessed
	// this can be on spawn or when it is placed in the Level
	// you can change this in the Class Defaults of the inheriting BP
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slenderman")
    float PatrolAcceptanceRadius = 50.0f;

	// we mark it with UPROPERTY, so that the Garbage Collector (GC) takes care of it, to avoid dangling pointers
	// the same thing with using a "TObjectPtr" type instead of a raw pointer "*"
	// if we would use raw pointers, we'd have to take care of garbage collecting ourselves
	UPROPERTY()
	TObjectPtr<ASlendermanCharacter> Slenderman;

	UPROPERTY()
	TObjectPtr<APawn> Player;


public:
	// UFUNCTION exposes this as a callable function to UE, in BPs
	UFUNCTION(BlueprintCallable, Category = "Slenderman Actions")
	void UpdateState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable, Category = "Slenderman Actions")
	void FindNextPatrolPoint();

	UFUNCTION(BlueprintCallable, Category = "Slenderman Actions")
	void EvaluatePlayer();
	

private:
	// all types provided by UE have a prefix (see slides nr 04 for more detail)
	// e.g. TArray or FVector
	TArray<FVector> PatrolPoints;
    int32 PatrolIndex = 0;

	void ChasePlayer(float Distance);
	void StalkPlayer(float Distance);
	void CatchPlayer();
	void RestartLevel();

	// these Timer Handles are needed when we want to set up a timer
	// they save the time and function that should be called after the time has run out
    FTimerHandle RestartHandle;
    FTimerHandle PatrolWaitHandle;

};
