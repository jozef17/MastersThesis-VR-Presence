// Fill out your copyright notice in the Description page of Project Settings.

#include "TakeHandler.h"
#include "Engine.h"

#include <limits>

float EPS = std::numeric_limits<float>::epsilon();

void UTakeHandler::Take(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (rules == nullptr)
		return;

	if ((OtherActor != nullptr) && (OtherComp != nullptr))
		if (OtherActor->GetClass()->IsChildOf(AHand::StaticClass()))
		{
			finger f = ((AHand *)OtherActor)->getFinger(OtherComp);
			if (f == NONE)
				return;

			overlapingFingers[f]++;

			if (overlapingFingers[f] == 1)
			{
				char h = ((AHand *)OtherActor)->getHand();
				hand[h]++;

				if (!enabled)
					return;

				if (hand[h] >= minFingers && overlapingFingers[5 * h] > 0 && !taken)
				{
					taken = true;
					rules->getObject()->SetSimulatePhysics(false);
					rules->getObject()->AttachToComponent(rules->getParentComponent(), FAttachmentTransformRules::KeepWorldTransform);
					rules->getOwner()->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

					if (OtherActor == registeredHands[0])
						primaryHand = 0;
					else if (OtherActor == registeredHands[1])
						primaryHand = 1;
				}
			}
		}
}

void UTakeHandler::Release(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (rules == nullptr)
		return;

	if ((OtherActor != nullptr) && (OtherComp != nullptr))
		if (OtherActor->GetClass()->IsChildOf(AHand::StaticClass()))
		{
			finger f = ((AHand *)OtherActor)->getFinger(OtherComp);
			if (f == NONE)
				return;

			overlapingFingers[f]--;

			if (overlapingFingers[f] == 0)
			{
				char h = ((AHand *)OtherActor)->getHand();
				hand[h]--;

				if (!enabled)
					return;

				if ((hand[h] < minFingers || overlapingFingers[5 * h] == 0) && taken)
				{
					taken = false;
					primaryHand = -1;
					rules->getOwner()->DetachRootComponentFromParent(true);

					if (rules->getParentActor() != nullptr)
						rules->getOwner()->AttachToActor(rules->getParentActor(), FAttachmentTransformRules::KeepWorldTransform);

					rules->getObject()->SetSimulatePhysics(rules->isSimulatingPhysics());
				}
			}
			else if (overlapingFingers[f] < 0)
				overlapingFingers[f] = 0;
		}
}

void UTakeHandler::RegisterHand(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (((OtherActor != nullptr) && (OtherComp != nullptr)) && (OtherActor->GetClass()->IsChildOf(AHand::StaticClass())))
	{
		char h = ((AHand *)OtherActor)->getHand();
		if (registeredHands[h] == nullptr)
			registeredHands[h] = (AHand *)OtherActor;
		if (registeredHands[h] != OtherActor)
		{
			registeredHands[h] = (AHand *)OtherActor;
			handParts[h] = 0;
		}
		handParts[h]++;
	}
}

void UTakeHandler::UnregisterHand(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (((OtherActor != nullptr) && (OtherComp != nullptr)) && (OtherActor->GetClass()->IsChildOf(AHand::StaticClass())))
	{
		char h = ((AHand *)OtherActor)->getHand();
		if (registeredHands[h] != OtherActor)
			return;

		handParts[h]--;
		if (handParts[h] == 0)
			registeredHands[h] = nullptr;
	}
}



UTakeHandler::UTakeHandler()
{
	PrimaryComponentTick.bCanEverTick = true;

	for (int i = 0; i < 10; i++)
		overlapingFingers[i] = 0;
}

UTakeHandler::~UTakeHandler()
{
	if (rules != nullptr)
		delete rules;
}

void UTakeHandler::init(TakeRules *rules)
{
	if (rules == nullptr)
		return;

	if (this->rules != nullptr)
		delete this->rules;

	this->rules = rules;

	// Init Component
	SetupAttachment(rules->getObject());

	minFingers = rules->gGetNumFingers();

	// Init primary collisions
	for (int i = 0; i < rules->getNumCollisions(); i++)
	{
		rules->getCollision(i)->bGenerateOverlapEvents = true;
		rules->getCollision(i)->OnComponentBeginOverlap.AddDynamic(this, &UTakeHandler::Take);
		rules->getCollision(i)->OnComponentEndOverlap.AddDynamic(this, &UTakeHandler::Release);
	}

	// Init secondary collisions (second hand)
	for (int i = 0; i < rules->getNumSecondaryCollisions(); i++)
	{
		rules->getSecondaryCollision(i)->bGenerateOverlapEvents = true;
		rules->getSecondaryCollision(i)->OnComponentBeginOverlap.AddDynamic(this, &UTakeHandler::RegisterHand);
		rules->getSecondaryCollision(i)->OnComponentEndOverlap.AddDynamic(this, &UTakeHandler::UnregisterHand);
	}

}

void UTakeHandler::enable()
{
	if (rules == nullptr)
		return;

	enabled = true;
	rules->getObject()->SetSimulatePhysics(rules->isSimulatingPhysics());
}

void UTakeHandler::disable()
{
	if (rules == nullptr)
		return;

	enabled = false;

	if (taken)
		taken = false;

	rules->getOwner()->DetachRootComponentFromParent(true);
	rules->getObject()->AttachToComponent(rules->getParentComponent(), FAttachmentTransformRules::KeepWorldTransform);

	if (rules->getParentActor() != nullptr)
		rules->getOwner()->AttachToActor(rules->getParentActor(), FAttachmentTransformRules::KeepWorldTransform);

	rules->getObject()->SetSimulatePhysics(rules->isSimulatingPhysics());
}

void UTakeHandler::disablePhysics()
{
	if (rules == nullptr)
		return;

	enabled = false;
	taken = false;

	rules->getOwner()->DetachRootComponentFromParent(true);
	rules->getObject()->SetSimulatePhysics(false);
	rules->getObject()->AttachToComponent(rules->getParentComponent(), FAttachmentTransformRules::KeepWorldTransform);

	if (rules->getParentActor() != nullptr)
		rules->getOwner()->AttachToActor(rules->getParentActor(), FAttachmentTransformRules::KeepWorldTransform);
}

void UTakeHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if ((rules == nullptr) || !taken || (rules->getNumSecondaryCollisions() <= 0) || (primaryHand == -1)
		|| (registeredHands[0] == nullptr) || (registeredHands[1] == nullptr))
		return;

	FVector loc = rules->getOwner()->GetActorLocation();
	FVector primary = primaryHand == 0 ? registeredHands[0]->GetActorLocation() : registeredHands[1]->GetActorLocation();
	FVector secondary = primaryHand == 0 ? registeredHands[1]->GetActorLocation() : registeredHands[0]->GetActorLocation();

	rules->getOwner()->SetActorRotation(FRotationMatrix::MakeFromZ(primary - secondary).Rotator());

	float radius = FVector::Dist(loc, primary);

	FVector oc(0.0f);
	FVector direction = secondary - primary;
	float a = FVector::DotProduct(direction, direction);
	float b = FVector::DotProduct(oc, direction);
	float c = FVector::DotProduct(oc, oc) - radius * radius;
	float dis = b * b - a * c;

	if (dis > 0)
	{
		float e = sqrt(dis);
		float t = (-b - e) / a;

		if (t > EPS)
		{
			FVector loc = primary + t * direction;
			rules->getOwner()->SetActorLocation(loc);
			return;
		}
		else
		{
			t = (-b + e) / a;

			if (t > EPS) {
				FVector loc = primary + t * direction;
				rules->getOwner()->SetActorLocation(loc);
				return;
			}
		}
	}
}



TakeRules::TakeRules(UPrimitiveComponent * obj, UShapeComponent * collision)
{
	this->obj = obj;
	collisions.Add(collision);
	owner = obj->GetOwner();

	TArray < USceneComponent * > Parents;
	obj->GetParentComponents(Parents);

	if (Parents.Num() > 0)
		parentComponent = Parents[0];
}

TakeRules *TakeRules::setMinFingers(int numFingers)
{
	if (numFingers > 1 && numFingers < 6)
		this->numFingers = numFingers;
	return this;
}

TakeRules *TakeRules::addCollision(UShapeComponent * collision)
{
	collisions.Add(collision);
	return this;
}

TakeRules *TakeRules::addSecondaryCollision(UShapeComponent * collision)
{
	secondaryCollisions.Add(collision);
	return this;
}

TakeRules *TakeRules::setParentActor(AActor * parent)
{
	this->parent = parent;
	return this;
}

TakeRules * TakeRules::disablePhysics()
{
	simulatePhysics = false;
	return this;
}

int TakeRules::gGetNumFingers()
{
	return numFingers;
}

int TakeRules::getNumCollisions()
{
	return collisions.Num();
}

int TakeRules::getNumSecondaryCollisions()
{
	return secondaryCollisions.Num();
}

bool TakeRules::isSimulatingPhysics()
{
	return simulatePhysics;
}

UPrimitiveComponent * TakeRules::getObject()
{
	return obj;
}

USceneComponent * TakeRules::getParentComponent()
{
	return parentComponent;
}

UShapeComponent * TakeRules::getCollision(int i)
{
	if (i < collisions.Num())
		return collisions[i];
	return nullptr;
}

UShapeComponent * TakeRules::getSecondaryCollision(int i)
{
	if (i < secondaryCollisions.Num())
		return secondaryCollisions[i];
	return nullptr;
}

AActor * TakeRules::getParentActor()
{
	return parent;
}

AActor * TakeRules::getOwner()
{
	return owner;
}
