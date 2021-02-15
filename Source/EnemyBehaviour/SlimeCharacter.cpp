// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASlimeCharacter::ASlimeCharacter()
{
	Mode = StateMode::PATROL;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//	Rotate the player with the movement vector
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	How fast the character can rotate
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	//	Start waypoint where the character starts to patrol
	CurrentWaypointIndex = 0;
	//	Delayed movement between waypoints
	WaypointDelay = 5.0f;

	//	Default value of the patrol behaviour
	MoveToWaypoints = false;
	MoveToRandomWaypoint = true;

	//	Default walk speed
	WalkSpeed = 225.0f;
	//	Sprint speed
	SprintSpeed = WalkSpeed * 1.35f;

	//	Initialize the default walk speed of the character
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bIsPlayerClose = false;
	bIsAttack = false;
}

// Called when the game starts or when spawned
void ASlimeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASlimeCharacter::Sprint()
{
	//	Increase the speed of the character
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASlimeCharacter::StopSprint()
{
	//	Decrease the speed of the character with the default walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASlimeCharacter::SetStateMode(StateMode ModeChange)
{
	//	Change the state of the character
	Mode = ModeChange;
}

