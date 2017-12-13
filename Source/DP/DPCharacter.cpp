// Fill out your copyright notice in the Description page of Project Settings.

#include "DPCharacter.h"
#include "Const.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Macro SKIP0s kinect data that are not used for displaying - neck, hands, thumb, ...
#define SKIP0(i) if(i == 2 || i == 3) continue;
#define SKIP1(i) if(i == 0 || i == 7) continue;
#define DISTANCE(v1, v2) sqrt( (v1.X - v2.X)*(v1.X - v2.X) + (v1.Y - v2.Y)*(v1.Y - v2.Y) + (v1.Z - v2.Z)*(v1.Z - v2.Z))

// Endpoints of Each skeletal points connector 
char ConnectorsEnds[NUM_CONNECTORS][2] = {
	{ 0,  1}, { 1, 20}, {20,  4}, {20,  8}, { 8,  9}, { 9, 10},
	{10, 11}, {10, 24}, {11, 23}, { 4,  5}, { 5,  6}, { 6,  7}, { 7, 21}, { 6, 22},
	{ 0, 12}, { 0, 16}, {12, 13}, {13, 14}, {14, 15}, {16, 17}, {17, 18}, {18, 19} };



// Index of Kinect Joint that represents body part location
char BodyPartsPositionsMapping[BODY_PARTS_COUNT]{
	0, 12, 16, 13, 17, 14, 18, 20, 4, 8, 5, 9, 6, 10 
};

// Indicies of Kineect Joints between which is body part placed
char BodyEnds[BODY_PARTS_COUNT][2] = {
	{ 1, 0}, {12, 13},{16, 17}, {13, 14}, {17, 18}, {14, 15}, {18, 19},
	{20, 1}, { 4,  5},{ 8,  9}, { 5,  6}, { 9, 10}, { 6,  7}, {10, 11}
};

float BodyPartsHeights[BODY_PARTS_COUNT]{
	31.5f, 55.0f, 55.0f, 45.0f, 45.0f, 20.0f, 20.0f, 
	49.0f, 22.0f, 22.0f, 31.5f, 31.5f, 26.0f, 26.0f
};



ADPCharacter::ADPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	kinect = KinectBridge::getInstance();
	leap = LeapBridge::getInstance();

	// Init Character Modes
	initMode0();
	initMode1();
	GetMesh()->SetVisibility(false);

	// Message when user is too close to the Kinect
	message = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Warning Message"));
	message->SetText(TEXT("MOVE BACK"));
	message->SetupAttachment(GetCapsuleComponent());

	HandRoot[0] = CreateDefaultSubobject<USceneComponent>(TEXT("Left Hand Root"));
	HandRoot[1] = CreateDefaultSubobject<USceneComponent>(TEXT("Right Hand Root"));
	HandRoot[0]->SetupAttachment(GetCapsuleComponent());
	HandRoot[1]->SetupAttachment(GetCapsuleComponent());

	// Create & Setup Camera.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 0;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADPCharacter::BeginPlay()
{
	Super::BeginPlay();
	kinect->connect();
	
	// Set Material to components
	for (int i = 0; i < NUM_JOINTS; i++)
	{
		SKIP0(i);
		SkeletalPoints[i]->SetMaterial(0, BodyMaterial);
	}

	for (int i = 0; i < NUM_CONNECTORS; i++)
		Connetctors[i]->SetMaterial(0, BodyMaterial);

	for (int i = 0; i < BODY_PARTS_COUNT + HAND_PARTS_COUNT; i++)
		BodyParts[i]->SetMaterial(0, BodyMaterial);

	// Set Mode
	mode--;
	ChangeUserRepresentation();
}

void ADPCharacter::initMode0()
{
#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DPCharacter mode 0 Initialized"));
#endif

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
		for (int i = 0; i < NUM_JOINTS; i++)
		{
			SKIP0(i);

			FString id = "Joint" + FString::FromInt(i);
			SkeletalPoints[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			SkeletalPoints[i]->SetupAttachment(GetCapsuleComponent());

			SkeletalPoints[i]->SetStaticMesh(Sphere.Object);
			SkeletalPoints[i]->SetRelativeScale3D(SkeletalPointsSize);
		}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
		for (int i = 0; i < NUM_CONNECTORS; i++)
		{
			FString id = "Connector" + FString::FromInt(i);
			Connetctors[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			Connetctors[i]->SetupAttachment(GetCapsuleComponent());

			Connetctors[i]->SetStaticMesh(Cylinder.Object);
			Connetctors[i]->SetRelativeScale3D(SkeletalPointsSize);
		}
}

void ADPCharacter::initMode1()
{
#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DPCharacter mode 1 Initialized"));
#endif

	static FString bodyPartsLocations[BODY_PARTS_COUNT + HAND_PARTS_COUNT] = {
		FString("StaticMesh'/Game/DP/Models/Body/SpineBase.SpineBase'"),
		FString("StaticMesh'/Game/DP/Models/Body/ThightLeft.ThightLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/ThightRight.ThightRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/CalfLeft.CalfLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/CalfRight.CalfRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/FootLeft.FootLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/FootRight.FootRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/Chest.Chest'"),
		FString("StaticMesh'/Game/DP/Models/Body/UpperArmLeft.UpperArmLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/UpperArmRight.UpperArmRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/LowerArmLeft.LowerArmLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/LowerArmRight.LowerArmRight'"),
		FString("StaticMesh'/Game/DP/Models/Body/HandLeft.HandLeft'"),
		FString("StaticMesh'/Game/DP/Models/Body/HandRight.HandRight'"),
	};

	static FName bodyNames[BODY_PARTS_COUNT + HAND_PARTS_COUNT] = {
		FName("Spine Base"),
		FName("Thight Left"),
		FName("Thight Right"),
		FName("Calf Left"),
		FName("Calf Right"),
		FName("Foot Left"),
		FName("Foot Right"),
		FName("Chest"),
		FName("Upper Arm Left"),
		FName("Upper Arm Right"),
		FName("Lower Arm Left"),
		FName("Lower Arm Right"),
		FName("Hand Left"),
		FName("Hand Right"),
	};

	for (int i = 0; i < (BODY_PARTS_COUNT + HAND_PARTS_COUNT); i++)
	{
		scales[i] = 1;

		ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh( *bodyPartsLocations[i] );
		if (BodyMesh.Succeeded())
		{
			BodyParts[i] = CreateDefaultSubobject<UStaticMeshComponent>(bodyNames[i]);
			BodyParts[i]->SetupAttachment(GetCapsuleComponent());
			BodyParts[i]->SetStaticMesh(BodyMesh.Object);
		}
	}
}

void ADPCharacter::hideMode0()
{
#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DPCharacter mode 0 Hidden"));
#endif

	// Hide skeletal points
	for (int i = 0; i < NUM_JOINTS; i++)
	{
		SKIP0(i);
		SkeletalPoints[i]->SetVisibility(false);
	}

	for (int i = 0; i < NUM_CONNECTORS; i++)
		Connetctors[i]->SetVisibility(false);
}

void ADPCharacter::hideMode1()
{
#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DPCharacter mode 1 Hidden"));
#endif

	for (int i = 0; i < (BODY_PARTS_COUNT + HAND_PARTS_COUNT); i++)
		BodyParts[i]->SetVisibility(false);
}

void ADPCharacter::setMode0()
{
	hideMode1();

	// Show Skeletal points
	for (int i = 0; i < NUM_JOINTS; i++)
	{
		SKIP0(i);
		SkeletalPoints[i]->SetVisibility(true);
	}

	for (int i = 0; i < NUM_CONNECTORS; i++)
		Connetctors[i]->SetVisibility(true);

#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DPCharacter mode 0 Activated"));
#endif
}

void ADPCharacter::setMode1()
{
	hideMode0();

	for (int i = 0; i < (BODY_PARTS_COUNT + HAND_PARTS_COUNT); i++)
		BodyParts[i]->SetVisibility(true);

#ifdef SHOW_DEBUG_MESSAGES
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DPCharacter mode 1 Activated"));
#endif
}

// Called every frame
void ADPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get positionf of body joints from Kinect
	FVector *jointPositions = kinect->getLoactions();
	// Get hand bones positions frol LM
	FVector *handPosition = leap->getLoactions();

	if (jointPositions != nullptr)
	{
		message->SetVisibility(false);
		float Zoffsef = abs(jointPositions[kinect->lastMinZ()].Z+ 67);
		
		// Move everithing up
		for (int i = 0; i < NUM_JOINTS; i++)
		{
			SKIP0(i);
			jointPositions[i].Z += Zoffsef;

			if (mode == 0)
				SkeletalPoints[i]->SetRelativeLocation(jointPositions[i]);
		}

		// Set HEAD POSITION
		SpringArm->SocketOffset = jointPositions[3];

		if (mode == 0)
		{
			for (int i = 0; i < NUM_CONNECTORS; i++)
			{
				int a = ConnectorsEnds[i][0];
				int b = ConnectorsEnds[i][1];

				float X = jointPositions[b].X - jointPositions[a].X;
				float Y = jointPositions[b].Y - jointPositions[a].Y;

				if (X < 0) X = -X;
				if (Y < 0) X = -Y;

				float x2 = X * X;
				float y2 = (jointPositions[b].Y - jointPositions[a].Y) * (jointPositions[b].Y - jointPositions[a].Y);
				float z2 = (jointPositions[b].Z - jointPositions[a].Z) * (jointPositions[b].Z - jointPositions[a].Z);

				// Connectors height
				float z = sqrt(x2 + y2 + z2) / 100.f;
				FVector connetctorSize(SkeletalPointsSize.X, SkeletalPointsSize.Y, z);

				// Connectors center
				FVector connetctorsLocation;
				connetctorsLocation.X = (jointPositions[b].X - jointPositions[a].X) / 2.f + jointPositions[a].X;
				connetctorsLocation.Y = (jointPositions[b].Y - jointPositions[a].Y) / 2.f + jointPositions[a].Y;
				connetctorsLocation.Z = (jointPositions[b].Z - jointPositions[a].Z) / 2.f + jointPositions[a].Z;

				// Connectors Rotation
				FRotator Rot = FRotationMatrix::MakeFromZ(jointPositions[a] - jointPositions[b]).Rotator();

				// Update Connector State
				Connetctors[i]->SetRelativeScale3D(connetctorSize);
				Connetctors[i]->SetRelativeRotation(Rot);
				Connetctors[i]->SetRelativeLocation(connetctorsLocation);
			}
		}
		else if (mode == 1) 
		{
			for (int i = 0; i < BODY_PARTS_COUNT; i++) 
			{
				BodyParts[i]->SetRelativeLocation(jointPositions[BodyPartsPositionsMapping[i]]);
				SKIP1(i);


				BodyParts[i]->SetRelativeRotation(FRotationMatrix::MakeFromZ(jointPositions[BodyEnds[i][0]] - jointPositions[BodyEnds[i][1]]).Rotator());

				float ratio = DISTANCE(jointPositions[BodyEnds[i][0]], jointPositions[BodyEnds[i][1]]) / BodyPartsHeights[i];
				scales[i] = (scales[i] + ratio) / 2.f;
				BodyParts[i]->SetRelativeScale3D(FVector(scales[i], scales[i], ratio));
			}

			// 0
			BodyParts[0]->SetRelativeLocation((jointPositions[12]+ jointPositions[16])/2.f);
			BodyParts[0]->SetRelativeRotation(FRotator(0, 0, 0));
			float dist = DISTANCE(jointPositions[12], jointPositions[16]) / 20;
			scales[0] = (scales[0] + dist) / 2.f;
			BodyParts[0]->SetRelativeScale3D(FVector(scales[0], dist, scales[0]));

			// 7
			FVector scale;
			BodyParts[7]->SetRelativeRotation(FRotator(0, 0, 0));
			scale.Y = DISTANCE(jointPositions[4], jointPositions[8]) / 40.f;
			scale.Z = DISTANCE(jointPositions[1], jointPositions[20]) / 20.f;
			scale.X = (scale.Y + scale.Z) / 3;
			BodyParts[7]->SetRelativeScale3D(scale);

		}
	}

}

// Called to bind functionality to input
void ADPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ChangeUserRepresentation", IE_Released, this, &ADPCharacter::ChangeUserRepresentation);
}

void ADPCharacter::ChangeUserRepresentation()
{
	mode = (mode + 1) % USER_MODES;

	switch (mode) {
	case 0:
		setMode0();
		break;
	case 1:
		setMode1();
		break;
	}
}

int ADPCharacter::GetCurrentMode()
{
	return mode;
}
