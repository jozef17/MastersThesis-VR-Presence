// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Body.h"
#include "ModelBody.generated.h"

#define BODY_PARTS_COUNT 12		// Number of body parts

/**
 * 
 */
UCLASS()
class DP_API AModelBody : public ABody
{
	GENERATED_BODY()

private:
	UStaticMeshComponent * BodyParts[BODY_PARTS_COUNT];

public:
	static AModelBody *build(AActor *owner);

	AModelBody();

	virtual void hide() override;
	virtual void set() override;

	virtual void enable() override;
	virtual void disable() override;

	virtual void processKinectData(FVector *data) override;
	virtual void updateHands(FVector leftHandEnd, FVector rightHandEnd) override;

};
