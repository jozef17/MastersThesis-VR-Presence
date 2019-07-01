// Fill out your copyright notice in the Description page of Project Settings.

#include "Hockey.h"

AHockey::AHockey() : AGoalGame()
{
	spawnPoint = FVector(0.0f, -10.0f, 20.0f);
	CreateGoal();
	CreateScoreBoard(FVector(-175, -380, 0), FRotator(0.0f, -10.0f, 0.0f));

	// Create Puck
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Puck(TEXT("StaticMesh'/Game/DP/Models/Hockey/Puck.Puck'"));
	if (Puck.Succeeded())
	{
		PlayingObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puck"));
		PlayingObject->SetupAttachment(RootComponent);
		PlayingObject->SetRelativeLocation(spawnPoint);
		PlayingObject->SetStaticMesh(Puck.Object);
		PlayingObject->SetSimulatePhysics(true);
		PlayingObject->SetRelativeScale3D(FVector(3.0f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> Ice(TEXT("Material'/Game/DP/Materials/Hockey/Ice.Ice'"));
	PlayArea->SetMaterial(0, Ice.Object);
}

void AHockey::rotateHockeyStick()
{
	if (left)
		HockeyStick->SetActorRotation(GetActorRotation() + FRotator(0.0f, -90.0f, -40.0f));
	else
		HockeyStick->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, -40.0f));
	left = !left;
}

void AHockey::CreateGoal()
{
	// Hockey Goal
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoalObj(TEXT("StaticMesh'/Game/DP/Models/Hockey/HockeyGoal.HockeyGoal'"));
	if (GoalObj.Succeeded())
	{
		Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hockey Goal"));
		Goal->SetStaticMesh(GoalObj.Object);
		Goal->SetupAttachment(GoalRoot);

		GoalAreaBox->SetRelativeScale3D(FVector(2.75f, 0.65f, 1.7f));
		GoalAreaBox->SetRelativeLocation(FVector(0.0f, -19.2f, 54.0f));
	}

	GoalRoot->SetRelativeLocation(FVector(0.0f, -200.0f, 0.0f));
}

void AHockey::CreateStick()
{
	FVector location = GetActorLocation() + GetActorRotation().RotateVector(FVector(0.0f, 0.0f, 55.0f) + spawnPoint);
	HockeyStick = GetWorld()->SpawnActor<AHockeyStick>(location, GetActorRotation() + FRotator(0.0f, 90.0f, -40.0f), FActorSpawnParameters());
	HockeyStick->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AHockey::BeginPlay()
{
	Super::BeginPlay();
	CreateStick();
}

void AHockey::ResetGame()
{
	Super::ResetGame();

	if (HockeyStick != nullptr)
		HockeyStick->Destroy();

	CreateStick();
}

void AHockey::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HockeyStick)
		HockeyStick->Destroy();
}
