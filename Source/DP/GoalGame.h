// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game.h"
#include "Engine.h"
#include "GoalGame.generated.h"

/**
 *
 */
UCLASS()
class DP_API AGoalGame : public AGame
{
	GENERATED_BODY()

private:
	UTextRenderComponent *score;

	UFUNCTION()
	void Spawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void updateScore();

protected:
	int Goals = 0;

	FVector spawnPoint = FVector(0.0f, 100.0f, 20.0f);	// Where object will be spawned after goal or leaving play area

	UStaticMeshComponent *PlayingObject;	// Model of Playing object (eg. ball or puck)

	USceneComponent *GoalRoot;
	UStaticMeshComponent *Goal;				// Model of Goal
	UBoxComponent* GoalAreaBox;				// Area that represents "inside" of goal (whether Playing object is inside of goal)

	UStaticMeshComponent *PlayArea;			// Play Area
	UBoxComponent* PlayAreaBox;				// Inside of Play Area  

	void CreateScoreBoard(FVector location, FRotator rotation);

	UFUNCTION()
	virtual void ProcessGoal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	AGoalGame();

	virtual void ResetGame() override;
	virtual FVector getPositionOffset() override;
	virtual FRotator getRotationOffset() override;

	void resetPlayingObject();

};
