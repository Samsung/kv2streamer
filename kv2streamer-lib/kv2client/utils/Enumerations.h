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

/******************************
 * Defines all the enumerations
 * that are defined in the K4W2 
 * SDK.
 * 
 * Based on SDK Version: June 2014
 ******************************/

#pragma once

#include <vector>

typedef enum _JointType
{
	JointType_SpineBase 	= 0,
	JointType_SpineMid 		= 1,
	JointType_Neck			= 2,
	JointType_Head			= 3,
	JointType_ShoulderLeft	= 4,
	JointType_ElbowLeft		= 5,
	JointType_WristLeft		= 6,
	JointType_HandLeft		= 7,
	JointType_ShoulderRight	= 8,
	JointType_ElbowRight	= 9,
	JointType_WristRight	= 10,
	JointType_HandRight		= 11,
	JointType_HipLeft		= 12,
	JointType_KneeLeft		= 13,
	JointType_AnkleLeft		= 14,
	JointType_FootLeft		= 15,
	JointType_HipRight		= 16,
	JointType_KneeRight		= 17,
	JointType_AnkleRight	= 18,
	JointType_FootRight		= 19,
	JointType_SpineShoulder	= 20,
	JointType_HandTipLeft	= 21,
	JointType_ThumbLeft		= 22,
	JointType_HandTipRight	= 23,
	JointType_ThumbRight	= 24,	
	JointType_Count 		= (JointType_ThumbRight+1)
} JointType;

typedef enum _TrackingState
{
	TrackingState_NotTracked	= 0,
	TrackingState_Inferred		= 1,
	TrackingState_Tracked		= 2
} TrackingState;

typedef enum _TrackingConfidence
{
	TrackingConfidence_Low	= 0,
	TrackingConfidence_High	= 1
} TrackingConfidence;

typedef enum _HandState
{
	HandState_Unknown 		= 0,
	HandState_NotTracked 	= 1,
	HandState_Open 			= 2,
	HandState_Closed 		= 3,
	HandState_Lasso 		= 4
} HandState;

typedef enum _DetectionResult
{
	DetectionResult_Unknown	= 0,
	DetectionResult_No		= 1,
	DetectionResult_Maybe	= 2,
	DetectionResult_Yes		= 3
} DetectionResult;

typedef enum _Activity
{
	Activity_EyeLeftClosed	= 0,
	Activity_EyeRightClosed	= 1,
	Activity_MouthOpen		= 2,
	Activity_MouthMoved		= 3,
	Activity_LookingAway	= 4,
	Activity_Count 			= (Activity_LookingAway+1)
} Activity;

typedef enum _Appearance
{
	Appearance_WearingGlasses 	= 0,
	Appearance_Count			= (Appearance_WearingGlasses+1)
} Appearance;

typedef enum _Expression
{
	Expression_Neutral	= 0,
	Expression_Happy	= 1,
	Expression_Count	= (Expression_Happy+1)
} Expression;

typedef struct _Bone
{
	JointType EndJoint1;
	JointType EndJoint2;
} Bone;