// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyStateTypes.h"

// !! the ".generated.h" header include needs to always be last! All other includes should be before this one
#include "SlendermanCharacter.generated.h"


// we need to first declare Delegates (i.e. Events) like this, before we can use them
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCaught); // FOnCaught is the delegate "type", basically
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAppearedAtPatrolPoint);

// for ones with parameters:
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovedTo, FVector, NewPos)
// the first param is the delegate name, the second the param type, the third the param name
// parameter types and names need to be separated with a comma!


// defines to UE that this is a class that can be referenced
UCLASS()
// any Actor-type should have the "A"-prefix (gets automatically added by UE)
class SLENDERMANPROJECT_API ASlendermanCharacter : public ACharacter // we need to inherit from "Character" for AIController Characters
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlendermanCharacter();
	
	// tells UE that this is a Variable that we can also access in BPs
	// VisibleAnywhere/EditAnywhere > concerning the Details Panel
	// BlueprintReadOnly/BlueprintReadWrite > concerning adding nodes in the BP Graph
	// Category > puts them in a category that can be seen in the BP Context Menu when searching for Nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	EEnemyState CurrentState = EEnemyState::Patrol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slenderman")
	float CatchRadius = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Slenderman")
    float WaitTimeAtPoint = 5.0f;

protected:
	// Called when the game starts or when spawned
	// "virtual" and "override" need to be added for overwritten functions
	virtual void BeginPlay() override;

// you can have multiple public/private/protected sections without issues
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Delegate definition
	UPROPERTY(BlueprintAssignable, Category = "Slenderman Delegates")
	FOnCaught OnCaught;

	UPROPERTY(BlueprintAssignable, Category = "Slenderman Delegates")
	FOnAppearedAtPatrolPoint OnAppearedAtPatrolPoint;

};
