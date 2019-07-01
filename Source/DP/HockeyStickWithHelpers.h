// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HockeyStick.h"
#include "HockeyStickWithHelpers.generated.h"

/**
 * 
 */
UCLASS()
class DP_API AHockeyStickWithHelpers : public AHockeyStick
{
	GENERATED_BODY()
	
private:
	UStaticMeshComponent * areas[2];
	
public:
	AHockeyStickWithHelpers();

};
