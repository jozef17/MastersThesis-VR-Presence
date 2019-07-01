// Fill out your copyright notice in the Description page of Project Settings.

#include "JointSkeleton.h"
#include "JointHand.h"
#include "JointBody.h"

void AJointSkeleton::setup()
{
	body = AJointBody::build(this);
	leftHand = AJointHand::build(true, this);
	rightHand = AJointHand::build(false, this);
}
