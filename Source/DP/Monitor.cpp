// Fill out your copyright notice in the Description page of Project Settings.

#include "Monitor.h"
#include "Engine.h"
#include "Runtime/Engine/Public/Features/ILiveStreamingService.h"

// Sets default values
AMonitor::AMonitor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Monitor Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		monitor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Monitor"));
		monitor->SetupAttachment(RootComponent);
		monitor->SetStaticMesh(Cube.Object);
	}


}

// Called when the game starts or when spawned
void AMonitor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

