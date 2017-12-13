// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Monitor.generated.h"

UCLASS()
class DP_API AMonitor : public AActor
{
	GENERATED_BODY()
private:
	UStaticMeshComponent *monitor;

public:	
	// Sets default values for this actor's properties
	AMonitor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
