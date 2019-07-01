// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KinectBridge.h"
#include "LeapBridge.h"
#include "Body.h"
#include "Hand.h"
#include "UserRepresentation.generated.h"

UCLASS()
class DP_API AUserRepresentation : public AActor
{
	GENERATED_BODY()

private:
	unsigned long counter = 0;

	FVector current[KINECT_JOINTS_COUNT];
	FVector leapHands[2];
	APawn * character = nullptr;

	KinectBridge * kinect = nullptr;
	LeapBridge *leap = nullptr;

protected:
	AHand *leftHand = nullptr;
	AHand *rightHand = nullptr;
	ABody *body = nullptr;

	bool isHidden = false;
	bool disabled = false;

	virtual void setup();	// Sets up Hands & Body

public:	
	AUserRepresentation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void setCharacter(APawn * character);

	virtual void hide();
	virtual void set();

	virtual void enable();
	virtual void disable();

	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
