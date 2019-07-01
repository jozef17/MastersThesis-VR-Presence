// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapBridge.h"
#include "Leap.h"
#include "Const.h"
#include "Engine.h"

using namespace Leap;

Controller controller;

LeapBridge::LeapBridge()
{
	controller.setPolicy(Controller::POLICY_OPTIMIZE_HMD);
	controller.setPolicyFlags(Controller::POLICY_OPTIMIZE_HMD);

	for (int i = 0; i < LEAP_TRACKED_POINTS; i++)
	{
		leftHand[i] = FVector(0.0f);
		rightHand[i] = FVector(0.0f);
	}
}

FVector LeapBridge::convert(bool VR, float leapX, float leapY, float leapZ)
{
	if (VR)
		return FVector(leapY / 10.f, -leapX / 10.f, -leapZ / 10.f);
	return FVector(-leapZ / 10.f, leapX / 10.f, leapY / 10.f);
}

LeapBridge * LeapBridge::getInstance()
{
	static LeapBridge instance;

	controller.setPolicy(Controller::POLICY_OPTIMIZE_HMD);
	controller.setPolicyFlags(Controller::POLICY_OPTIMIZE_HMD);
	
	return &instance;
}

FVector * LeapBridge::getLoactions(bool left, bool VR)
{
	controller.setPolicy(Controller::POLICY_OPTIMIZE_HMD);
	controller.setPolicyFlags(Controller::POLICY_OPTIMIZE_HMD);

	FVector *ret = nullptr;
	Frame frame = controller.frame();
	HandList hands = frame.hands();

	if (left && lastLeft == frame.id())
		return nullptr;
	else if (lastRight == frame.id())
		return nullptr;

	// Find required hand
	for (int i = 0; i < hands.count(); i++)
	{
		Hand hand = hands[i];
		if ((left == true) && hand.isRight() || (left == false) && hand.isLeft())
			continue;

		ret = left ? leftHand : rightHand;

		// Set Hand Position
		ret[0] = convert(VR, hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);

		// For every finger on hand
		FingerList fingers = hand.fingers();
		for (int j = 0; j < fingers.count(); j++)
		{
			Vector boneBeginning = fingers[j].bone(static_cast<Bone::Type>(0)).prevJoint();
			ret[j * 5 + 1] = convert(VR, boneBeginning.x, boneBeginning.y, boneBeginning.z) - ret[0];

			// For every bone on hand
			for (int b = 0; b < 4; b++)
			{
				Vector boneEnd = fingers[j].bone(static_cast<Bone::Type>(b)).nextJoint();
				ret[j * 5 + 2 + b] = convert(VR, boneEnd.x, boneEnd.y, boneEnd.z) - ret[0];
			}
		}
		break;
	}

	if (left)
		lastLeft = frame.id();
	else
		lastRight = frame.id();

	return ret;
}
