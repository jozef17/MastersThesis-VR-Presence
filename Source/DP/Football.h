// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalGame.h"
#include "Football.generated.h"

/**
*
*/
UCLASS()
class DP_API AFootball : public AGoalGame
{
	GENERATED_BODY()

private:
	void CreateGoal();

protected:
	void ProcessGoal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	AFootball();

};
