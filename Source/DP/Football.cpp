// Fill out your copyright notice in the Description page of Project Settings.

#include "Football.h"

void AFootball::CreateGoal()
{
	// Football Goal
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GoalObj(TEXT("StaticMesh'/Game/DP/Models/Football/FootballGoal.FootballGoal'"));
	if (GoalObj.Succeeded())
	{
		Goal->SetStaticMesh(GoalObj.Object);

		GoalAreaBox->SetRelativeScale3D(FVector(6.2f, 0.985f, 3.075f));
		GoalAreaBox->SetRelativeLocation(FVector(0.0f, -29.5f, 100));
	}
	GoalRoot->SetRelativeLocation(FVector(0.0f, -300, 0));

	// Adjust Play Area Box
	PlayAreaBox->SetRelativeScale3D(FVector(9.4f, 11.0f, 4.0f));
	PlayAreaBox->SetRelativeLocation(FVector(0, -50, 128));
}

void AFootball::ProcessGoal(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::ProcessGoal(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (Goals == 6)
		PlayingObject->SetRelativeScale3D(FVector(0.18f));
	else if (Goals == 12)
		PlayingObject->SetRelativeScale3D(FVector(0.16f));
}

AFootball::AFootball() : AGoalGame()
{
	CreateGoal();
	CreateScoreBoard(FVector(-350, -400, 0), FRotator(0.0f, -36.0f, 0.0f)); 

	// Load Ball Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Ball(TEXT("StaticMesh'/Game/DP/Models/Football/Ball.Ball'"));
	if (Ball.Succeeded())
	{
		PlayingObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
		PlayingObject->SetupAttachment(RootComponent);
		PlayingObject->SetRelativeLocation(spawnPoint);
		PlayingObject->SetStaticMesh(Ball.Object);
		PlayingObject->SetSimulatePhysics(true);
		PlayingObject->SetRelativeScale3D(FVector(0.23f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> Grass(TEXT("Material'/Game/DP/Materials/Football/Grass.Grass'"));
	PlayArea->SetMaterial(0, Grass.Object);
}
