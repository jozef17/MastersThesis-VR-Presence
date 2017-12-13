// Fill out your copyright notice in the Description page of Project Settings.

#include "KinectBridge.h"
#include <Kinect.h>

// Kinect variables
IKinectSensor *sensor = nullptr;				// Kinect sensor
IBodyFrameReader *bodyFrameReader = nullptr;	// Kinect data source
IBodyFrameSource *bodyFrameSource = nullptr;

// Data from kinect
Joint joints[JointType_Count];					// Absolute joint position

template<class Interface>
static inline void safeRelease(Interface *&interfaceToRelease)
{
	if (interfaceToRelease != nullptr)
	{
		interfaceToRelease->Release();
		interfaceToRelease = nullptr;
	}
}



bool KinectBridge::getKinectData()
{
	bool found = false;
	IBodyFrame *bodyFrame = nullptr;
	HRESULT hr = bodyFrameReader->AcquireLatestFrame(&bodyFrame);

	if (SUCCEEDED(hr))
	{
		IBody *bodies[BODY_COUNT] = { 0 };
		hr = bodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);

		if (SUCCEEDED(hr))
		{
			for (unsigned int bodyIndex = 0; bodyIndex < BODY_COUNT; bodyIndex++)
			{
				BOOLEAN isTracked = false;
				IBody *body = bodies[bodyIndex];
				hr = body->get_IsTracked(&isTracked);

				if (FAILED(hr) || isTracked == false)
					continue;

				hr = body->GetJoints(_countof(joints), joints);

				if (SUCCEEDED(hr))
				{
					found = true;
					break;
				}
			}

			for (unsigned int bodyIndex = 0; bodyIndex < _countof(bodies); bodyIndex++)
				safeRelease(bodies[bodyIndex]);

			safeRelease(bodyFrame);
		}
	}

	if (found)
		return true;
	return false;
}

KinectBridge * KinectBridge::getInstance()
{
	static KinectBridge instance;
	return &instance;
}

bool KinectBridge::connect()
{
	HRESULT hr = GetDefaultKinectSensor(&sensor);

	if (SUCCEEDED(hr))
	{
		hr = sensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = sensor->get_BodyFrameSource(&bodyFrameSource);

			if (SUCCEEDED(hr))
				hr = bodyFrameSource->OpenReader(&bodyFrameReader);

			safeRelease(bodyFrameSource);
		}
	}

	if (sensor == nullptr || FAILED(hr))
		return false;
	return true;
}

FVector * KinectBridge::getLoactions()
{
	// Get data from Kinect
	if (!getKinectData())
		return nullptr;

	minZ = 0;

	// Convert data from Kinect to UE4
	for (int i = 0; i < NUM_JOINTS; i++) {
		bodyData[i].Set(-joints[i].Position.Z * 100, joints[i].Position.X * 100, joints[i].Position.Y * 100);

		if (bodyData[i].Z < bodyData[minZ].Z)
			minZ = i;
	}

	return bodyData;
}

int KinectBridge::lastMinZ()
{
	return minZ;
}
