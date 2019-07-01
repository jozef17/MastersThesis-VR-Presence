// Fill out your copyright notice in the Description page of Project Settings.

#include "DPCharacter.h"
#include "JointSkeleton.h"
#include "BodyModel.h"

#include "Hockey.h"
#include "Football.h"
#include "TowerOfHanoi.h"


ADPCharacter::ADPCharacter()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DP Character Root"));
	PrimaryActorTick.bCanEverTick = true;

	// Create & Setup Camera.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ADPCharacter::ChangeUserRepresentation()
{
	mode = (mode + 1) % 2;

	if (current != nullptr)
	{
		current->Destroy();
		GetWorld()->ForceGarbageCollection(true);
	}

	if (mode == 0)
		current = GetWorld()->SpawnActor<AJointSkeleton>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FActorSpawnParameters());
	else if (mode == 1)
		current = GetWorld()->SpawnActor<ABodyModel>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), FActorSpawnParameters());

	current->AttachRootComponentToActor(this);
	current->setCharacter(this);
	current->set();
}

void ADPCharacter::Exit()
{
	FGenericPlatformMisc::RequestExit(false);
}

void ADPCharacter::ResetBallPOsition()
{
	for (TActorIterator<AFootball> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		((AFootball *)*ActorItr)->resetPlayingObject();
}

void ADPCharacter::ResetPuckPosition()
{
	for (TActorIterator<AHockey> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		((AHockey *)*ActorItr)->resetPlayingObject();
}

void ADPCharacter::RotateHockeyStick()
{
	for (TActorIterator<AHockey> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		((AHockey *)*ActorItr)->rotateHockeyStick();
}

void ADPCharacter::MoveForward(float val)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	SetActorLocation(GetActorLocation() + Direction * val);
}

void ADPCharacter::MoveRight(float val)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	SetActorLocation(GetActorLocation() + Direction * val);
}

void ADPCharacter::MoveUp(float val)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	SetActorLocation(GetActorLocation() + Direction * val);
}

// Called when the game starts or when spawned
void ADPCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Mode
	mode--;
	ChangeUserRepresentation();
}

// Called to bind functionality to input
void ADPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ChangeUserRepresentation", IE_Released, this, &ADPCharacter::ChangeUserRepresentation);
	PlayerInputComponent->BindAction("Exit", IE_Released, this, &ADPCharacter::Exit);

	PlayerInputComponent->BindAction("ResetBallPosition", IE_Released, this, &ADPCharacter::ResetBallPOsition);
	PlayerInputComponent->BindAction("ResetPuckPosition", IE_Released, this, &ADPCharacter::ResetPuckPosition);
	PlayerInputComponent->BindAction("RotateHockeyStick", IE_Released, this, &ADPCharacter::RotateHockeyStick);

	// Moving with keys
	PlayerInputComponent->BindAxis("MoveForward", this, &ADPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADPCharacter::MoveUp);
}

FRotator ADPCharacter::GetCameraRotation()
{
	return Camera->RelativeRotation;
}

#define USE_AVG

void ADPCharacter::SetCameraLocation(FVector camLocation)
{
#ifdef USE_AVG
	// X
	if (FMath::Abs(avg.X - camLocation.X) < 2)
	{
		counter.X += 1;
		avg.X = avg.X + (camLocation.X - avg.X) / counter.X;
	}
	else
	{
		avg.X = camLocation.X;
		counter.X = 0;
	}
	// ...
	// Y
	if (FMath::Abs(avg.Y - camLocation.Y) < 2)
	{
		counter.Y += 1;
		avg.Y = avg.Y + (camLocation.Y - avg.Y) / counter.Y;
	}
	else
	{
		avg.Y = camLocation.Y;
		counter.Y = 0;
	}
#else
	avg.X = camLocation.X;
	avg.Y = camLocation.Y;
#endif;
	// Z
	if (FMath::Abs(avg.Z - camLocation.Z) < 15)
	{
		counter.Z += 1;
		avg.Z = avg.Z + (camLocation.Z - avg.Z) / counter.Z;
	}
	else
	{
		avg.Z = camLocation.Z;
		counter.Z = 0;
	}
	
	SpringArm->SocketOffset = avg;

#ifdef USE_AVG
	if (counter.X > 1000)
		counter.X = 1000;
	if (counter.Y > 1000)
		counter.Y = 1000;
#endif
	if (counter.Z > 2000)
		counter.Z = 2000;
}

FVector ADPCharacter::GetCameraLocation()
{
	return SpringArm->SocketOffset;
}
