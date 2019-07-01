// Fill out your copyright notice in the Description page of Project Settings.

#include "HockeyStickWithHelpers.h"

AHockeyStickWithHelpers::AHockeyStickWithHelpers() : AHockeyStick()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> Primary(TEXT("Material'/Game/DP/Materials/Hockey/StickHelpers/Transparent_Red.Transparent_Red'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Secondary(TEXT("Material'/Game/DP/Materials/Hockey/StickHelpers/Transparent_Black.Transparent_Black'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	static FVector locations[2] = { FVector(0.0f, 0.0f, 59.5f), FVector(0.0f, 0.0f, 27.0f) };
	static FVector scales[2] = { FVector(0.162f, 0.145f, 0.31f), FVector(0.27f, 0.2355f, 0.98f) };

	if (Cube.Succeeded())
	{
		for (int i = 0; i < 2; i++)
		{
			FString id = FString("Helper") + FString::FromInt(i);
			areas[i] = CreateDefaultSubobject<UStaticMeshComponent>(*id);
			areas[i]->SetupAttachment(RootComponent);
			areas[i]->SetRelativeLocation(locations[i]);
			areas[i]->SetRelativeScale3D(scales[i]);
			areas[i]->SetStaticMesh(Cube.Object);
		}

		areas[0]->SetMaterial(0, Primary.Object);
		areas[1]->SetMaterial(0, Secondary.Object);
	}
}
