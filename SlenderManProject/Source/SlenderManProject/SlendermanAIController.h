// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyStateTypes.h"
#include "SlendermanCharacter.h"

#include "SlendermanAIController.generated.h"

/**
 * 
 */
UCLASS()
class SLENDERMANPROJECT_API ASlendermanAIController : public AAIController
{
	GENERATED_BODY()


public:
	virtual void Tick(float DeltaSeconds) override;
	

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slenderman")
    float PatrolAcceptanceRadius = 50.0f;


public:
	UFUNCTION(BlueprintCallable, Category = "Slenderman Actions")
	void UpdateState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable, Category = "Slenderman Actions")
	void FindNextPatrolPoint();
	

private:
	ASlendermanCharacter* Slenderman;
	APawn* Player;
	
	TArray<FVector> PatrolPoints;
    int32 PatrolIndex = 0;
};
