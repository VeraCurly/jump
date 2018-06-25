// jump.cpp : main project file.

#include "stdafx.h"
#include <Kinect.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <ctime>
#include <windows.h>

using namespace System;
using namespace std;
//safe way of deleting a COM object
template<typename T>
void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }

// Kinect depth data source
IKinectSensor* sensor = nullptr;
IBodyFrameReader* bodyReader;
ICoordinateMapper* coordinateMapper;
int massiveY[5];

int minimum(int *arr, int size)
{
	int min;
	min = arr[0];
	for (int i = 0; i<size; i++)
	{
		if (arr[i]<min)
		{
			min = arr[i];
		}
	}
	return min;
}

int timer(ColorSpacePoint colorpoint, ColorSpacePoint colorpoint1, ColorSpacePoint colorpoint2, int i)
{
	
	//cout << (colorpoint.Y + colorpoint1.Y + colorpoint2.Y)/3.0 << "\n";
	if (i == 4)
	{
		massiveY[4] = (colorpoint.Y + colorpoint1.Y + colorpoint2.Y) / 3.0;
		
		int min = minimum(massiveY, 5);
		if (massiveY[0] - min > 100 && massiveY[4] - min > 100)
		{
			cout << "Jump \n";
			i = 0; 
		}
		else 
		{
			for (int j = 0; j < 4; j++)
			{
				massiveY[j] = massiveY[j + 1];
			}
			i = 4;
		}
	}
	else if (i < 4)
	{
		massiveY[i] = (colorpoint.Y + colorpoint1.Y + colorpoint2.Y) / 3.0;
		i++;
	}
	return i;
}

int main()
{
	int massiveCount = 0;
	ofstream logFile;
	logFile.open("log.txt");
	if (!logFile.is_open())
	{
		cout << "Can't open a file\n";
		return 1;
	}
	logFile << "Joint_type," << "x,"  << "y" << "\n" << endl;

	DWORD dw1 = GetTickCount();

	HRESULT hr = S_OK;
	if (FAILED(GetDefaultKinectSensor(&sensor))) return 0;
	if (!sensor)
		return 1;

	sensor->Open();
	IBodyFrameSource* bodysource = nullptr;
	hr = sensor->get_BodyFrameSource(&bodysource);
	if (FAILED(hr))
		return 1;

	hr = bodysource->OpenReader(&bodyReader);
	if (FAILED(hr))
		return 1;

	while (1) {
		IBodyFrame* frame = nullptr;
		if (FAILED(bodyReader->AcquireLatestFrame(&frame)))
			continue;

		IBody* Body[BODY_COUNT] = { 0 };
		hr = frame->GetAndRefreshBodyData(BODY_COUNT, Body);
		logFile << "\n" << endl;
		SafeRelease(frame);

		if (FAILED(hr))
			continue;

		for (int count = 0; count < BODY_COUNT; count++)
		{
			BOOLEAN isTracking = false;
			hr = Body[count]->get_IsTracked(&isTracking);
			if (FAILED(hr) || !isTracking)
				continue;

			Joint joint[JointType::JointType_Count];
			hr = Body[count]->GetJoints(JointType::JointType_Count, joint);
			if (FAILED(hr))
				continue;
			
			/*if (joint[JointType_Head].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_Head].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile <<"Head,"<< colorpoint.X << "," << colorpoint.Y << endl;
				cout << "Head\n";
			}
			if (joint[JointType_ShoulderRight].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_ShoulderRight].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "ShoulderRight," << colorpoint.X << "," << colorpoint.Y <<endl;
			}
			if (joint[JointType_ShoulderLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_ShoulderLeft].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "ShoulderLeft," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_ElbowRight].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_ElbowRight].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "ElbowRight," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_ElbowLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_ElbowLeft].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "ElbowLeft," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_HandRight].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_HandRight].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "HandRight," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_HandLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_HandLeft].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "HandLeft," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_SpineBase].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_SpineBase].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "SpineBase," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_KneeRight].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_KneeRight].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "KneeRight," << colorpoint.X << "," << colorpoint.Y <<endl;
			}
			if (joint[JointType_KneeLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_KneeLeft].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "KneeLeft," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_FootRight].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_FootRight].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "FootRight," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			if (joint[JointType_FootLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				// this is the finger we need
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_FootLeft].Position;
				ColorSpacePoint colorpoint;
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				logFile << "FootLeft," << colorpoint.X << "," << colorpoint.Y << endl;
			}
			*/
			if (joint[JointType_Head].TrackingState != TrackingState::TrackingState_NotTracked && joint[JointType_ShoulderRight].TrackingState != TrackingState::TrackingState_NotTracked && joint[JointType_ShoulderLeft].TrackingState != TrackingState::TrackingState_NotTracked)
			{
				ColorSpacePoint colorpoint, colorpoint1, colorpoint2;
				hr = sensor->get_CoordinateMapper(&coordinateMapper);
				CameraSpacePoint point = joint[JointType_Head].Position;
			
				coordinateMapper->MapCameraPointToColorSpace(point, &colorpoint);
				CameraSpacePoint point1 = joint[JointType_ShoulderLeft].Position;
				coordinateMapper->MapCameraPointToColorSpace(point1, &colorpoint1);
				CameraSpacePoint point2 = joint[JointType_ShoulderRight].Position;
				coordinateMapper->MapCameraPointToColorSpace(point2, &colorpoint2);
				
				DWORD dw2 = GetTickCount();
				if (dw2-dw1 > 100) 
				{
					massiveCount = timer(colorpoint, colorpoint1, colorpoint2, massiveCount);
					dw1 = dw2;
				}
			}
			SafeRelease(Body[count]);
		}
	}
	logFile.close();
    return 0;
}
