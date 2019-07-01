// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Menu.generated.h"

UCLASS()
class DP_API AMenu : public AActor
{
	GENERATED_BODY()

private:
	UStaticMeshComponent * iPad;
	UBoxComponent * buttons[4];
	AActor *games[4] = { nullptr, nullptr, nullptr, nullptr };
	char closestGame;

	char nextGame;
	float timeLeft;

	UMaterial *gray;
	UMaterial *green;
	UMaterial *yellow;
	UMaterial *red;

	bool isHidden = false;

	void changeBacklight();

	// Press button
	UFUNCTION()
	void Press(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	AMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void hide();
	void show();

};
