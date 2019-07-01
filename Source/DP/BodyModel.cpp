// Fill out your copyright notice in the Description page of Project Settings.

#include "BodyModel.h"
#include "ModelHand.h"
#include "ModelBody.h"

void ABodyModel::setup()
{
	body = AModelBody::build(this);
	leftHand = AModelHand::build(true, this);
	rightHand = AModelHand::build(false, this);
}
