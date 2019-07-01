// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Body.h"
#include "Const.h"
#include "JointBody.generated.h"

#define NUM_CONNECTORS 18		// number of connecting parts in mode 0


/**
 *
 */
UCLASS()
class DP_API AJointBody : public ABody
{
	GENERATED_BODY()

private:
	UStaticMeshComponent * Connetctors[NUM_CONNECTORS];
	UStaticMeshComponent *SkeletalPoints[KINECT_JOINTS_COUNT];

	FVector BodyPointsSize = FVector(0.08f, 0.08f, 0.08f);

public:
	static AJointBody *build(AActor *owner);

	AJointBody();

	virtual void hide() override;
	virtual void set() override;

	virtual void enable() override;
	virtual void disable() override;

	virtual void processKinectData(FVector *data) override;
	virtual void updateHands(FVector leftHandEnd, FVector rightHandEnd) override;
};
