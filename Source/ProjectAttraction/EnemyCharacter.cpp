// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter() {

	NPCAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudioComponent"));
	NPCAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	// I want the sound to follow the pawn around, so I attach it to the Pawns root.
	NPCAudioComponent->AttachTo(RootComponent);
	NPCAudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

