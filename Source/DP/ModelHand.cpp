// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelHand.h"
#include "Engine.h"
#include "Const.h"

// Which index in Array returned by LeapBridge corespods to beginning of finger part
const char HandIndicies[HAND_PARTS_COUNT - 1] = { 3, 4, 7, 8 , 9, 12, 13, 14, 17, 18, 19, 22, 23, 24 };

// Heighs of each finger's part
const float HandPartsHeights[HAND_PARTS_COUNT - 1] = {
	4.00f, 3.00f, 4.20f, 4.50f, 3.60f, 4.40f, 4.40f, 4.30f, 4.10f, 4.00f, 4.75f, 4.16f, 2.90f, 3.00f
};



AModelHand::AModelHand() : AHand()
{
	static FString bodyPartsLocations[HAND_PARTS_COUNT][2] = {
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/Palm.Palm'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/Thumb1.Thumb1'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/Thumb2.Thumb2'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/IndexFinger1.IndexFinger1'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/IndexFinger2.IndexFinger2'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/IndexFinger3.IndexFinger3'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/MiddleFinger1.MiddleFinger1'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/MiddleFinger2.MiddleFinger2'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/MiddleFinger3.MiddleFinger3'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/RingFinger1.RingFinger1'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/RingFinger2.RingFinger2'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/RingFinger3.RingFinger3'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/LittleFinger1.LittleFinger1'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/LittleFinger2.LittleFinger2'") },
		{ FString("StaticMesh'/Game/DP/Models/Body/"), FString("/LittleFinger3.LittleFinger3'") },
	};

	static FString bodyNames[HAND_PARTS_COUNT] = {
		FString(" Palm"),
		FString(" Thumb 1"),
		FString(" Thumb 2"),
		FString(" Index Finger 1"),
		FString(" Index Finger 2"),
		FString(" Index Finger 3"),
		FString(" Middle Finger 1"),
		FString(" Middle Finger 2"),
		FString(" Middle Finger 3"),
		FString(" Ring Finger 1"),
		FString(" Ring Finger 2"),
		FString(" Ring Finger 3"),
		FString(" Little Finger 1"),
		FString(" Little Finger 2"),
		FString(" Little Finger 3"),
	};

	FString h = isLeft ? FString("LeftHand") : FString("RightHand");

	for (int i = 0; i < (HAND_PARTS_COUNT); i++)
	{
		FString obj = bodyPartsLocations[i][0] + h + bodyPartsLocations[i][1];
		ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(*obj);
		if (BodyMesh.Succeeded())
		{
			FString name = h + bodyNames[i];
			BodyParts[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*name));
			BodyParts[i]->SetStaticMesh(BodyMesh.Object);
			BodyParts[i]->SetupAttachment(handRooot);
		}
	}
}

AModelHand * AModelHand::build(bool left, AActor *owner)
{
	AHand::left = left;
	AModelHand *hand = owner->GetWorld()->SpawnActor<AModelHand>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (hand != nullptr)
		hand->AttachRootComponentToActor(owner);
	return hand;
}

void AModelHand::hide()
{
	for (int i = 0; i < (HAND_PARTS_COUNT); i++)
	{
		BodyParts[i]->SetVisibility(false);
		BodyParts[i]->SetRelativeLocation(FVector(0.0f));
	}
}

void AModelHand::set()
{
	for (int i = 0; i < (HAND_PARTS_COUNT); i++)
		BodyParts[i]->SetVisibility(true);
}

void AModelHand::enable()
{
	// TODO
}

void AModelHand::disable()
{
	// TODO
}

finger AModelHand::getFinger(void * ptr)
{
	for (int i = 1; i < HAND_PARTS_COUNT; i++)
	{
		if (i == HAND_PARTS_COUNT)
			continue;

		if (BodyParts[i] == ptr)
		{
			if (i < 3)
				return isLeft ? LEFT_THUMB : RIGHT_THUMB;
			else if (i < 6)
				return isLeft ? LEFT_INDEX_FINGER : RIGHT_INDEX_FINGER;
			else if (i < 9)
				return isLeft ? LEFT_MIDDLE_FINGER : RIGHT_MIDDLE_FINGER;
			else if (i < 12)
				return isLeft ? LEFT_RING_FINGER : RIGHT_RING_FINGER;
			else
				return isLeft ? LEFT_LITTLE_FINGER : RIGHT_LITTLE_FINGER;
		}
	}
	return NONE;
}

FVector AModelHand::getFingerLocation(finger f)
{
	return FVector();
}

void AModelHand::processLeapData(FVector *data, FRotator rotation)
{
	Super::processLeapData(data, rotation);
	handRooot->SetRelativeRotation(rotation);

	for (int i = 0; i < HAND_PARTS_COUNT - 1; i++)
	{
		BodyParts[i + 1]->SetRelativeLocation(data[HandIndicies[i]]);
		BodyParts[i + 1]->SetRelativeRotation(FRotationMatrix::MakeFromZ(data[HandIndicies[i]] - data[HandIndicies[i] + 1]).Rotator());

		float ratio = FVector::Dist(data[HandIndicies[i]], data[HandIndicies[i] + 1]) / HandPartsHeights[i];

		BodyParts[i + 1]->SetRelativeScale3D(FVector(1, 1, ratio + 0.2f));
	}

	/*
	FVector a = data[7];
	FVector b = data[22];
	a.Normalize();
	b.Normalize();

	FVector normal = FVector::CrossProduct(a, b);
	normal.Normalize();
	FVector x = FVector(1, 0, 0);
	x.Normalize();

	FVector axis = FVector::CrossProduct(x, normal);
	float angle = FGenericPlatformMath::Acos(FVector::DotProduct(x, normal));
	//	float angle = FVector::DotProduct(x, normal);
	FQuat quat = FQuat(axis, angle);

	FVector h = FVector::CrossProduct(FVector(1, 0, 0), FVector(0, 0, 1));
	FVector i = FVector::CrossProduct(FVector(0, 0, 1), FVector(1, 0, 0));

	FRotator rot1 = FQuat(FVector(0, 1, 0), FGenericPlatformMath::Acos(FVector::DotProduct(FVector(1, 0, 0), FVector(0, 0, 1)))).Rotator();
	FRotator rot2 = FQuat(FVector(0, -1, 0), FGenericPlatformMath::Acos(FVector::DotProduct(FVector(1, 0, 0), FVector(0, 0, 1)))).Rotator();


	float ratio = FVector::Dist(FVector(0), data[12]) / 3.0f;
	BodyParts[0]->SetRelativeScale3D(FVector(0.6, 1, ratio));
	BodyParts[0]->SetRelativeRotation(quat.Rotator()); // */

	BodyParts[0]->SetVisibility(false);
	BodyParts[0]->SetRelativeLocation(FVector(1000));
	BodyParts[0]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
