// Fill out your copyright notice in the Description page of Project Settings.

#include "LightSwitch.h"
#include "Engine.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Const.h"

void ALightSwitch::Create()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	LigthswitchKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Key"));

	LigthswitchBaseLeftBorder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Left Border"));
	LigthswitchBaseRightBorder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Right Border"));
	LigthswitchBaseTopBorder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Top Border"));
	LigthswitchBaseBottomBorder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Bottom Border"));
	LigthswitchBaseBottomInside = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Bottom inside"));
	LigthswitchBaseBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lightswitch Base - Bottom"));

	ON = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ON"));
	OFF = CreateDefaultSubobject<UTextRenderComponent>(TEXT("OFF"));

	OnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("On Colision"));
	OffCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Off Colision"));
}

void ALightSwitch::Attach()
{
	LigthswitchBaseLeftBorder->SetupAttachment(RootComponent);
	LigthswitchBaseRightBorder->SetupAttachment(RootComponent);
	LigthswitchBaseTopBorder->SetupAttachment(RootComponent);
	LigthswitchBaseBottomBorder->SetupAttachment(RootComponent);
	LigthswitchBaseBottomInside->SetupAttachment(RootComponent);
	LigthswitchBaseBottom->SetupAttachment(RootComponent);

	LigthswitchKey->SetupAttachment(RootComponent);
	ON->SetupAttachment(LigthswitchKey);
	OFF->SetupAttachment(LigthswitchKey);

	OnCollision->SetupAttachment(RootComponent);
	OffCollision->SetupAttachment(RootComponent);
}

void ALightSwitch::Setup()
{
	// Setup "Cube" parts (base borders & switch / key)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		LigthswitchBaseLeftBorder->SetStaticMesh(Cube.Object);
		LigthswitchBaseLeftBorder->SetRelativeLocation(FVector(-45.0f, 0.0f, -1.5f));
		LigthswitchBaseLeftBorder->SetRelativeScale3D(FVector(0.1f, 0.8f, 0.07f));

		LigthswitchBaseRightBorder->SetStaticMesh(Cube.Object);
		LigthswitchBaseRightBorder->SetRelativeLocation(FVector(45.0f, 0.0f, -1.5f));
		LigthswitchBaseRightBorder->SetRelativeScale3D(FVector(0.1f, 0.8f, 0.07f));

		LigthswitchBaseTopBorder->SetStaticMesh(Cube.Object);
		LigthswitchBaseTopBorder->SetRelativeLocation(FVector(0.0, -45.0f, -1.5f));
		LigthswitchBaseTopBorder->SetRelativeScale3D(FVector(1.0f, 0.1f, 0.07f));

		LigthswitchBaseBottomBorder->SetStaticMesh(Cube.Object);
		LigthswitchBaseBottomBorder->SetRelativeLocation(FVector(0.0, 45.0f, -1.5f));
		LigthswitchBaseBottomBorder->SetRelativeScale3D(FVector(1.0f, 0.1f, 0.07f));

		LigthswitchKey->SetStaticMesh(Cube.Object);
		LigthswitchKey->SetRelativeLocation(FVector(0.0, 0.0f, 1.9f));
		LigthswitchKey->SetRelativeScale3D(FVector(0.8f, 0.78f, 0.07f));
		LigthswitchKey->SetRelativeRotation(FRotator(0.0f, 0.0f, -5.0f));
	}

	// Setup Base Bottom
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (Plane.Succeeded())
	{
		LigthswitchBaseBottomInside->SetStaticMesh(Plane.Object);
		LigthswitchBaseBottomInside->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));

		LigthswitchBaseBottom->SetStaticMesh(Plane.Object);
		LigthswitchBaseBottom->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));
		LigthswitchBaseBottom->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	}

	// Setup Text
	ON->SetText(TEXT("ON"));
	ON->SetRelativeLocation(FVector(15.0, 16.0f, 51.0f));
	ON->SetRelativeRotation(FRotator(90.0f, -90.0f, 0.0f));
	ON->SetRelativeScale3D(FVector(1.0f, 1.0f, 14.29f));

	OFF->SetText(TEXT("OFF"));
	OFF->SetRelativeLocation(FVector(20.0, -51.0f, 51.0f));
	OFF->SetRelativeRotation(FRotator(90.0f, -90.0f, 0.0f));
	OFF->SetRelativeScale3D(FVector(1.0f, 1.0f, 14.29f));

	// Setup Colisions
	OnCollision->SetRelativeLocation(FVector(0.0f, 33.5f, -3.2f));
	OnCollision->SetRelativeScale3D(FVector(0.8f, 0.2f, 0.05f));

	OffCollision->SetRelativeLocation(FVector(0.0f, -33.5f, -3.2f));
	OffCollision->SetRelativeScale3D(FVector(0.8f, 0.2f, 0.05f));

	// TODO Enable physics Symulation For "Key"

}

void ALightSwitch::TurnOn_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && !on)
	{
		on = true;

#ifdef SHOW_DEBUG_MESSAGES
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Lightswitch Turned ON"));
#endif
	}

}

void ALightSwitch::TurnOff_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && on)
	{
		on = false;

#ifdef SHOW_DEBUG_MESSAGES
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Lightswitch Turned OFF"));
#endif
	}
}

// Sets default values
ALightSwitch::ALightSwitch()
{
	PrimaryActorTick.bCanEverTick = false; // ???

	Create();	// Create Components
	Attach();	// Attach to Root Component
	Setup();	// Setup Visuals
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();

	OnCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitch::TurnOn);
	OffCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitch::TurnOff);
}
