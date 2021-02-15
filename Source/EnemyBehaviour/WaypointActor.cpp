// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointActor.h"
#include "SlimeCharacter.h"
#include <Runtime\Engine\Classes\Components\BoxComponent.h>

// Sets default values
AWaypointActor::AWaypointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
	//	Add a box component to the root component, to be able to trigger some actions
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxComponent->SetupAttachment(GetRootComponent());
	//	Bind an action when something overlap with the box component
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypointActor::OnPlayerEnter);
}

// Called when the game starts or when spawned
void AWaypointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypointActor::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlimeCharacter* Enemy = nullptr;

	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComponent == nullptr)) return;

	Enemy = Cast<ASlimeCharacter>(OtherActor);

	if (Enemy == nullptr) return;
	
	// Do stuff when the enemy character arrives to the waypoint position
	
}
