// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef DP_LEAP_BRIDGE
#define DP_LEAP_BRIDGE

#include "CoreMinimal.h"

/**
 * 
 */
class DP_API LeapBridge
{
private:
	LeapBridge();
public:
	static LeapBridge *getInstance();

	FVector * getLoactions();
	~LeapBridge();
};

#endif