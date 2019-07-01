// Fill out your copyright notice in the Description page of Project Settings.

#include "HockeyStick.h"
#include "TakeHandler.h"

// Sets default values
AHockeyStick::AHockeyStick()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Hockey Stick Root"));

	// Create components
	UStaticMeshComponent * stick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hockey Stick"));
	UBoxComponent *bottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Main"));
	UBoxComponent *top = CreateDefaultSubobject<UBoxComponent>(TEXT("Direction"));
	UTakeHandler *takeHandler = CreateDefaultSubobject<UTakeHandler>(TEXT("Take Handler"));

	// Attach Components
	top->SetupAttachment(stick);
	bottom->SetupAttachment(stick);
	stick->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Stick(TEXT("StaticMesh'/Game/DP/Models/Hockey/hockeyStick.HockeyStick'"));
	if (Stick.Succeeded())
	{
		stick->SetStaticMesh(Stick.Object);
		stick->SetRelativeScale3D(FVector(0.42f, 0.16f, 0.18f));
	}

	// Init take Areas
	top->SetRelativeScale3D(FVector(0.6f, 1.4f, 2.7f));
	bottom->SetRelativeScale3D(FVector(1.0f, 2.3f, 8.5f));

	top->SetRelativeLocation(FVector(0.0f, 0.0f, 330.0f));
	bottom->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	TakeRules *rules = (new TakeRules(stick, top))->addSecondaryCollision(bottom)
		->disablePhysics();
	takeHandler->init(rules);
}
