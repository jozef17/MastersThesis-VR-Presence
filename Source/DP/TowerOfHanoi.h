// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game.h"
#include "TowerOfHanoiDisk.h"
#include "Engine.h"
#include "TowerOfHanoi.generated.h"

typedef struct {
	char stick;		// 0-3
	char height;	//0-5
}loc_t;

UCLASS()
class DP_API ATowerOfHanoi : public AGame
{
	GENERATED_BODY()

private:
	float timeLeft = 0.0f;

	int diskCount;

	// Game Elements
	UParticleSystemComponent *firework;
	UStaticMeshComponent *Bottom;
	UStaticMeshComponent *Sticks[3];
	ATowerOfHanoiDisk *Disks[6];

	UBoxComponent *levels[3][6];
	UBoxComponent *borders;

	void InitComponents();
	char stickMap[3][6];
	loc_t diskLocations[6];

	void create(int disk);

	bool initRunning = true;

	UFUNCTION()
	void Entry(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void LeaveBorder(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:
	static int numOfDisks;

	ATowerOfHanoi();

	virtual void ResetGame() override;
	virtual FVector getPositionOffset() override;

	virtual void Tick(float delta) override;

};
