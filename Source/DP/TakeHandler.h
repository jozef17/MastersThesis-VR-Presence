// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Hand.h"
#include "TakeHandler.generated.h"

class TakeRules
{
private:
	int numFingers = 2;
	bool simulatePhysics = true;
	AActor *parent = nullptr;
	AActor *owner = nullptr;
	UPrimitiveComponent * obj;
	USceneComponent * parentComponent = nullptr;
	TArray<UShapeComponent *> collisions;
	TArray<UShapeComponent *> secondaryCollisions;

public:
	TakeRules(UPrimitiveComponent *obj, UShapeComponent *collision);

	// Setters for rules
	TakeRules *setMinFingers(int numFingers);
	TakeRules *addCollision(UShapeComponent *collision);
	TakeRules *addSecondaryCollision(UShapeComponent *collision);
	TakeRules *setParentActor(AActor *parent);
	TakeRules *disablePhysics();

	// Getters for applying rules
	int gGetNumFingers();
	int getNumCollisions();
	int getNumSecondaryCollisions();
	bool isSimulatingPhysics();
	AActor *getOwner();
	AActor *getParentActor();
	USceneComponent *getParentComponent();
	UShapeComponent *getCollision(int i);
	UShapeComponent *getSecondaryCollision(int i);
	UPrimitiveComponent *getObject();

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DP_API UTakeHandler : public USceneComponent
{
	GENERATED_BODY()

private:
	TakeRules * rules = nullptr;

	// One hand
	unsigned char overlapingFingers[10];
	unsigned char hand[2] = { 0, 0 };
	unsigned char minFingers = 2;

	// Two hands
	AHand *registeredHands[2] = {nullptr, nullptr};
	unsigned int handParts[2] = { 0, 0 };
	char primaryHand = -1;

	bool taken = false;
	bool enabled = true;

	// One Hand
	// Take object
	UFUNCTION()
	void Take(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Release object
	UFUNCTION()
	void Release(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Two Hands
	UFUNCTION()
	void RegisterHand(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UnregisterHand(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Sets default values for this component's properties
	UTakeHandler();
	~UTakeHandler();

	void init(TakeRules *rules);

	void enable();
	void disable();
	void disablePhysics();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

};
