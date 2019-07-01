// Fill out your copyright notice in the Description page of Project Settings.

#include "Body.h"
#include "Engine.h"

ABody * ABody::build(AActor *owner)
{
	ABody *body = owner->GetWorld()->SpawnActor<ABody>(FVector(0.0f), FRotator(0.0f), FActorSpawnParameters());
	if (body != nullptr)
		body->AttachRootComponentToActor(owner);
	return body;
}

// Sets default values
ABody::ABody()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}
