// Fill out your copyright notice in the Description page of Project Settings.

#include "Forest.h"
#include "Engine.h"

// Sets default values
AForest::AForest()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ForestRoot"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree1(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree1.SM_tree1'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree2(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree2.SM_tree2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree3(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree3.SM_tree3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree4(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree4.SM_tree4'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree5(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree5.SM_tree5'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree6(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree6.SM_tree6'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree7(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree7.SM_tree7'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree8(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree8.SM_tree8'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree9(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree9.SM_tree9'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Tree10(TEXT("StaticMesh'/Game/VegetationPack/Meshes/Trees/SM_tree10.SM_tree10'"));

	int treeCounter = 0;
	for (int i = -5; i < 5; i++)
		for (int j = -5; j < 5; j++)
		{
			FString id = "Tree" + FString::FromInt(treeCounter++);
			UStaticMeshComponent *tree = CreateDefaultSubobject<UStaticMeshComponent>(FName(*id));
			tree->SetupAttachment(RootComponent);

			switch (FMath::RandRange(0, 9)) {
			case 0:
				tree->SetStaticMesh(Tree1.Object);
				break;
			case 1:
				tree->SetStaticMesh(Tree2.Object);
				break;
			case 2:
				tree->SetStaticMesh(Tree3.Object);
				break;
			case 3:
				tree->SetStaticMesh(Tree4.Object);
				break;
			case 4:
				tree->SetStaticMesh(Tree5.Object);
				break;
			case 5:
				tree->SetStaticMesh(Tree6.Object);
				break;
			case 6:
				tree->SetStaticMesh(Tree7.Object);
				break;
			case 7:
				tree->SetStaticMesh(Tree8.Object);
				break;
			case 8:
				tree->SetStaticMesh(Tree9.Object);
				break;
			case 9:
				tree->SetStaticMesh(Tree10.Object);
				break;
			}

			tree->SetRelativeRotation(FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));
			tree->SetRelativeLocation(FVector(i * 300.0f + FMath::RandRange(-75.0f, 75.0f), j * 300.0f + FMath::RandRange(-20.0f, 20.0f), 0.0f));
		}
}
