// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hand.h"
#include "ModelHand.generated.h"

#define HAND_PARTS_COUNT 15		// Number of hand parts (one hand)

/**
 *
 */
UCLASS()
class DP_API AModelHand : public AHand
{
	GENERATED_BODY()

private:
	UStaticMeshComponent * BodyParts[HAND_PARTS_COUNT];

public:
	AModelHand();

	static AModelHand *build(bool left, AActor *owner);

	virtual void hide() override;
	virtual void set() override;

	virtual void enable() override;
	virtual void disable() override;

	virtual finger getFinger(void *ptr) override;
	virtual FVector getFingerLocation(finger f) override;
	virtual void processLeapData(FVector *data, FRotator rotation) override;

};
