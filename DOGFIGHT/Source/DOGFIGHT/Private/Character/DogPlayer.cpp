// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DogPlayer.h"

// Sets default values
ADogPlayer::ADogPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADogPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADogPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADogPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

