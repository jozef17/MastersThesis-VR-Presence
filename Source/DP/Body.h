// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Body.generated.h"

UCLASS()
class DP_API ABody : public AActor
{
	GENERATED_BODY()
	
public:	
	static ABody *build(AActor *owner);

	// Sets default values for this actor's properties
	ABody();

	virtual void hide() {};
	virtual void set() {};

	virtual void enable() {};
	virtual void disable() {};

	virtual void processKinectData(FVector *data) {};
	virtual void updateHands(FVector leftHandEnd, FVector rightHandEnd) {};
};
