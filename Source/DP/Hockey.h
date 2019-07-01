// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoalGame.h"
#include "HockeyStickWithHelpers.h"
#include "Hockey.generated.h"

/**
*
*/
UCLASS()
class DP_API AHockey : public AGoalGame
{
	GENERATED_BODY()
		
private:
	bool left = true;
	AHockeyStick * HockeyStick = nullptr;

	void CreateGoal();
	void CreateStick();

protected:
	virtual void BeginPlay() override;

public:
	AHockey();

	void rotateHockeyStick();

	virtual void ResetGame() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
