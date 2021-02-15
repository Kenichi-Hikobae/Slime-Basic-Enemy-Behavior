// Fill out your copyright notice in the Description page of Project Settings.


#include "MannequinCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMannequinCharacter::AMannequinCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//	Create the BoomArm
	BoomArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	BoomArm->SetupAttachment(RootComponent);
	BoomArm->TargetArmLength = 300.0f;
	BoomArm->bUsePawnControlRotation = true;

	//	Create the camera and attach it to the BoomArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(BoomArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	//	Set the Player movement
	//	Orientate the player with the direction vector
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	How fast the player rotates
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	//	Walking normal speed
	WalkSpeed = 400.0f;
	//	How long the player can increment his velocity
	SprintSpeedMultiplier = 2.0f;
	//	Initialize the walk speed in the character movement
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//	Rate of the mouse input (Yaw Input)
	TurnRate = 45;
	//	Rate of the mouse input (Pitch Input)
	LookRate = 45;
}

// Called when the game starts or when spawned
void AMannequinCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMannequinCharacter::MoverForward(float Axis)
{
	//	Check if there is no Input
	if (Controller != NULL && Axis != 0.0f) 
	{
		//	Get the Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		//	Set the Yaw Rotation
		const FRotator Yaw(0, Rotation.Yaw, 0);
		//	Set the vector of the X axis Input
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		//	Apply the movement with the input front direction
		AddMovementInput(Direction, Axis);
	}
}

void AMannequinCharacter::MoverRight(float Axis)
{
	//	Check if there is no Input
	if (Controller != NULL && Axis != 0.0f)
	{
		//	Get the Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		//	Set the Yaw Rotation
		const FRotator Yaw(0, Rotation.Yaw, 0);
		//	Set the vector of the Y axis Input
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
		//	Apply the movement with the input side direction
		AddMovementInput(Direction, Axis);
	}
}

void AMannequinCharacter::TunrRate(float Rate)
{
	//	Get the forward rotation and apply the rotation with the rate input
	AddControllerYawInput(Rate * GetWorld()->GetDeltaSeconds() * TurnRate);
}

void AMannequinCharacter::LookAtRate(float Rate)
{
	//	Get the sideward rotation and apply the rotation with the rate input
	AddControllerPitchInput(Rate * GetWorld()->GetDeltaSeconds() * LookRate);
}

void AMannequinCharacter::Sprint()
{
	//	Increase the walk speed
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void AMannequinCharacter::StopSprint()
{
	//	Decrease the walk speed with the default value
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

// Called every frame
void AMannequinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//	& -> Address of the function
	//	Mouse Input Movement
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//	Axis Input Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMannequinCharacter::MoverForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMannequinCharacter::MoverRight);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMannequinCharacter::LookAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMannequinCharacter::TunrRate);

	//	Bind Actions Input
	//	Start Jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//	Stop Jumping
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//	Start Running
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMannequinCharacter::Sprint);
	//	Stop Running
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMannequinCharacter::StopSprint);
}

