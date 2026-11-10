// Fill out your copyright notice in the Description page of Project Settings.


#include "SlendermanCharacter.h"

// Sets default values
ASlendermanCharacter::ASlendermanCharacter()
{
 	// Set this character to call Tick() every frame. 
	// You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlendermanCharacter::BeginPlay()
{
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


