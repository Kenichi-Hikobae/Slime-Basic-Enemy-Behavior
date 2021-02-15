// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController.h"
#include "SlimeAI.generated.h"

/**
 * 
 */
UCLASS()
class ENEMYBEHAVIOUR_API ASlimeAI : public AAIController
{
	GENERATED_BODY()

public:
	ASlimeAI();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void MoveToWaypoint();

	UFUNCTION()
		void MoveToRandomWaypoint();
	
	UFUNCTION()
		void ResetAttackMove();
	
	UFUNCTION()
		void LookPlayer();

	UFUNCTION()
		void MoveToPlayer(FVector PlayerSeenLocation);

	bool GetRandomPosition(const FVector Origin, float Radius, FVector& OutResult);	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class ASlimeCharacter* EnemyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AISightRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AISightAge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AILoseSightRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float AIFieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsPlayerSeen;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		FVector LastPositionPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float RandomMoveRadius = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool MoveWaypoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool MoveRandomWaypoints;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;

protected:

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
