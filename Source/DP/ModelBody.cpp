// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBody.h"
#include "Engine.h"

// Index of Kinect Joint that represents body part location
const char BodyPartsPositionsMapping[BODY_PARTS_COUNT]{
	12, 16, 13, 17, 14, 18, 4, 8, 0, 20, 5, 9
};

// Indicies of Kineect Joints between which is body part placed
const char BodyEnds[BODY_PARTS_COUNT - 4][2] = {
	{ 12, 13 }, { 16, 17 }, { 13, 14 }, { 17, 18 },
	{ 14, 15 },	{ 18, 19 }, {  4,  5 }, {  8,  9 }
};

const float BodyPartsHeights[BODY_PARTS_COUNT]{
	55.0f, 55.0f, 45.0f, 45.0f, 20.0f, 20.0f,
	22.0f, 22.0f, 31.5f, 49.0f, 31.5f, 31.5f
};


AModelBody * AModelBody::build(AActor *owner)
{
	AModelBody *body = owner->GetWorld()->SpawnActor<AModelBody>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (body != nullptr)
		body->AttachRootComponentToActor(owner);
	return body;
}

AModelBody::AModelBody() : ABody()
{
	static FString bodyPartsLocations[BODY_PARTS_COUNT] = {
		FString("StaticMesh'/Game/DP/Models/Body/ThightLeft.ThightLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/ThightRight.ThightRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/CalfLeft.CalfLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/CalfRight.CalfRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/FootLeft.FootLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/FootRight.FootRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/UpperArmLeft.UpperArmLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/UpperArmRight.UpperArmRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/SpineBase.SpineBase'"),
		FString("StaticMesh'/Game/DP/Models/Body/Chest.Chest'"),
		FString("StaticMesh'/Game/DP/Models/Body/LowerArmLeft.LowerArmLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/LowerArmRight.LowerArmRight'"),
	};

	static FName bodyNames[BODY_PARTS_COUNT] = {
		FName("Thight Left"),
		FName("Thight Right"),
		FName("Calf Left"),
		FName("Calf Right"),
		FName("Foot Left"),
		FName("Foot Right"),
		FName("Upper Arm Left"),
		FName("Upper Arm Right"),
		FName("Spine Base"),
		FName("Chest"),
		FName("Lower Arm Left"),
		FName("Lower Arm Right"),
	};

	for (int i = 0; i < (BODY_PARTS_COUNT); i++)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(*bodyPartsLocations[i]);
		if (BodyMesh.Succeeded())
		{
			BodyParts[i] = CreateDefaultSubobject<UStaticMeshComponent>(bodyNames[i]);
			BodyParts[i]->SetStaticMesh(BodyMesh.Object);
			BodyParts[i]->SetupAttachment(RootComponent);
		}
	}
}

void AModelBody::hide()
{
	for (int i = 0; i < BODY_PARTS_COUNT; i++)
	{
		BodyParts[i]->SetVisibility(false);
		BodyParts[i]->SetRelativeLocation(FVector(0.0f));
	}
}

void AModelBody::set()
{
	for (int i = 0; i < BODY_PARTS_COUNT; i++)
		BodyParts[i]->SetVisibility(true);
}

void AModelBody::enable()
{
	// TODO
}

void AModelBody::disable()
{
	// TODO
}

void AModelBody::processKinectData(FVector * data)
{
	for (int i = 0; i < BODY_PARTS_COUNT; i++)
		BodyParts[i]->SetRelativeLocation(data[BodyPartsPositionsMapping[i]]);

	for (int i = 0; i < BODY_PARTS_COUNT - 4; i++)
	{
		BodyParts[i]->SetRelativeRotation(FRotationMatrix::MakeFromZ(data[BodyEnds[i][0]] - data[BodyEnds[i][1]]).Rotator());
		float ratio = FVector::Dist(data[BodyEnds[i][0]], data[BodyEnds[i][1]]) / BodyPartsHeights[i];
		BodyParts[i]->SetRelativeScale3D(FVector(0.85f, 0.85f, ratio));
	}

	// TODO	// 0
	BodyParts[8]->SetRelativeLocation((data[12] + data[16]) / 2.f);
	BodyParts[8]->SetRelativeRotation(FRotator(0, 0, 0));
	float dist = FVector::Dist(data[12], data[16]) / 20;
	BodyParts[8]->SetRelativeScale3D(FVector(0.85f, dist, 0.85f));

	// TODO	// 7
	FVector scale;
	BodyParts[9]->SetRelativeRotation(FRotator(0, 0, 0));
	scale.Y = FVector::Dist(data[4], data[8]) / 40.f;
	scale.Z = FVector::Dist(data[1], data[20]) / 20.f;
	scale.X = (scale.Y + scale.Z) / 3;
	BodyParts[9]->SetRelativeScale3D(scale);
}

void AModelBody::updateHands(FVector leftHandEnd, FVector rightHandEnd)
{
	BodyParts[10]->SetRelativeRotation(FRotationMatrix::MakeFromZ(BodyParts[10]->GetComponentLocation() - leftHandEnd).Rotator() - GetActorRotation());
	float ratio = FVector::Dist(BodyParts[10]->GetComponentLocation(), leftHandEnd) / 31.5f;
	BodyParts[10]->SetRelativeScale3D(FVector(0.85f, 0.85f, ratio - 0.2f));

	BodyParts[11]->SetRelativeRotation(FRotationMatrix::MakeFromZ(BodyParts[11]->GetComponentLocation() - rightHandEnd).Rotator() - GetActorRotation());
	ratio = FVector::Dist(BodyParts[11]->GetComponentLocation(), rightHandEnd) / 31.5f;
	BodyParts[11]->SetRelativeScale3D(FVector(0.85f, 0.85f, ratio - 0.2f));
}
