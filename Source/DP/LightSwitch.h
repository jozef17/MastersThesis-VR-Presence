// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "LightSwitch.generated.h"

UCLASS()
class DP_API ALightSwitch : public AActor
{
	GENERATED_BODY()

private:
	// Initialization
	void Create();
	void Attach();
	void Setup();

	// Components
	bool on = false;

	UTextRenderComponent *ON;
	UTextRenderComponent *OFF;

	UBoxComponent *OnCollision;
	UBoxComponent *OffCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ALightSwitch();

	UFUNCTION(BlueprintPure)
		bool IsTurnedOn() { return on; }

	// Caled when turned on / off
	UFUNCTION(BlueprintNativeEvent)
		void TurnOn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintNativeEvent)
		void TurnOff(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Visual Components
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseLeftBorder;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseRightBorder;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseTopBorder;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseBottomBorder;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseBottomInside;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchBaseBottom;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *LigthswitchKey;
};
