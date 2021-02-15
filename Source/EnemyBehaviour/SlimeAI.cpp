// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimeAI.h"
#include "SlimeCharacter.h"
#include "WaypointActor.h"
#include "MannequinCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"

ASlimeAI::ASlimeAI() 
{
	PrimaryActorTick.bCanEverTick = true;

	AISightRadius = 1500.0f;
	AISightAge = 5.0f;
	AILoseSightRadius = AISightRadius + 50.0f;
	AIFieldOfView = 90.0f;

	//	Create the sight configuration
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	//	Assign the variables created to the configuration
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	//	Turn on the detection of the AI
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//	Set the dominant sense as sight
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//	Pass the sight config
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//	Attach function when a new pawn is detected by the perception system
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASlimeAI::OnTargetPerceptionUpdated);

	bIsPlayerSeen = false;
	LastPositionPlayer = FVector(0, 0, 0);
	//	How far the character moves away after attacking (Slime Behaviour)
	RandomMoveRadius = 1000.0f;
}

void ASlimeAI::BeginPlay()
{
	Super::BeginPlay();

	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occcured"));
	}

	//	Get the Slime character
	EnemyCharacter = Cast<ASlimeCharacter>(GetPawn());
	//	Get the Patrol behaviour of the character, if this moves randomly or by waypoints
	MoveWaypoints = EnemyCharacter->MoveToWaypoints;
	MoveRandomWaypoints = EnemyCharacter->MoveToRandomWaypoint;

	if (MoveRandomWaypoints)
		MoveToRandomWaypoint();
	else if (MoveWaypoints)
		MoveToWaypoint();
}

void ASlimeAI::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}

void ASlimeAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FRotator ASlimeAI::GetControlRotation() const
{
	//	Check the pawn
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	//	Return the yaw rotation of the pawn detected
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

/*
*	When the character arrives to the destination, this function what is the
*	next decision of the character to do
*/
void ASlimeAI::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);	

	//	If the player is close, then the slime attacks and reset the attack
	if (EnemyCharacter->bIsPlayerClose)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASlimeAI::ResetAttackMove, 0.5f);
	}
	//	If the slime has already attacked, it look at the player again
	else if (EnemyCharacter->bIsAttack)
	{
		LookPlayer();
		//	If the slime see the player again, then chase him again
		if (bIsPlayerSeen) 
		{
			FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();
			TimerDel.BindUFunction(this, FName("MoveToPlayer"), PlayerLocation);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.0f, false);
		}
		else 
		{
			//	Start to patrol
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASlimeAI::MoveToWaypoint, EnemyCharacter->WaypointDelay);
		}
	}
	//	If the slime does not see the player, then go to waypoint and patrol
	else if (!bIsPlayerSeen)
	{
		if (MoveRandomWaypoints)
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASlimeAI::MoveToRandomWaypoint, EnemyCharacter->WaypointDelay);
		else if (MoveWaypoints)
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASlimeAI::MoveToWaypoint, EnemyCharacter->WaypointDelay);
	}
}

/*
*	When the perception conmponent is updated, this function get the
*	player's position and set if the player has been seen
*/
void ASlimeAI::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Cast<AMannequinCharacter>(Actor))
	{
		LastPositionPlayer = Stimulus.StimulusLocation;
		bIsPlayerSeen = Stimulus.WasSuccessfullySensed();
		//	If the slime has not attacked, then follow the player
		if (!EnemyCharacter->bIsAttack)
		{
			LookPlayer();
			TimerDel.BindUFunction(this, FName("MoveToPlayer"), LastPositionPlayer);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 1.0f, false);
		}
	}
}

/*
*	Move the character to the next waypoint base on the current position index,
*	And increment this index plus one and reset when it is equal to the 
*	lenght of the Waypoints array
*/
void ASlimeAI::MoveToWaypoint()
{
	if (EnemyCharacter->Waypoints[EnemyCharacter->CurrentWaypointIndex] == nullptr) return;

	EnemyCharacter->SetStateMode(StateMode::PATROL);
	//	Walking
	EnemyCharacter->StopSprint();
	//	Move to the waypoint
	MoveToActor(EnemyCharacter->Waypoints[EnemyCharacter->CurrentWaypointIndex], 5.0f);
	if (EnemyCharacter->CurrentWaypointIndex >= EnemyCharacter->Waypoints.Num() - 1)
	{
		EnemyCharacter->CurrentWaypointIndex = 0;
	}
	else
	{
		EnemyCharacter->CurrentWaypointIndex++;
	}
}

/*
*	Move the character to a random position in a radius considering the 
*	Navigation volume in the scene
*/
void ASlimeAI::MoveToRandomWaypoint()
{
	EnemyCharacter->SetStateMode(StateMode::PATROL);
	//	Walking
	EnemyCharacter->StopSprint();
	FVector Result;
	if (GetRandomPosition(GetPawn()->GetActorLocation(), RandomMoveRadius, Result)) 
	{
		//	Move to the hit position in the navigation volumen in the scene
		MoveToLocation(Result, 5.0f);
	}
}

/*
*	Reset the attack of the Slime, moving to a backward position
*/
void ASlimeAI::ResetAttackMove()
{
	EnemyCharacter->bIsAttack = true;
	FVector CurrentLocation = EnemyCharacter->GetActorLocation() - EnemyCharacter->GetActorForwardVector() * 800;

	MoveToLocation(CurrentLocation, 5.0f);
}

/*
*	Look at the player position, rotates the character to the player position in the scene
*/
void ASlimeAI::LookPlayer()
{
	EnemyCharacter->bIsAttack = false;
	//	Get the actor location
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();
	//	Do not rotate the character if the player is in air
	PlayerLocation.Z = EnemyCharacter->GetActorLocation().Z;
	//	Get the rotation target
	FRotator CharacterRotation = UKismetMathLibrary::FindLookAtRotation(EnemyCharacter->GetActorLocation(), PlayerLocation);
	//	Rotate the character to that rotation
	EnemyCharacter->SetActorRotation(CharacterRotation);
}

/*
*	Move the character to the player, if the player is in range of vision
*	then it move to the player actor in the scene, if not it moves 
*	to the last position that the character saw the player
*/
void ASlimeAI::MoveToPlayer(FVector PlayerSeenLocation)
{
	EnemyCharacter->SetStateMode(StateMode::CHASE);
	//	Running
	EnemyCharacter->Sprint();
	if (!EnemyCharacter->bIsAttack)
	{
		if (!bIsPlayerSeen || EnemyCharacter->bIsPlayerClose)
		{
			//	Moves to the last position of the player
			MoveToLocation(PlayerSeenLocation, 5.0f);
		}
		else
		{
			//	Moves to the actor player in the scene
			AMannequinCharacter* Player = Cast<AMannequinCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			MoveToActor(Player, 5.0f);
		}
	}
}

/*
*	Get a random position in the scene in a radius
*/
bool ASlimeAI::GetRandomPosition(const FVector Origin, float Radius, FVector& OutResult)
{
	//	Get the navigation volume
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (!NavSystem) return false;

	FVector Result;
	//	Get if the random position is walkable
	bool bSuccess = NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), Origin, Result, Radius);

	//Output
	OutResult = Result;
	return bSuccess;
}
