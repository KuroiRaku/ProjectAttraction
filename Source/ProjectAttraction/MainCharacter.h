// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include <ProjectAttraction\EnemyCharacter.h>
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTATTRACTION_API AMainCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	AEnemyCharacter* EnemyCharacterReference;

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	/** Called for side to side input */
	void MoveRight(float Val);

	void MoveForward(float Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbook* AttractAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float Score;

	// 1 for Red, 2 for green, 3 for red
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	int TargetColor;

public: 

	FTimerHandle TimerForAttracting;

	// A boolean variable to check whether it is in the interacting range of NPC characters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool IsInterecting;

	// A boolean variable to check whether it is doing attracting action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool IsAttracting;

	// A boolean variable to check whether it is doing Absorbing Enemy Heart
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool IsAbsorbing;

	void Attract();

	void StopAttracting();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
