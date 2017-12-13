// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef DP_KINECT_BRIDGE
#define DP_KINECT_BRIDGE

#include "CoreMinimal.h"

#define NUM_JOINTS 25

/**
 * 
 */
class DP_API KinectBridge
{
private:
	int minZ;		// Index of joint with the lovest Z coordinates
	FVector bodyData[NUM_JOINTS];	// UE4 mapped joint positions

	KinectBridge() {};
	bool getKinectData(/*char type*/);

public:
	static KinectBridge *getInstance();

	/**
	 * Connects to Kinect Device
	 */
	bool connect();

	/**
	 * Returns location data mapped to UE4
	 */
	FVector* getLoactions();

	/**
	 * Returns index of joint with the lowest Z coordinate
	 */
	int lastMinZ();
};

#endif // DP_KINECT_BRIDGE
