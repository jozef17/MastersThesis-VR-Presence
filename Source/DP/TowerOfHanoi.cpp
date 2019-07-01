// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerOfHanoi.h"

int ATowerOfHanoi::numOfDisks = 3;

ATowerOfHanoi::ATowerOfHanoi()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Tower Of Hanoi Root"));

	diskCount = numOfDisks;
	InitComponents();

	for (int i = 0; i < 6; i++)
	{
		diskLocations[i].height = -1;
		diskLocations[i].stick = -1;

		for (int j = 0; j < 3; j++)
			stickMap[j][i] = -1;
	}
}

void ATowerOfHanoi::InitComponents()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Polished.M_Wood_Floor_Walnut_Polished'"));

	// Create Bottom Desk
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Desk"));
		Bottom->SetupAttachment(RootComponent);
		Bottom->SetStaticMesh(Cube.Object);
		Bottom->SetMaterial(0, Material.Object);
		Bottom->SetRelativeScale3D(FVector(1, 3.6, 0.05)); // TODO
	}

	// Create 3 pillars
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (Cylinder.Succeeded())
		for (int i = 0; i < 3; i++)
		{
			FString id = "Stick" + FString::FromInt(i);
			Sticks[i] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			Sticks[i]->SetupAttachment(RootComponent);
			Sticks[i]->SetStaticMesh(Cylinder.Object);
			Sticks[i]->SetRelativeLocation(FVector(0, (i - 1) * 130, 60));
			Sticks[i]->SetRelativeScale3D(FVector(0.2f, 0.2f, 1.2f));
			Sticks[i]->SetMaterial(0, Material.Object);
		}

	for (int i = 0; i < 3; i++)
	{
		diskLocations[i].stick = -1;
		diskLocations[i].height = -1;

		for (int j = 0; j < diskCount; j++)
		{
			FString id = "Level" + FString::FromInt(i) + "_" + FString::FromInt(j);
			levels[i][j] = CreateDefaultSubobject<UBoxComponent>(FName(*id));
			levels[i][j]->SetupAttachment(Sticks[i]);
			levels[i][j]->SetRelativeLocation(FVector(0, 0, -47.0f + j * 12.8f));
			levels[i][j]->SetRelativeScale3D(FVector(2.2f, 2.05f, .03));

			levels[i][j]->bGenerateOverlapEvents = true;
			levels[i][j]->OnComponentBeginOverlap.AddDynamic(this, &ATowerOfHanoi::Entry);
		}
	}

	borders = CreateDefaultSubobject<UBoxComponent>(TEXT("Border"));
	borders->SetupAttachment(RootComponent);
	borders->SetRelativeLocation(FVector(0, 0, 96));
	borders->SetRelativeScale3D(FVector(4.0f, 8.0f, 3.0f));
	borders->bGenerateOverlapEvents = true;
	borders->OnComponentEndOverlap.AddDynamic(this, &ATowerOfHanoi::LeaveBorder);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Firework(TEXT("ParticleSystem'/Game/DP/Particles/Firework.Firework'"));
	if (Firework.Succeeded())
	{
		firework = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Firework"));
		firework->SetupAttachment(RootComponent);
		firework->SetTemplate(Firework.Object);
		firework->SetRelativeLocation(FVector(0, 0, -1000));
	}

}

void ATowerOfHanoi::create(int disk)
{
	FVector loc = FVector(0, -130 + 130 * diskLocations[disk].stick, 10.2f + (diskLocations[disk].height) * 15.3f);
	loc = GetActorRotation().RotateVector(GetActorScale() * loc);

	Disks[disk] = ATowerOfHanoiDisk::build(disk, this, GetActorLocation() + loc);
	Disks[disk]->SetActorScale3D(FVector(0.25f, 0.25f, 0.35f) * GetActorScale());
}

void ATowerOfHanoi::Entry(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (initRunning)
		return;

	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OverlappedComp != nullptr) &&
		OtherActor->GetClass()->IsChildOf(ATowerOfHanoiDisk::StaticClass()) && (OverlappedComp->GetOwner() == this)
		&& OtherComp->GetName().Contains(TEXT("Disk")))
	{
		int i, j, disk;

		// Find which level overlaps
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < diskCount; j++)
				if (levels[i][j] == OverlappedComp)
					break;

			if (j < 6 && levels[i][j] == OverlappedComp)
				break;
		}

		if (levels[i][j] != OverlappedComp)
			return;

		// Find disk
		for (disk = 0; disk < diskCount; disk++)
			if (Disks[disk] == OtherActor)
				break;

		if (Disks[disk] != OtherActor)
			return;

		if (stickMap[i][j] == disk)
			return;
		if (stickMap[i][j] > -1)
			return;

		if (j == 0)
		{
			stickMap[diskLocations[disk].stick][diskLocations[disk].height] = -1;
			stickMap[i][j] = disk;

			if (diskLocations[disk].height > 0)
				Disks[stickMap[diskLocations[disk].stick][diskLocations[disk].height - 1]]->enable();

			diskLocations[disk].stick = i;
			diskLocations[disk].height = j;
		}
		else if (stickMap[i][j - 1] > -1 && stickMap[i][j] == -1)
		{
			if (stickMap[i][j - 1] < disk)
			{
				ATowerOfHanoiDisk *d = Disks[disk];
				Disks[disk] = nullptr;
				d->Destroy();
			}
			else if (stickMap[i][j - 1] > disk)
			{
				Disks[stickMap[i][j - 1]]->disablePhysics();
				stickMap[diskLocations[disk].stick][diskLocations[disk].height] = -1;
				stickMap[i][j] = disk;

				if (diskLocations[disk].height > 0)
					Disks[stickMap[diskLocations[disk].stick][diskLocations[disk].height - 1]]->enable();

				diskLocations[disk].stick = i;
				diskLocations[disk].height = j;

				if ((j == (diskCount - 1)) && (i > 0))
				{
					firework->SetRelativeLocation(FVector(0, (i - 1) * 130, 130));
					firework->ActivateSystem(true);
					timeLeft = 5.0f;
				}
			}
		}

	}
}

void ATowerOfHanoi::LeaveBorder(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (IsPendingKill() || !UKismetSystemLibrary::IsValid(this))
		return;

	if ((OtherActor != nullptr) && (OtherComp != nullptr) && OtherActor->GetClass()->IsChildOf(ATowerOfHanoiDisk::StaticClass()) && OtherComp->GetName().Contains(TEXT("Disk")))
	{
		for (int disk = 0; disk < diskCount; disk++)
			if (Disks[disk] == OtherActor)
			{
				ATowerOfHanoiDisk *d = Disks[disk];
				Disks[disk] = nullptr;
				d->Destroy();
			}


		for (int disk = 0; disk < diskCount; disk++)
		{
			if (Disks[disk] == nullptr)
				create(disk);
		}
	}
}

// Called when the game starts or when spawned
void ATowerOfHanoi::BeginPlay()
{
	initRunning = true;
	Super::BeginPlay();
	ResetGame();
}

void ATowerOfHanoi::ResetGame()
{
	initRunning = true;

	for (int i = 0; i < diskCount; i++)
	{
		diskLocations[i].stick = -1;
		diskLocations[i].height = -1;

		for (int j = 0; j < 3; j++)
			stickMap[j][i] = -1;
	}

	for (int i = (diskCount - 1); i >= 0; i--)
	{
		stickMap[0][(diskCount - 1) - i] = i;
		diskLocations[i].height = (diskCount - 1) - i;
		diskLocations[i].stick = 0;

		if (Disks[i] != nullptr)
			Disks[i]->Destroy();
		else
			create(i);

		if (i > 0)
			Disks[i]->disablePhysics();
	}

	Disks[0]->enable();
	initRunning = false;
}

FVector ATowerOfHanoi::getPositionOffset()
{
	return FVector(110.0f, 0.0f, -1 * GetActorLocation().Z);
}

void ATowerOfHanoi::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (timeLeft > 0)
	{
		timeLeft -= deltaTime;

		if (timeLeft <= 0)
		{
			SetActorScale3D(FVector(0.25));
			ResetGame();
		}
	}
}
