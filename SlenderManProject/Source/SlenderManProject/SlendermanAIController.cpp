// Fill out your copyright notice in the Description page of Project Settings.


#include "SlendermanAIController.h"
#include "SlendermanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" // this is for the character movement component
#include "Kismet/GameplayStatics.h" // this has all common functions like GetWorld() or GetAllActorsWithTag()
#include "Navigation/PathFollowingComponent.h" // we need this for Navigation
#include "NavigationSystem.h" // we need this for Navigation



// !! RECAP
// 1) access pointer properties/functions = "->"
// 2) access class static properties/functions = "::"
// 3) access object properties/functions = "."


void ASlendermanAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // get the pawn that we assigned this AIController to
    // to assign an AIController to a Pawn, go to the Class Defaults in the BP and look for "Pawn" > "AI Controller Class"
    Slenderman = Cast<ASlendermanCharacter>(GetPawn());
    // get the player controller pawn at index 0 (fine in this case, since it's a singleplayer game)
    Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // we always need Pointers for UObjects!
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("PatrolPoint"), Found);
    // to assign a Tag to an Actor, go to Class Defaults and search for "Tags". There should be a "Tags" Array in the Actor Section. Add your tag to the Array
    // this is a great alternative to trying to get all actors of a class
    // never do any of those calls on Tick! They are very memory heavy. OnPossess is only called once, so it's fine

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
    // nullptr check; UE automatically converts nullptr checks to booleans
    // same as (Slenderman == nullptr)
    // or (IsValid(Slenderman)) or (Slenderman.IsValid()); also catches pending-kill objects, which is often the preferred version
    if (!Slenderman)
        return;

    // fail-safe, since sometimes the Player is not yet initialized when OnPossess is called
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
    // to log enums, we need to cast them to integers
    // there's a few types that can't be easily logged; look at Slides nr 04 for more detail

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

    // wraps the index back to 0 once we go higher than the array length
    // possible in other programming languages as well and very useful :D
    PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();

    // 1) Move from point to point, instead of teleporting
    // MoveToLocation(PatrolPoints[PatrolIndex], PatrolAcceptanceRadius);

    // 2) teleporting
    FVector Dest = PatrolPoints[PatrolIndex];

    // get the current navigation system
    // !! to make this work, sometimes you need to manually add the "NavigationSystem" Module in the following file:
    // "[Project name].Build.cs", so for this project "SlenderManProject.Build.cs"
    // add it to the "PrivateDependencyModuleNames" array
    if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
    {
        // we need to project our next patrol point onto the nav mesh bounds
        // to make sure we don't teleport to a position Slenderman can't actually walk to
        // otherwise it's possible that he gets stuck
        FNavLocation Projected;
        if (NavSys->ProjectPointToNavigation(Dest, Projected, FVector(100.0f, 100.0f, 200.0f))) // the vector is the extent of the area it can be projected to
            Dest = Projected.Location;
    }

    Slenderman->SetActorLocation(Dest);

    // starting a timer
    // (Timer Handle, object to call from, function to call after timer ran out passed as reference, time)
    GetWorldTimerManager().SetTimer(PatrolWaitHandle, this, &ASlendermanAIController::FindNextPatrolPoint, Slenderman->WaitTimeAtPoint);

    // reminder that we always need to use a Pointer to log strings!
    UE_LOG(LogTemp, Warning, TEXT("Slenderman moving to %s"), *PatrolPoints[PatrolIndex].ToString());
}

void ASlendermanAIController::StalkPlayer(float Distance)
{
    MoveToActor(Player, PatrolAcceptanceRadius);

    if (Distance < Slenderman->SightRadius * 0.4f)
    {
        // we can access the CharacterMovement component of our Character
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
    MoveToActor(Player, PatrolAcceptanceRadius);

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

    // this broadcasts our delegate
    // we can assign to it in BPs
    Slenderman->OnCaught.Broadcast();

    // start a timer
    GetWorldTimerManager().SetTimer(RestartHandle, this, &ASlendermanAIController::RestartLevel, 3.0f);
}

void ASlendermanAIController::RestartLevel()
{
    // reloads the current level
    // "true" means that it reloads fully, starting cleanly again
    // "false" would mean that it preserves e.g. session context (mostly relevant for networking)
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetMapName()), true);
}

void ASlendermanAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (!Slenderman)
        return;

    // 1) Move from point to point, instead of teleporting
    // if (Slenderman->CurrentState == EEnemyState::Patrol && Result.IsSuccess())
    // {
    //     FindNextPatrolPoint();
    // }
}