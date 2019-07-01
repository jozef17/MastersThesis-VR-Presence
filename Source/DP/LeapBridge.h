// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef DP_LEAP_BRIDGE
#define DP_LEAP_BRIDGE

#include "CoreMinimal.h"
#include "Const.h"

/**
 * 
 */
class DP_API LeapBridge
{
private:
	FVector leftHand[LEAP_TRACKED_POINTS];
	FVector rightHand[LEAP_TRACKED_POINTS];
	int64_t lastLeft = 0;
	int64_t lastRight = 0;

	LeapBridge();
	FVector convert(bool VR, float leapX, float leapY, float leapZ);
public:
	static LeapBridge *getInstance();

	FVector * getLoactions(bool left,  bool VR);

};

#endif
