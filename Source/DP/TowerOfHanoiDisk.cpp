// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerOfHanoiDisk.h"
#include "Engine.h"

unsigned char ATowerOfHanoiDisk::diskID = 0;

// Sets default values
ATowerOfHanoiDisk::ATowerOfHanoiDisk()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	this->id = diskID;

	// Create Disk
	FString target = "StaticMesh'/Game/DP/Models/TowerOfHanoi/TowerOfHanoi_Disk" + FString::FromInt(id) + ".TowerOfHanoi_Disk" + FString::FromInt(id) + "'";
	ConstructorHelpers::FObjectFinder<UStaticMesh> Disk(*target);
	if (Disk.Succeeded())
	{
		disk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disk"));
		disk->SetStaticMesh(Disk.Object);
		disk->SetupAttachment(RootComponent);
	}

	FVector scale;
	switch (id)
	{
	case 0:
		scale = FVector(2.55f, 1.0f, 1.0f);
		break;
	case 1:
		scale = FVector(3.25f, 1.3f, 1.0f);
		break;
	case 2:
		scale = FVector(4.25f, 1.7f, 1.0f);
		break;
	case 3:
		scale = FVector(5.15f, 2.1f, 1.0f);
		break;
	case 4:
		scale = FVector(6.05f, 2.5f, 1.0f);
		break;
	case 5:
		scale = FVector(6.95f, 2.85f, 1.0f);
		break;
	}

	for (int i = 0; i < 4; i++)
	{
		FString id = "Collision" + FString::FromInt(i);
		collision[i] = CreateDefaultSubobject<UBoxComponent>(FName(*id));
		collision[i]->SetupAttachment(disk);
		collision[i]->SetRelativeScale3D(scale);
		collision[i]->SetRelativeRotation(FRotator(0.0f, i*45.0f, 0.0f));
	}

	takeHandler = CreateDefaultSubobject<UTakeHandler>(TEXT("UTakeHandler"));

	rules = new TakeRules(disk, collision[0]);

	for (int i = 1; i < 4; i++)
		rules->addCollision(collision[i]);

	takeHandler->init(rules);
}

ATowerOfHanoiDisk * ATowerOfHanoiDisk::build(int diskID, AActor * owner)
{
	return build(diskID, owner, FVector(0));
}

ATowerOfHanoiDisk * ATowerOfHanoiDisk::build(int diskID, AActor * owner, FVector location)
{
	if (diskID < 0 || diskID >= DISKS)
		return nullptr;
	if (owner->IsPendingKill() || !UKismetSystemLibrary::IsValid(owner))
		return nullptr;

	ATowerOfHanoiDisk::diskID = (unsigned char)diskID;
	ATowerOfHanoiDisk *disk = owner->GetWorld()->SpawnActor<ATowerOfHanoiDisk>(location, FRotator(0.0f), FActorSpawnParameters());
	disk->AttachToActor(owner, FAttachmentTransformRules::KeepWorldTransform);
	disk->rules->setParentActor(owner);
	return disk;
}

void ATowerOfHanoiDisk::reset()
{
	disablePhysics();
	disk->SetRelativeLocation(FVector(0));
	disk->SetRelativeRotation(FRotator(0));
}

void ATowerOfHanoiDisk::enable()
{
	takeHandler->enable();
}

void ATowerOfHanoiDisk::disable()
{
	takeHandler->disable();
}

void ATowerOfHanoiDisk::disablePhysics()
{
	takeHandler->disablePhysics();
}
