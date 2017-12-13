// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerOfHanoi.h"
#include "Engine.h"

ATowerOfHanoi::ATowerOfHanoi()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Tower Of Hanoi Root"));

	InitComponents();
}

ATowerOfHanoi::~ATowerOfHanoi()
{
	delete Disks;
}

void ATowerOfHanoi::InitComponents()
{
	// Create Bottom Desk
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Desk"));
		Bottom->SetupAttachment(RootComponent);
		Bottom->SetStaticMesh(Cube.Object);
		Bottom->SetRelativeScale3D(FVector(1, 3, 0.05)); // TODO
	}

	// Create 3 pillats
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
		for (int i = 0; i < 3; i++)
		{
			FString id = "Stick" + FString::FromInt(i);
			Sticks[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			Sticks[i]->SetupAttachment(RootComponent);
			Sticks[i]->SetStaticMesh(Cylinder.Object);
			Sticks[i]->SetRelativeLocation(FVector(0, (i - 1) * 100, NumOfDisks * 10)); // TODO
			Sticks[i]->SetRelativeScale3D(FVector(.2, .2, .2*NumOfDisks)); // TODO
		}

	// Create Disks
	Disks = new UStaticMeshComponent *[NumOfDisks];
	for (unsigned int i = 0; i < NumOfDisks; i++)
	{
		FString id = "Disk" + FString::FromInt(i);
		Disks[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
		Disks[i]->SetupAttachment(RootComponent);

		FString target = "StaticMesh'/Game/DP/Models/TowerOfHanoi/TowerOfHanoi_Disk" + FString::FromInt(i) + ".TowerOfHanoi_Disk" + FString::FromInt(i) + "'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> Disk(*target);
		if (Disk.Succeeded())
			Disks[i]->SetStaticMesh(Disk.Object);

		Disks[i]->SetRelativeScale3D(FVector(.25, .25, .35)); // TODO
		Disks[i]->SetRelativeLocation(FVector(0, -100, 15.3*(NumOfDisks - i) - 5));
	}

}

// Called when the game starts or when spawned
void ATowerOfHanoi::BeginPlay()
{
	Super::BeginPlay();

	Bottom->SetMaterial(0, Material);
	Sticks[0]->SetMaterial(0, Material);
	Sticks[1]->SetMaterial(0, Material);
	Sticks[2]->SetMaterial(0, Material);

	Disks[0]->SetMaterial(0, Disk0Material);
	Disks[1]->SetMaterial(0, Disk1Material);
	if (NumOfDisks >= 3)
		Disks[2]->SetMaterial(0, Disk2Material);
	if (NumOfDisks >= 4)
		Disks[3]->SetMaterial(0, Disk3Material);
	if (NumOfDisks >= 5)
		Disks[4]->SetMaterial(0, Disk4Material);
	if (NumOfDisks >= 6)
		Disks[5]->SetMaterial(0, Disk5Material);
}

// Called every frame
void ATowerOfHanoi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

