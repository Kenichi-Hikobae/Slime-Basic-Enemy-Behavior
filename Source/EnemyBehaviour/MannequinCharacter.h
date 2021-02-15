// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MannequinCharacter.generated.h"

UCLASS()
class ENEMYBEHAVIOUR_API AMannequinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequinCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
		class USpringArmComponent* BoomArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UAIPerceptionStimuliSourceComponent* AIStimulationSouce;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
		float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
		float SprintSpeedMultiplier;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoverForward(float Axis);
	void MoverRight(float Axis);
	void TunrRate(float Rate);
	void LookAtRate(float Rate);

	void Sprint();
	void StopSprint();

	float TurnRate;
	float LookRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
