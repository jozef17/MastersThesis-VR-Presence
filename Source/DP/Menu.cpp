// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"
#include "DPCharacter.h"
#include <limits>

// Games
#include "TowerOfHanoi.h"
#include "Hockey.h"
#include "Football.h"

void AMenu::changeBacklight()
{
	// Find closest game
	closestGame = -1;
	float min = std::numeric_limits<float>::max();

	for (char i = 0; i < 4; i++)
	{
		if (games[i] != nullptr)
		{
			float dist = FVector::DistSquared(GetActorLocation(), games[i]->GetActorLocation());
			if (dist < min)
			{
				min = dist;
				closestGame = i;
			}
		}
	}

	for (char i = 0; i < 4; i++)
	{
		if (games[i] == nullptr)
			iPad->SetMaterial(11 + i, red);
		else if (i == closestGame)
			iPad->SetMaterial(11 + i, gray);
		else if (i == nextGame)
			iPad->SetMaterial(11 + i, yellow);
		else
			iPad->SetMaterial(11 + i, green);
	}
}

void AMenu::Press(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (isHidden)
		return;

	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor->GetClass()->IsChildOf(AHand::StaticClass())))
		if (((AHand *)OtherActor)->getHand() == RIGHT)
		{
			char n;

			for (n = 0; n < 4; n++)
				if (buttons[n] == OverlappedComp)
					break;

			if (n == nextGame)
				return;

			if ((games[n] == nullptr) || (closestGame == n))
				nextGame = -1;
			else
				nextGame = n;

			timeLeft = 2.0f;
			changeBacklight();
		}
}

// Sets default values
AMenu::AMenu()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> IPAD(TEXT("StaticMesh'/Game/DP/Models/ipad.iPad'"));
	if (IPAD.Succeeded())
	{
		iPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("iPad"));
		iPad->SetupAttachment(RootComponent);
		iPad->SetStaticMesh(IPAD.Object);
		iPad->SetRelativeScale3D(FVector(0.14f));
		iPad->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
		iPad->SetRelativeLocation(FVector(5.0f, 18.0f, 23.0f));
		iPad->SetCollisionProfileName(TEXT("OverlapAll"));

		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
			{
				int n = i * 2 + j;
				FString id = FString("Game") + FString::FromInt(n);
				buttons[n] = CreateDefaultSubobject<UBoxComponent>(*id);
				buttons[n]->SetupAttachment(iPad);
				buttons[n]->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.15f));
				buttons[n]->SetRelativeLocation(FVector(-48.0f + j * 96.0f, -58.0f + i * 124.0f, 3.2f));

				buttons[n]->bGenerateOverlapEvents = true;
				buttons[n]->OnComponentBeginOverlap.AddDynamic(this, &AMenu::Press);
			}

		static ConstructorHelpers::FObjectFinder<UMaterial> Gray(TEXT("Material'/Game/DP/Materials/Menu/GrayBacklight.GrayBacklight'"));
		static ConstructorHelpers::FObjectFinder<UMaterial> Yellow(TEXT("Material'/Game/DP/Materials/Menu/YellowBacklight.YellowBacklight'"));
		static ConstructorHelpers::FObjectFinder<UMaterial> Green(TEXT("Material'/Game/DP/Materials/Menu/GreenBacklight.GreenBacklight'"));
		static ConstructorHelpers::FObjectFinder<UMaterial> Red(TEXT("Material'/Game/DP/Materials/Menu/RedBacklight.RedBacklight'"));

		if (Gray.Succeeded())
			gray = Gray.Object;
		if (Green.Succeeded())
			green = Green.Object;
		if (Yellow.Succeeded())
			yellow = Yellow.Object;
		if (Red.Succeeded())
			red = Red.Object;
	}
}

// Called when the game starts or when spawned
void AMenu::BeginPlay()
{
	Super::BeginPlay();

	// Find all 4 games in scene
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor *Actor = *ActorItr;

		if (Actor->GetClass()->IsChildOf(ATowerOfHanoi::StaticClass()))
			games[0] = Actor;
		else if (Actor->GetClass()->IsChildOf(AFootball::StaticClass()))
			games[1] = Actor;
		else if (Actor->GetClass()->IsChildOf(AHockey::StaticClass()))
			games[2] = Actor;
	}

	changeBacklight();
}

void AMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isHidden)
		return;

	if (nextGame >= 0)
	{
		timeLeft -= DeltaTime;
		if (timeLeft <= 0)
		{
			APawn* pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (pawn->GetClass()->IsChildOf(ADPCharacter::StaticClass()))
			{
				FVector location = nextGame < 3 ? ((AGame *)games[nextGame])->getPositionOffset() : FVector{0, 0, 0};
				FRotator rotation = nextGame < 3 ? ((AGame *)games[nextGame])->getRotationOffset() : FRotator{0, 0, 0};

				location += games[nextGame]->GetActorLocation() + FVector(0.0f, 0.0f, 93.0f);
				rotation += games[nextGame]->GetActorRotation();

				pawn->SetActorLocation(location);
				pawn->SetActorRotation(rotation);

				nextGame = -1;
				changeBacklight();
			}
		}
	}
}

void AMenu::hide()
{
	isHidden = true;
	iPad->SetVisibility(false);
}

void AMenu::show()
{
	nextGame = -1;
	timeLeft = 1.5f;
	isHidden = false;
	iPad->SetVisibility(true);
	changeBacklight();
}
