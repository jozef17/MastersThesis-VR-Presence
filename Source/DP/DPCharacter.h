// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserRepresentation.h"
#include "Engine.h"
#include "DPCharacter.generated.h"

UCLASS()
class DP_API ADPCharacter : public APawn
{
	GENERATED_BODY()

public:
	ADPCharacter();

private:
	AUserRepresentation * current = nullptr;

	USpringArmComponent * SpringArm;
	UCameraComponent *Camera;

	FVector avg;
	FVector counter;

	void ChangeUserRepresentation();	// Action to change user representation - Sceleton / Body
	void Exit();						// Exits Game
	void ResetBallPOsition();			// Resets default Ball position in Foorball
	void ResetPuckPosition();			// Resets default Puck position in Hockey
	void RotateHockeyStick();			// Rotates Hockey Stick 180 degrees

	void MoveForward(float val);
	void MoveRight(float val);
	void MoveUp(float val);

protected:
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns current camera (HMD) rotation
	FRotator GetCameraRotation();

	// Sets Camera Relative Location
	void SetCameraLocation(FVector camLocation);
	FVector GetCameraLocation();

	// Blueprint settable components
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	int mode = 1;
};
