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

    UpdateState(EEnemyState::Patrol);
    FindNextPatrolPoint();
}

void ASlendermanAIController::Tick(float DeltaSeconds)
{
    if (!Slenderman)
        return;

    if (!Player)
        Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (Slenderman->CurrentState == EEnemyState::Catch)
        return;

    EvaluatePlayer();
}

void ASlendermanAIController::UpdateState(EEnemyState NewState)
{
    if (!Slenderman)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Enemy state changed from %d to %d"), (uint8)Slenderman->CurrentState, (uint8)NewState);

    Slenderman->CurrentState = NewState;
}

void ASlendermanAIController::EvaluatePlayer()
{
    if (!Player || !Slenderman)
        return;

    const float Distance = FVector::Dist(Slenderman->GetActorLocation(), Player->GetActorLocation());
    const bool SeesPlayer = LineOfSightTo(Player);

    switch (Slenderman->CurrentState)
    {
        case EEnemyState::Patrol:
            if (/* SeesPlayer &&  */Distance < Slenderman->SightRadius)
            {
                GetWorldTimerManager().ClearTimer(PatrolWaitHandle);
                UE_LOG(LogTemp, Warning, TEXT("Canceling timer"));
                UpdateState(EEnemyState::Stalk);
            }
            break;

        case EEnemyState::Stalk:
            StalkPlayer(Distance);
            break;

        case EEnemyState::Chase:
            ChasePlayer(Distance);
            break;

        default:
            break;
    }
}

void ASlendermanAIController::FindNextPatrolPoint()
{
    if (PatrolPoints.Num() == 0)
        return;

    PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
    UE_LOG(LogTemp, Warning, TEXT("Finding new patrol point"));

    // Move from point to point
    // MoveToLocation(PatrolPoints[PatrolIndex], PatrolAcceptanceRadius);

    FVector Dest = PatrolPoints[PatrolIndex];

    if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
    {
        FNavLocation Projected;
        if (NavSys->ProjectPointToNavigation(Dest, Projected, FVector(100.0f, 100.0f, 200.0f)))
            Dest = Projected.Location;
    }

    Slenderman->SetActorLocation(Dest);

    GetWorldTimerManager().SetTimer(PatrolWaitHandle, this, &ASlendermanAIController::FindNextPatrolPoint, Slenderman->WaitTimeAtPoint);

    UE_LOG(LogTemp, Warning, TEXT("Slenderman moving to %s"), *PatrolPoints[PatrolIndex].ToString());
}

void ASlendermanAIController::StalkPlayer(float Distance)
{
    MoveToActor(Player, 50.0f);

    if (Distance < Slenderman->SightRadius * 0.4f)
    {
        Slenderman->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
        UpdateState(EEnemyState::Chase);
    }
    else if (Distance >= Slenderman->SightRadius)
    {
        UpdateState(EEnemyState::Patrol);
        FindNextPatrolPoint();
    }
}

void ASlendermanAIController::ChasePlayer(float Distance)
{    
    MoveToActor(Player, 50.0f);

    if (Distance >= Slenderman->SightRadius)
    {
        Slenderman->GetCharacterMovement()->MaxWalkSpeed = 50.0f;
        UpdateState(EEnemyState::Patrol);
        FindNextPatrolPoint();
    }
    else if (Distance < Slenderman->CatchRadius)
    {
        CatchPlayer();
    }
}

void ASlendermanAIController::CatchPlayer()
{
    UpdateState(EEnemyState::Catch);
    Slenderman->OnCaught.Broadcast();

    GetWorldTimerManager().SetTimer(RestartHandle, this, &ASlendermanAIController::RestartLevel, 3.0f);
}

void ASlendermanAIController::RestartLevel()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
}

void ASlendermanAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!Slenderman)
        return;

    // Move from point to point
    // if (Slenderman->CurrentState == EEnemyState::Patrol && Result.IsSuccess())
    // {
    //     FindNextPatrolPoint();
    // }
}