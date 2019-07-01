// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TakeHandler.h"
#include "TowerOfHanoiDisk.generated.h"

#define DISKS 6

UCLASS()
class DP_API ATowerOfHanoiDisk : public AActor
{
	GENERATED_BODY()

private:
	static unsigned char diskID;
	unsigned char id;

	TakeRules* rules = nullptr;

	UStaticMeshComponent *disk;
	UShapeComponent *collision[4];
	UTakeHandler *takeHandler;

public:
	// Sets default values for this actor's properties
	ATowerOfHanoiDisk();

	static ATowerOfHanoiDisk *build(int diskID, AActor *owner);
	static ATowerOfHanoiDisk *build(int diskID, AActor *owner, FVector location);

	void reset();
	void enable();
	void disable();
	void disablePhysics();

};
