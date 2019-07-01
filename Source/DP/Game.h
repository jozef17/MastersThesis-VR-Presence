// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game.generated.h"

UCLASS()
class DP_API AGame : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual void ResetGame() {};

	virtual FVector getPositionOffset(); 
	virtual FRotator getRotationOffset();

};
