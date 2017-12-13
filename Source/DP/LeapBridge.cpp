// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapBridge.h"
#include "Leap.h"
#include "Engine.h"

Leap::Controller controller;

LeapBridge::LeapBridge()
{
}

LeapBridge * LeapBridge::getInstance()
{
	static LeapBridge instance;
	return &instance;
}

FVector * LeapBridge::getLoactions()
{
	Leap::Frame frame = controller.frame();
	Leap::HandList hands = frame.hands();

	return nullptr;
}

LeapBridge::~LeapBridge()
{
}
