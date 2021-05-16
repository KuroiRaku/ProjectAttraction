// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
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

AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnOverlapEnd);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-45.0f, 0.0f, 0.0f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	IsAttracting = false;
	IsInterecting = false;
	IsAbsorbing = false;
}

void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !IsAttracting)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//left
		if (Value < 0)
		{
			SetActorRotation(FRotator(0.f, -180.f, 0.f));
		}
		//right
		else {
			SetActorRotation(FRotator(0.f, 0.f, 0.f));
		}

		AddMovementInput(Direction, Value);
		Character->SetFlipbook(RunningAnimation);
	}
}

void AMainCharacter::MoveForward(float Value)
{
	//normal movement speed
	if ((Controller != NULL) && (Value != 0.0f) && !IsAttracting)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
		Character->SetFlipbook(RunningAnimation);
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings

	PlayerInputComponent->BindAction("Attract", IE_Pressed, this, &AMainCharacter::Attract);
	PlayerInputComponent->BindAction("Attract", IE_Released, this, &AMainCharacter::StopAttracting);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
}

void AMainCharacter::Attract()
{
	if (!IsAttracting) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attracting!")));
		IsAttracting = true;

		if (Character && AttractAnimation) {
			Character->SetFlipbook(AttractAnimation);
		}
	}
}

void AMainCharacter::StopAttracting()
{
	if (IsAttracting) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stop Attracting!")));
		IsAttracting = false;
		if (Character && IdleAnimation) {
			Character->SetFlipbook(IdleAnimation);
		}
	}
}

// This only trigger once when they overlap, it won't keep checking
void AMainCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// check if Actors do not equal nullptr
	if (OtherActor && (OtherActor != this))
	{
		AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OtherActor);
		if (enemyCharacter)
		{
			IsInterecting = true;
			EnemyCharacterReference = enemyCharacter;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Start Overlapping! Enemy Time Left: %f"), EnemyCharacterReference->TimeNeededForAttracting));
		}

	}
}

void AMainCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(OtherActor);
		if (enemyCharacter) 
		{
			IsInterecting = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stop Overlapping!")));
		}
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// every second, the time needed to finish attracting will decrease
	if (IsInterecting) 
	{
		if (IsAttracting)
		{
			if (EnemyCharacterReference) 
			{
				if (EnemyCharacterReference->Color == TargetColor && !EnemyCharacterReference->FallInLove)
				{
					if (HP >= EnemyCharacterReference->HP)
					{
						// When finish absorbing heart
						if (EnemyCharacterReference->TimeNeededForAttracting <= 1 && EnemyCharacterReference->TimeNeededForAttracting > 0)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Finish Attracting and minus HP %f"), EnemyCharacterReference->HP));
							HP -= EnemyCharacterReference->HP;

							if (EnemyCharacterReference->Character) {
								EnemyCharacterReference->Character->SetFlipbook(EnemyCharacterReference->FallInLoveAnimation);
							}

							TargetColor += 1;
							// Changing RGB
							if (TargetColor >= 4 || TargetColor <= 0)
							{
								// Reset to Green
								TargetColor = 1;
							}
							IsAbsorbing = false;

							if (EnemyCharacterReference->Character) {
								EnemyCharacterReference->Character->SetFlipbook(EnemyCharacterReference->FallInLoveAnimation);
							}

							EnemyCharacterReference->HP = 0;
							EnemyCharacterReference->TimeNeededForAttracting = 0;
							EnemyCharacterReference->FallInLove = true;
						}
						else if (EnemyCharacterReference->TimeNeededForAttracting > 1)
						{
							// Only set the flipbook once
							if (!IsAbsorbing)
							{
								if (EnemyCharacterReference->Character) 
								{
									if (EnemyCharacterReference->HeartBeingAbsorbAnimation) {
										EnemyCharacterReference->Character->SetFlipbook(EnemyCharacterReference->HeartBeingAbsorbAnimation);
									}
									else
									{
										GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy HeartBeingAbsorbAnimation is missing")));
									}
								}
								else
								{
									GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy paper2d flipbook component is missing")));
								}
							}

							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attracting NPCS")));
							IsAbsorbing = true;
							EnemyCharacterReference->TimeNeededForAttracting -= 0.06;
						}
					}
				}
			}
		}
	}
}
