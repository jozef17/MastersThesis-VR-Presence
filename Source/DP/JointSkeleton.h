// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserRepresentation.h"
#include "JointSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class DP_API AJointSkeleton : public AUserRepresentation
{
	GENERATED_BODY()
	
protected:
	virtual void setup() override;	// Sets up Hands & Body
	
};
