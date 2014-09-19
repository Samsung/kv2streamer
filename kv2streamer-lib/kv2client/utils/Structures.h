/*
 * Copyright (c) 2014 Samsung Electronics Corporation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <kv2client/utils/Enumerations.h>

typedef long				HRESULT;
#define S_OK				((HRESULT)0L)
#define SUCCEEDED(hr)		(((HRESULT)hr) >= 0)
#define FAILED(hr) 			(((HRESULT)hr) < 0)

typedef unsigned char		BYTE;
typedef BYTE				BOOLEAN;
typedef unsigned long long	UINT64;
typedef unsigned int		UINT;

typedef signed long long	INT64;
typedef INT64				TIMESPAN;	

typedef unsigned long		DWORD;

typedef struct _CameraSpacePoint
{
	float X;
	float Y;
	float Z;
} CameraSpacePoint;

typedef struct _ColorSpacePoint
{
	float X;
	float Y;
} ColorSpacePoint;

typedef struct _DepthSpacePoint
{
	float X;
	float Y;
} DepthSpacePoint;

typedef struct _Joint
{
	_JointType 			JointType;
	CameraSpacePoint 	Position;
	_TrackingState		TrackingState;	
} Joint;

typedef struct _Vector4
{
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct _JointOrientation
{
	_JointType	JointType;
	Vector4		Orientation;
} JointOrientation;

typedef struct _PointF
{
	float X;
	float Y;
} PointF;




