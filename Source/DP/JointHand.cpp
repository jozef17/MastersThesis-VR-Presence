// Fill out your copyright notice in the Description page of Project Settings.

#include "JointHand.h"
#include "Engine.h"

// Endpoints of Each skeletal points connector 
const char HandConnectorsEnds[NUM_HAND_CONNECTORS][2] = {
	{  2,  3 },{  3,  4 },{  4,  5 },{  3,  7 },{  7,  8 },
	{  8,  9 },{  9, 10 },{  7, 12 },{ 12, 13 },{ 13, 14 },
	{ 14, 15 },{ 12, 17 },{ 17, 18 },{ 18, 19 },{ 19, 20 },
	{ 21, 22 },{ 17, 22 },{ 22, 23 },{ 23, 24 },{ 24, 25 }
};


AJointHand::AJointHand() : AHand()
{
	FString h = isLeft ? FString("LeftHand") : FString("RightHand");
	static ConstructorHelpers::FObjectFinder<UMaterial> Skin(TEXT("Material'/Game/DP/Materials/SkinMaterial.SkinMaterial'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		for (int i = 0; i < LEAP_TRACKED_POINTS - 2; i++)
		{
			FString id = h + FString::FromInt(i);
			HandsPoints[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			HandsPoints[i]->SetupAttachment(handRooot);
			HandsPoints[i]->SetStaticMesh(Sphere.Object);
			HandsPoints[i]->SetRelativeScale3D(HandPointsSize);
			HandsPoints[i]->SetMaterial(0, Skin.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
	{
		for (int i = 0; i < NUM_HAND_CONNECTORS; i++)
		{
			FString id = h + " Connector" + FString::FromInt(i);
			HandConnectors[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			HandConnectors[i]->SetupAttachment(handRooot);
			HandConnectors[i]->SetStaticMesh(Cylinder.Object);
			HandConnectors[i]->SetRelativeScale3D(HandPointsSize);
			HandConnectors[i]->SetMaterial(0, Skin.Object);
		}
	}
}

AJointHand * AJointHand::build(bool left, AActor *owner)
{
	AHand::left = left;
	AJointHand *hand = owner->GetWorld()->SpawnActor<AJointHand>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (hand != nullptr)
		hand->AttachRootComponentToActor(owner);
	return hand;
}

void AJointHand::hide()
{
	for (int i = 0; i < LEAP_TRACKED_POINTS - 2; i++) 
	{
		HandsPoints[i]->SetVisibility(false);
		HandsPoints[i]->SetRelativeLocation(FVector(0.0f));
	}
	for (int i = 0; i < NUM_HAND_CONNECTORS; i++)
	{
		HandConnectors[i]->SetVisibility(false);
		HandConnectors[i]->SetRelativeLocation(FVector(0.0f));
	}
}

void AJointHand::set()
{
	for (int i = 0; i < LEAP_TRACKED_POINTS - 2; i++)
		HandsPoints[i]->SetVisibility(true);
	for (int i = 0; i < NUM_HAND_CONNECTORS; i++)
		HandConnectors[i]->SetVisibility(true);
}

void AJointHand::enable()
{
	// TODO
}

void AJointHand::disable()
{
	// TODO
}

finger AJointHand::getFinger(void * ptr)
{
	// TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return NONE;
}

FVector AJointHand::getFingerLocation(finger f)
{
	return FVector();
}

void AJointHand::processLeapData(FVector * data, FRotator rotation)
{
	Super::processLeapData(data, rotation);
	handRooot->SetRelativeRotation(rotation);

	// Hand Points
	for (int i = 0; i < LEAP_TRACKED_POINTS - 2; i++)
		HandsPoints[i]->SetRelativeLocation(data[i + 2]);

	// Hand Connectors
	for (int i = 0; i < NUM_HAND_CONNECTORS; i++)
	{
		int a = HandConnectorsEnds[i][0];
		int b = HandConnectorsEnds[i][1];

		// Connectors height
		float z = FVector::Dist(data[a], data[b]) / 100.f;
		FVector size(HandPointsSize.X, HandPointsSize.Y, z);

		// Connectors center
		FVector location = (data[b] - data[a]) / 2.f + data[a];

		// Connectors Rotation
		FRotator rot = FRotationMatrix::MakeFromZ(data[a] - data[b]).Rotator();

		// Update Connector State
		HandConnectors[i]->SetRelativeScale3D(size);
		HandConnectors[i]->SetRelativeRotation(rot);
		HandConnectors[i]->SetRelativeLocation(location);
	}
}