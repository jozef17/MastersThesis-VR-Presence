// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Const.h"
#include "Hand.h"
#include "JointHand.generated.h"

#define NUM_HAND_CONNECTORS 20

/**
 *
 */
UCLASS()
class DP_API AJointHand : public AHand
{
	GENERATED_BODY()

private:
	UStaticMeshComponent *HandsPoints[LEAP_TRACKED_POINTS - 2];
	UStaticMeshComponent *HandConnectors[NUM_HAND_CONNECTORS];

	FVector HandPointsSize = FVector(0.02f);
public:
	AJointHand();

	static AJointHand *build(bool left, AActor *owner);

	virtual void hide() override;
	virtual void set() override;

	virtual void enable() override;
	virtual void disable() override;

	virtual finger getFinger(void *ptr) override;
	virtual FVector getFingerLocation(finger f) override;
	virtual void processLeapData(FVector *data, FRotator rotation) override;

};
