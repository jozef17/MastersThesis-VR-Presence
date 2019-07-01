// Fill out your copyright notice in the Description page of Project Settings.

#include "UserRepresentation.h"
#include "DPCharacter.h"
#include "Const.h"

void AUserRepresentation::setup()
{
	body = ABody::build(this);
	leftHand = AHand::build(true, this);
	rightHand = AHand::build(false, this);
}

// Sets default values
AUserRepresentation::AUserRepresentation()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	kinect = KinectBridge::getInstance();
	leap = LeapBridge::getInstance();

	for (int i = 0; i < KINECT_JOINTS_COUNT; i++)
		current[i] = FVector(0.0f);
}

// Called when the game starts or when spawned
void AUserRepresentation::BeginPlay()
{
	Super::BeginPlay();
	setup();
	set();
}

void AUserRepresentation::setCharacter(APawn * character)
{
	if (dynamic_cast<ADPCharacter *>(character))
		this->character = character;
}

void AUserRepresentation::hide()
{
	isHidden = true;

	if (leftHand)
		leftHand->hide();
	if (rightHand)
		rightHand->hide();
	if (body)
		body->hide();
}

void AUserRepresentation::set()
{
	isHidden = false;

	if (leftHand)
		leftHand->set();
	if (rightHand)
		rightHand->set();
	if (body)
		body->set();
}

void AUserRepresentation::enable()
{
	if (!disabled)
		return;
	disabled = false;

	if (leftHand)
		leftHand->enable();
	if (rightHand)
		rightHand->enable();
	if (body)
		body->enable();
}

void AUserRepresentation::disable()
{
	if (disabled)
		return;
	disabled = true;

	if (leftHand)
		leftHand->disable();
	if (rightHand)
		rightHand->disable();
	if (body)
		body->disable();
}

// Called every frame
void AUserRepresentation::Tick(float DeltaTime)
{
	if (isHidden == true || kinect == nullptr || leap == nullptr)
		return;
	counter++;

	// Get Camera Location & rotation
	FRotator cameraRot = FRotator(0.0f, 0.0f, 0.0f);
	FVector cameraLoc = FVector(0.0f);

	if (character)
	{
		cameraRot = ((ADPCharacter *)character)->GetCameraRotation();
		cameraLoc = ((ADPCharacter *)character)->GetCameraLocation();
	}

	// 1. Process Leap Data
	FVector *leftHandData = leap->getLoactions(true, true);
	FVector *rightHandData = leap->getLoactions(false, true);

	// Check if data given by Leap represents given hands
	if (leftHandData && rightHandData)
	{
		FVector leapL = cameraLoc + cameraRot.RotateVector(leftHandData[0]);
		FVector leapR = cameraLoc + cameraRot.RotateVector(rightHandData[0]);

		float LLdist = FVector::DistSquared(leapL, current[7]);
		float LRdist = FVector::DistSquared(leapL, current[11]);
		float RLdist = FVector::DistSquared(leapR, current[7]);
		float RRdist = FVector::DistSquared(leapR, current[11]);

		if ((LRdist < LLdist) && (RLdist < RRdist))
		{
			FVector *tmp = leftHandData;
			leftHandData = rightHandData;
			rightHandData = tmp;
		}
		counter = 0;
	}
	else if (leftHandData)
	{
		FVector leapL = cameraLoc + cameraRot.RotateVector(leftHandData[0]);
		float Ldist = FVector::DistSquared(leapL, current[7]);
		float Rdist = FVector::DistSquared(leapL, current[11]);

		if (Rdist < Ldist)
		{
			rightHandData = leftHandData;
			leftHandData = nullptr;
		}
		counter = 0;
	}
	else if (rightHandData)
	{
		FVector leapR = cameraLoc + cameraRot.RotateVector(rightHandData[0]);
		float Ldist = FVector::DistSquared(leapR, current[7]);
		float Rdist = FVector::DistSquared(leapR, current[11]);

		if (Ldist < Rdist)
		{
			leftHandData = rightHandData;
			rightHandData = nullptr;
		}
		counter = 0;
	}

	// Process Left Hand
	if (leftHand && leftHandData)
	{
		leftHand->processLeapData(leftHandData, cameraRot);
		leapHands[0] = leftHandData[0] + FVector(5, 0, 0);
	}

	// Process Right Hand
	if (rightHand && rightHandData)
	{
		rightHand->processLeapData(rightHandData, cameraRot);
		leapHands[1] = rightHandData[0] + FVector(5, 0, 0);
	}

	// 2. Process Kinect Data
	// Get positionf of body joints from Kinect
	FVector *kinectData = kinect->getLoactions();

	if (kinectData && body)
	{
		float Zoffsef = -(kinectData[kinect->lastMinZ()].Z + 98);

		// Move everithing up
		for (int i = 0; i < KINECT_JOINTS_COUNT; i++)
		{
			kinectData[i].Z += Zoffsef;

			if (FVector::DistSquared(kinectData[i], current[i]) >= KINECT_TRESHOLD)
				current[i] = kinectData[i];
		}
		body->processKinectData(current);

		if (character && kinectData)
			((ADPCharacter *)character)->SetCameraLocation(current[3] + FVector(15, 0, 0));
	}

	// 3. Set Hands location
	if (leftHand && leftHandData)
		leftHand->SetActorLocation(GetActorLocation() + GetActorRotation().RotateVector(cameraLoc + cameraRot.RotateVector(leapHands[0])), false);
	else if ((leftHand && rightHandData) || (kinectData && (counter > 10)))
		leftHand->SetActorLocation(GetActorLocation() + GetActorRotation().RotateVector(current[7]), false);

	if (rightHand && rightHandData)
		rightHand->SetActorLocation(GetActorLocation() + GetActorRotation().RotateVector(cameraLoc + cameraRot.RotateVector(leapHands[1])), false);
	else if ((rightHand && leftHandData) || (kinectData && (counter > 10)))
		rightHand->SetActorLocation(GetActorLocation() + GetActorRotation().RotateVector(current[11]), false);

	// 4. Sync Kinect & Leap Hands
	if (body && leftHand && rightHand)
		body->updateHands(leftHand->GetActorLocation(), rightHand->GetActorLocation());
}

void AUserRepresentation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (leftHand)
		leftHand->Destroy();
	if (rightHand)
		rightHand->Destroy();
	if (body)
		body->Destroy();
}
