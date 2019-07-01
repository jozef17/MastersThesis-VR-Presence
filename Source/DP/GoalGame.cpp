// Fill out your copyright notice in the Description page of Project Settings.

#include "GoalGame.h"


void AGoalGame::ProcessGoal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsPendingKill())
		return;

	if ((OtherActor == this) && (OtherComp == PlayingObject))
	{
		PlayingObject->SetWorldLocation(GetActorLocation() + GetActorRotation().RotateVector(spawnPoint));
		PlayingObject->SetPhysicsAngularVelocity(FVector(0));
		PlayingObject->SetPhysicsLinearVelocity(FVector(0));
		Goals++;
		updateScore();
	}
}

void AGoalGame::Spawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsPendingKill())
		return;

	if ((OtherActor == this) && (OtherComp == PlayingObject))
	{
		PlayingObject->SetWorldLocation(GetActorLocation() + GetActorRotation().RotateVector(spawnPoint));
		PlayingObject->SetPhysicsAngularVelocity(FVector(0));
		PlayingObject->SetPhysicsLinearVelocity(FVector(0));
	}
}



void AGoalGame::updateScore()
{
	FString text;
	if (Goals < 20)
		text = FString("0") + FString::FromInt(Goals / 2);
	else
		text = FString::FromInt(Goals / 2);

	score->SetText(FText::FromString(text));
}

void AGoalGame::CreateScoreBoard(FVector location, FRotator rotation)
{
	USceneComponent *BoardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Board Root"));
	BoardRoot->SetupAttachment(RootComponent);
	BoardRoot->SetRelativeLocation(location);
	BoardRoot->SetRelativeRotation(rotation);

	// Create Skeleton material
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/DP/Materials/MetalScuffs_Green.MetalScuffs_Green'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BoardMaterial(TEXT("Material'/Game/DP/Materials/BlackMaterial.BlackMaterial'"));

	// Create Skeleton for Score Board
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
		for (int i = 0; i < 2; i++)
		{
			FString name = FString("Skeleton") + FString::FromInt(i);
			UStaticMeshComponent *comp = CreateDefaultSubobject<UStaticMeshComponent>(FName(*name));
			comp->SetupAttachment(BoardRoot);
			comp->SetStaticMesh(Cylinder.Object);
			comp->SetRelativeLocation(FVector(-50.0f + i * 100.0f, 0, 75.0f));
			comp->SetRelativeScale3D(FVector(0.1f, 0.1f, 1.7f));
			comp->SetMaterial(0, Material.Object);
			comp->SetRelativeRotation(FRotator(0.0f, i * 75.0f, 0.0f));
		}

	// Create Board
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		UStaticMeshComponent *comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
		comp->SetupAttachment(BoardRoot);
		comp->SetStaticMesh(Cube.Object);
		comp->SetRelativeLocation(FVector(0, 0, 150.0f));
		comp->SetRelativeScale3D(FVector(2.0f, 0.1f, 1.0f));
		comp->SetMaterial(0, BoardMaterial.Object);
	}

	// Create Board Text - number of goals
	score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Score Text"));
	score->SetupAttachment(BoardRoot);
	score->SetRelativeLocation(FVector(-50.0f, 7.0f, 93.0f));
	score->SetRelativeScale3D(FVector(4.0f));
	score->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	updateScore();
}

AGoalGame::AGoalGame()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Goal Game Root"));

	// Goal
	GoalRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Goal Root"));
	GoalRoot->SetupAttachment(RootComponent);

	GoalAreaBox = CreateAbstractDefaultSubobject<UBoxComponent>(TEXT("Goal Area Cube"));
	GoalAreaBox->SetupAttachment(GoalRoot);
	GoalAreaBox->bGenerateOverlapEvents = true;
	GoalAreaBox->OnComponentBeginOverlap.AddDynamic(this, &AGoalGame::ProcessGoal);

	Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal"));
	Goal->SetupAttachment(GoalRoot);

	// Play Area
	PlayAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Play Area Cube"));
	PlayAreaBox->SetupAttachment(RootComponent);
	PlayAreaBox->SetRelativeScale3D(FVector(9.4f, 9.40f, 4.0f));
	PlayAreaBox->SetRelativeLocation(FVector(0, 0, 128));

	PlayAreaBox->bGenerateOverlapEvents = true;
	PlayAreaBox->OnComponentEndOverlap.AddDynamic(this, &AGoalGame::Spawn);

	// Ground
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Ground(TEXT("StaticMesh'/Game/DP/Models/MoveArea.MoveArea'"));
	if (Ground.Succeeded())
	{
		PlayArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Play Area"));
		PlayArea->SetStaticMesh(Ground.Object);
		PlayArea->SetupAttachment(RootComponent);
		PlayArea->SetRelativeScale3D(FVector(1.0f, 1.0f, 8.0f));
		PlayArea->SetRelativeLocation(FVector(0.0f, 0.0f, -3.5f));
	}
}

void AGoalGame::ResetGame()
{
	Goals = 0;
	updateScore();

	if (PlayingObject != nullptr)
		PlayingObject->SetWorldLocation(GetActorLocation() + GetActorRotation().RotateVector(spawnPoint));
}

FVector AGoalGame::getPositionOffset()
{
	FVector offset = spawnPoint;
	offset.Z = 0;
	offset.Y -= 120;

	return offset;
}

FRotator AGoalGame::getRotationOffset()
{
	return FRotator(0.0f, -90.0f, 0.0f);
}

void AGoalGame::resetPlayingObject()
{
	PlayingObject->SetRelativeLocation(spawnPoint);
}
