// Fill out your copyright notice in the Description page of Project Settings.


#include "SlendermanCharacter.h"


// !! GENERAL
// to make a BP child of this class, when creating a Blueprint, look for this class' name in the "All Classes" Section


// Sets default values
// to implement class functions, you need to use [class name]::[function name]
ASlendermanCharacter::ASlendermanCharacter()
{
 	// Set this character to call Tick() every frame. 
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASlendermanCharacter::BeginPlay()
{
	// with Super::[function name] we can invoke the parent's function implementation
	Super::BeginPlay();	
}

// Called every frame
void ASlendermanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASlendermanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


