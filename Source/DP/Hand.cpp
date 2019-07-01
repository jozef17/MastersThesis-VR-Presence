// Fill out your copyright notice in the Description page of Project Settings.

#include "Hand.h"
#include "DPCharacter.h"

bool AHand::left = true;

void AHand::Press(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor->GetClass()->IsChildOf(AHand::StaticClass())))
		if (((AHand *)OtherActor)->getHand() == RIGHT)
			fingerCounter++;
}
// Check if User representation is dysplayd and devices are available
void AHand::Release(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor->GetClass()->IsChildOf(AHand::StaticClass())))
		if (((AHand *)OtherActor)->getHand() == RIGHT)
		{
			fingerCounter--;

			if (!isEnabled)
				return;

			if (fingerCounter == 0 && counter == 0)
				if (isShown)
				{
					isShown = false;
					menu->hide();
					button->SetMaterial(0, green);
				}
				else
				{
					isShown = true;
					menu->show();
					button->SetMaterial(0, red);
				}
		}
}

void AHand::Enter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != menu))
	{
		// Skip if component is part of user - user representation, hand or body
		if (OtherActor->GetClass()->IsChildOf(AHand::StaticClass()) || OtherActor->GetClass()->IsChildOf(ABody::StaticClass())
			|| OtherActor->GetClass()->IsChildOf(AUserRepresentation::StaticClass()) || OtherActor->GetClass()->IsChildOf(AMenu::StaticClass()))
			return;
		if (OtherComp->GetClass()->IsChildOf(UShapeComponent::StaticClass()))
			return;
		counter++;
		button->SetMaterial(0, red);
	}
}

void AHand::Leave(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != menu))
	{
		// Skip if component is part of user - user representation, hand or body
		if (OtherActor->GetClass()->IsChildOf(AHand::StaticClass()) || OtherActor->GetClass()->IsChildOf(ABody::StaticClass())
			|| OtherActor->GetClass()->IsChildOf(AUserRepresentation::StaticClass()) || OtherActor->GetClass()->IsChildOf(AMenu::StaticClass()))
			return;
		if (OtherComp->GetClass()->IsChildOf(UShapeComponent::StaticClass()))
			return;
		if (counter > 0)
			counter--;
		if (counter == 0)
			button->SetMaterial(0, green);
	}
}

void AHand::BeginPlay()
{
	Super::BeginPlay();

	if (left)
	{
		menu = GetWorld()->SpawnActor<AMenu>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
		if (menu != nullptr)
			menu->AttachRootComponentToActor(this);
		menu->hide();
	}
}

// Sets default values
AHand::AHand()
{
	isLeft = left;
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	handRooot = CreateDefaultSubobject<USceneComponent>(TEXT("Hand Root"));
	handRooot->SetupAttachment(RootComponent);

	if (isLeft)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		if (Sphere.Succeeded())
		{
			button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Menu Button"));
			button->SetupAttachment(handRooot);
			button->SetStaticMesh(Sphere.Object);
			button->SetRelativeScale3D(FVector(0.075f));

			USphereComponent *collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
			collision->SetupAttachment(button);
			collision->SetRelativeScale3D(FVector(1.2f));
			collision->bGenerateOverlapEvents = true;
			collision->OnComponentBeginOverlap.AddDynamic(this, &AHand::Press);
			collision->OnComponentEndOverlap.AddDynamic(this, &AHand::Release);

			static ConstructorHelpers::FObjectFinder<UMaterial> Green(TEXT("Material'/Game/DP/Materials/Menu/GreenBacklight.GreenBacklight'"));
			static ConstructorHelpers::FObjectFinder<UMaterial> Red(TEXT("Material'/Game/DP/Materials/Menu/RedBacklight.RedBacklight'"));

			if (Green.Succeeded())
				green = Green.Object;
			if (Red.Succeeded())
				red = Red.Object;

			button->SetMaterial(0, green);
		}

		menuArea = CreateDefaultSubobject<UBoxComponent>(TEXT("MenuArea"));
		menuArea->SetupAttachment(handRooot);
		menuArea->SetRelativeLocation(FVector(5.0f, 18.0f, 23.2f));
		menuArea->SetRelativeScale3D(FVector(0.05f, 0.48f, 0.68f));

		menuArea->bGenerateOverlapEvents = true;
		menuArea->OnComponentBeginOverlap.AddDynamic(this, &AHand::Enter);
		menuArea->OnComponentEndOverlap.AddDynamic(this, &AHand::Leave);
	}
}

void AHand::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(menu)
		menu->Destroy();
}

hand AHand::getHand()
{
	return isLeft ? LEFT : RIGHT;
}

finger AHand::getFinger(void * ptr)
{
	return NONE;
}

FVector AHand::getFingerLocation(finger f)
{
	return FVector();
}

void AHand::processLeapData(FVector * data, FRotator rotation)
{
	if (data != nullptr)
		isEnabled = true;
}

AHand * AHand::build(bool left, AActor * owner)
{
	AHand::left = left;
	AHand *hand = owner->GetWorld()->SpawnActor<AHand>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (hand != nullptr)
		hand->AttachRootComponentToActor(owner);
	return hand;
}
