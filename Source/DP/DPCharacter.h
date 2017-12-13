// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KinectBridge.h"
#include "LeapBridge.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "DPCharacter.generated.h"

#define NUM_CONNECTORS 22	// number of connecting parts in mode 0
#define BODY_PARTS_COUNT 14 // number of body parts in mode 1
#define HAND_PARTS_COUNT 0  // number of hand parts in mode 1

UCLASS()
class DP_API ADPCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	const int USER_MODES = 2;
	KinectBridge *kinect;
	LeapBridge *leap;

	USpringArmComponent *SpringArm;
	UCameraComponent *Camera;
	UTextRenderComponent *message;

	USceneComponent *HandRoot[2];

	// Mode 0 (Kinect Sceletal points)
	UStaticMeshComponent *SkeletalPoints[NUM_JOINTS];
	UStaticMeshComponent *Connetctors[NUM_CONNECTORS];

	// Mode 1 (Kinect Body)
	UStaticMeshComponent *BodyParts[BODY_PARTS_COUNT + HAND_PARTS_COUNT];
	float scales[BODY_PARTS_COUNT + HAND_PARTS_COUNT];

protected:
	virtual void BeginPlay() override;

	void initMode0();
	void initMode1(); // TODO

	void hideMode0();
	void hideMode1(); // TODO

	void setMode0(); // TODO
	void setMode1(); // TODO

public:
	ADPCharacter();

	// UE4 functions
	virtual void Tick(float DeltaTime) override;	// Called every frame
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

	// Specific functions
	// Action to change user representation - Sceleton / Body
	UFUNCTION(BlueprintCallable)
	void ChangeUserRepresentation();	
	
	// Returns current user representation mode
	UFUNCTION(BlueprintPure)
	int GetCurrentMode();


	// Blueprint settable components
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	int mode = 0;

	// Kinect Height from floor
	UPROPERTY(EditAnywhere)
	float KinectHeightOffset = 100.0;

	// Mode 1
	UPROPERTY(EditAnywhere)
	FVector SkeletalPointsSize = FVector(0.08f, 0.08f, 0.08f);

	// Material that will be aplied on user representation body
	UPROPERTY(EditAnywhere)
	UMaterial *BodyMaterial;
};
