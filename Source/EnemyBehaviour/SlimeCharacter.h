// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WaypointActor.h"
#include "SlimeCharacter.generated.h"

/**
 * The modes that the Character can be in.
 * PATROL - Patrolling between Waypoints.
 * CHASE - Chasing the Player.
 */
UENUM(BlueprintType)
enum class StateMode : uint8
{
	PATROL UMETA(DisplayName = "PATROL"),
	CHASE UMETA(DisplayName = "CHASE")
};

UCLASS()
class ENEMYBEHAVIOUR_API ASlimeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlimeCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoints")
		StateMode Mode;
	
	//	Total Waypoints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints")
		TArray<AWaypointActor*> Waypoints;

	//	Index of the current waypoint the character is going towards
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Waypoints")
		int32 CurrentWaypointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
		float WaypointDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
		bool MoveToWaypoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
		bool MoveToRandomWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
		float SprintSpeed;	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bIsPlayerClose;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		bool bIsAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Sprint();

	void StopSprint();

	// Called to change the state of the character
	void SetStateMode(StateMode ModeChange);
};
