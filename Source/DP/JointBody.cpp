// Fill out your copyright notice in the Description page of Project Settings.

#include "JointBody.h"
#include "Engine.h"

// Endpoints of Each skeletal points connector 
//const char ConnectorsEnds[NUM_CONNECTORS][2] = {
//{ 0,  1 },{ 1, 20 },{ 20,  2 },{ 2,  3 },{ 20,  4 },{ 20,  8 },
//{ 8,  9 },{ 9, 10 },{ 4,  5 },{ 5,  6 },{ 0, 12 },{ 0, 16 },
//{ 12, 13 },{ 13, 14 },{ 14, 15 },{ 16, 17 },{ 17, 18 },{ 18, 19 } };

const char ConnectorsEnds[NUM_CONNECTORS][2] = {
	{ 0,  1 },{ 1, 20 },{ 20,  2 },{ 2,  3 },{ 20,  4 },{ 20,  8 },
	{ 8,  9 },{ 4,  5 },{ 0, 12 },{ 0, 16 },{ 12, 13 },{ 13, 14 },
	{ 14, 15 },{ 16, 17 },{ 17, 18 },{ 18, 19 },{ 9, 10 },{ 5,  6 } };



AJointBody * AJointBody::build(AActor *owner)
{
	AJointBody *body = owner->GetWorld()->SpawnActor<AJointBody>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (body != nullptr)
		body->AttachRootComponentToActor(owner);
	return body;
}

AJointBody::AJointBody() : ABody()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Skin(TEXT("Material'/Game/DP/Materials/SkinMaterial.SkinMaterial'"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		for (int i = 0; i < KINECT_JOINTS_COUNT; i++)
		{
			SKIP_HANDS(i);

			FString id = "Joint" + FString::FromInt(i);
			SkeletalPoints[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			SkeletalPoints[i]->SetupAttachment(RootComponent);
			SkeletalPoints[i]->SetStaticMesh(Sphere.Object);
			SkeletalPoints[i]->SetRelativeScale3D(BodyPointsSize);
			SkeletalPoints[i]->SetMaterial(0, Skin.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
	{
		// Body
		for (int i = 0; i < NUM_CONNECTORS; i++)
		{
			FString id = "Connector" + FString::FromInt(i);
			Connetctors[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			Connetctors[i]->SetupAttachment(RootComponent);
			Connetctors[i]->SetStaticMesh(Cylinder.Object);
			Connetctors[i]->SetRelativeScale3D(BodyPointsSize);
			Connetctors[i]->SetMaterial(0, Skin.Object);

		}
	}

	SkeletalPoints[2]->SetOwnerNoSee(true);
	SkeletalPoints[3]->SetOwnerNoSee(true);
	Connetctors[2]->SetOwnerNoSee(true);
	Connetctors[3]->SetOwnerNoSee(true);
}

void AJointBody::hide()
{
	for (int i = 0; i < KINECT_JOINTS_COUNT; i++)
	{
		SKIP_HANDS(i);
		SkeletalPoints[i]->SetVisibility(false);
		SkeletalPoints[i]->SetRelativeLocation(FVector(0.0f));
	}

	for (int i = 0; i < NUM_CONNECTORS; i++)
	{
		Connetctors[i]->SetVisibility(false);
		Connetctors[i]->SetRelativeLocation(FVector(0.0f));
	}
}

void AJointBody::set()
{
	for (int i = 0; i < KINECT_JOINTS_COUNT; i++)
	{
		SKIP_HANDS(i);
		SkeletalPoints[i]->SetVisibility(true);
	}

	for (int i = 0; i < NUM_CONNECTORS; i++)
		Connetctors[i]->SetVisibility(true);
}

void AJointBody::enable()
{
	// TODO
}

void AJointBody::disable()
{
	// TODO
}

void AJointBody::processKinectData(FVector * data)
{
	// Body Points
	for (int i = 0; i < KINECT_JOINTS_COUNT; i++) {
		SKIP_HANDS(i);
		if (i == 6 || i == 10) continue;
		SkeletalPoints[i]->SetRelativeLocation(data[i]);
	}

	// Body Connectors
	for (int i = 0; i < NUM_CONNECTORS - 2; i++)
	{
		int a = ConnectorsEnds[i][0];
		int b = ConnectorsEnds[i][1];

		// Connectors height
		float z = FVector::Dist(data[a], data[b]) / 100.f;
		FVector connetctorSize(BodyPointsSize.X, BodyPointsSize.Y, z);

		// Connectors center
		FVector connetctorsLocation = (data[b] - data[a]) / 2.f + data[a];

		// Connectors Rotation
		FRotator Rot = FRotationMatrix::MakeFromZ(data[a] - data[b]).Rotator();

		// Update Connector State
		Connetctors[i]->SetRelativeScale3D(connetctorSize);
		Connetctors[i]->SetRelativeRotation(Rot);
		Connetctors[i]->SetRelativeLocation(connetctorsLocation);
	}
}

void AJointBody::updateHands(FVector leftHandEnd, FVector rightHandEnd)
{
	FVector l = SkeletalPoints[6]->GetComponentLocation();
	FVector r = SkeletalPoints[10]->GetComponentLocation();



	float z1 = FVector::Dist(l, leftHandEnd) / 100.f;
	float z2 = FVector::Dist(r, rightHandEnd) / 100.f;

	Connetctors[NUM_CONNECTORS - 2]->SetRelativeScale3D(FVector(BodyPointsSize.X, BodyPointsSize.Y, z1)); //L
	Connetctors[NUM_CONNECTORS - 1]->SetRelativeScale3D(FVector(BodyPointsSize.X, BodyPointsSize.Y, z2)); //R

	Connetctors[NUM_CONNECTORS - 2]->SetRelativeLocation(((leftHandEnd - l) / 2.f + l) - GetActorLocation());
	Connetctors[NUM_CONNECTORS - 1]->SetRelativeLocation(((rightHandEnd - r) / 2.f + r) - GetActorLocation());

	Connetctors[NUM_CONNECTORS - 2]->SetRelativeRotation(FRotationMatrix::MakeFromZ(l - leftHandEnd).Rotator() - GetActorRotation());
	Connetctors[NUM_CONNECTORS - 1]->SetRelativeRotation(FRotationMatrix::MakeFromZ(r - rightHandEnd).Rotator() - GetActorRotation());

	SkeletalPoints[6]->SetRelativeLocation(GetActorLocation() - leftHandEnd);
	SkeletalPoints[10]->SetRelativeLocation(GetActorLocation() - rightHandEnd);
}
