// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Menu.h"
#include "Hand.generated.h"

enum finger {
	LEFT_THUMB = 0,
	LEFT_INDEX_FINGER = 1,
	LEFT_MIDDLE_FINGER = 2,
	LEFT_RING_FINGER = 3,
	LEFT_LITTLE_FINGER = 4,

	RIGHT_THUMB = 5,
	RIGHT_INDEX_FINGER = 6,
	RIGHT_MIDDLE_FINGER = 7,
	RIGHT_RING_FINGER = 8,
	RIGHT_LITTLE_FINGER = 9,

	NONE = 100
};

enum hand {
	LEFT = 0,
	RIGHT = 1
};

UCLASS()
class DP_API AHand : public AActor
{
	GENERATED_BODY()

private:
	AMenu * menu = nullptr;		// Menu
	UBoxComponent *menuArea;	// Are where menu will be shown 
	unsigned int counter;		// Number of Right-hand parts "in button"
	bool isShown = false;
	bool isEnabled = false;

	UStaticMeshComponent * button; // Button to show / hide menu
	unsigned int fingerCounter = 0;

	UMaterial *green;	// Menu can be shown
	UMaterial *red;		// Hide menu

	// Press buton to show / hide menu
	UFUNCTION()
	void Press(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Release(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Some object enters area, wher could be shown menu
	// -> dobt show menu until this area is empty
	UFUNCTION()
	void Enter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Some object leaves area, if area is empty 
	// -> menu could be shown
	UFUNCTION()
	void Leave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	USceneComponent * handRooot;

	bool isLeft = true;
	static bool left;

	virtual void BeginPlay() override;

public:	
	AHand();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	hand getHand();
	
	virtual void hide() {};
	virtual void set() {};

	virtual void enable() {};
	virtual void disable() {};

	virtual finger getFinger(void *ptr);
	virtual FVector getFingerLocation(finger f);
	virtual void processLeapData(FVector *data, FRotator rotation);

	static AHand *build(bool left, AActor *owner);

};
