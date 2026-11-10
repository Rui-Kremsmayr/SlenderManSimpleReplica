// Fill out your copyright notice in the Description page of Project Settings.


#include "SlendermanAIController.h"
#include "SlendermanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"


void ASlendermanAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    Slenderman = Cast<ASlendermanCharacter>(GetPawn());
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PatrolPoint"), Found);

    for (AActor* Actor : Found)
    {
        PatrolPoints.Add(Actor->GetActorLocation());
        UE_LOG(LogTemp, Warning, TEXT("Found Patrol Point: %s"), *Actor->GetActorLocation().ToString());
    }
}

void ASlendermanAIController::Tick(float DeltaSeconds)
{

}

void ASlendermanAIController::UpdateState(EEnemyState NewState)
{
    
}

void ASlendermanAIController::FindNextPatrolPoint()
{
    
}

void ASlendermanAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    
}