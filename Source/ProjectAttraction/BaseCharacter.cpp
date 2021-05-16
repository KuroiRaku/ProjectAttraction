// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "PaperSpriteComponent.h"
#include "Math/Rotator.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Set up Sprite Component
	Character = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Character"));
	Character->AttachTo(RootComponent);
	Character->SetWorldScale3D(FVector(4, 4, 4));
	Character->SetRelativeScale3D(FVector(4, 4, 4));
	Character->SetRelativeRotation(FRotator(0, 90, 0));
	Character->SetVisibility(true);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


