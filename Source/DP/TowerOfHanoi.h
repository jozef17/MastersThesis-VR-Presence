// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "TowerOfHanoi.generated.h"

UCLASS()
class DP_API ATowerOfHanoi : public AActor
{
	GENERATED_BODY()
	
private:
	// Game Elements
	UStaticMeshComponent *Bottom;
	UStaticMeshComponent *Sticks[3];
	UStaticMeshComponent **Disks;

	void InitComponents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATowerOfHanoi();
	~ATowerOfHanoi();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "2", ClampMax = "6", UIMin = "2", UIMax = "6") )
		unsigned int NumOfDisks = 6;

	UPROPERTY(EditAnywhere)
		UMaterial *Material;

	UPROPERTY(EditAnywhere)
		UMaterial *Disk0Material;
	UPROPERTY(EditAnywhere)
		UMaterial *Disk1Material;
	UPROPERTY(EditAnywhere)
		UMaterial *Disk2Material;
	UPROPERTY(EditAnywhere)
		UMaterial *Disk3Material;
	UPROPERTY(EditAnywhere)
		UMaterial *Disk4Material;
	UPROPERTY(EditAnywhere)
		UMaterial *Disk5Material;
};
