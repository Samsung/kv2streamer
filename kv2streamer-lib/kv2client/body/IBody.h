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

/**
 * @file 
 * @author Kevin Wong <k.wong2@partner.samsung.com, kevinwinatawong@gmail.com>
 * @date August 4th, 2014
 * 
 * @brief This file contains IBodyFrame class definition.
 * 
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <kv2client/utils/Structures.h>
#include <kv2client/utils/Enumerations.h>
#include <oscpack-wrapper/oscMessage.h>

namespace kv2s {

class IBodyFrame;

/**
 * @class IBody
 * @brief Manages body data and provides an API that mimics its K4W2 counterpart.
 * 
 * @todo add sample usage and references and what data it contains.
 */

class IBody
{
	
public:
	/* NOT IN KW42: returns the index used to access the array of BODY_COUNT = 6 IBody objects. UNVERIFIED GUESS: same index is used for body index */
	int getIndex();
	
	/* Returns whether this body is tracked or not. !!! Other members undefined if tracked=false*/
	bool get_IsTracked(BOOLEAN* tracked);
	
	/* Returns unique tracking ID of type ulonglong, not the same as index used for body index, not in order */
	bool get_TrackingId(UINT64* trackingId);
	
	// Hand
	bool get_HandLeftState	(HandState* handState);
	bool get_HandRightState	(HandState* handState);
	bool get_HandLeftConfidence	(TrackingConfidence* confidence);
	bool get_HandRightConfidence	(TrackingConfidence* confidence);
	
	// Joints
	bool GetJoints(UINT capacity_JointType_Count, Joint* joints);
	bool GetJointOrientations(UINT capacity_JointType_Count, JointOrientation* jointOrientations);

	// Lean Vector
	bool get_Lean(PointF* amount);
	bool get_LeanTrackingState(TrackingState* trackingState);

	// Activity
	bool GetActivityDetectionResults(UINT capacity_Activity_Count, DetectionResult* detectionResults);

	// Expression
	bool GetExpressionDetectionResults(UINT capacity_Expression_Count, DetectionResult* detectionResults);

	// Appearance
	bool GetAppearanceDetectionResults(UINT capacity_Appearance_Count, DetectionResult* detectionResults);
	
	// Miscellanea
	bool get_Engaged(DetectionResult* detectionResult);
	bool get_ClippedEdges(DWORD* clippedEdges);
	bool get_IsRestricted(BOOLEAN* isRestricted);

public:	
	~IBody();

private: // methods accessible to friend(s)
	friend class IBodyFrame;
	IBody();
	void UpdateBody(oscMessage* msg, int &index);

private: // helper methods and variables

	bool has_received_joints_;
	bool has_received_hands_;
	bool has_received_activity_;
	bool has_received_expression_;
	bool has_received_joint_orientations_;
	bool has_received_appearance_;
	bool has_received_lean_;
	bool has_received_miscellanous_info_;	

	void ReceiveJoints					(oscMessage* msg, int &index);
	void ReceiveHandStateAndConfidence	(oscMessage* msg, int &index);
	void ReceiveActivity				(oscMessage* msg, int &index);
	void ReceiveExpression				(oscMessage* msg, int &index);
	void ReceiveJointOrientations		(oscMessage* msg, int &index);
	void ReceiveAppearance				(oscMessage* msg, int &index);
	void ReceiveLean					(oscMessage* msg, int &index);
	void ReceiveMiscellaneousInfo		(oscMessage* msg, int &index);

private: // DATA
	// Joints
	std::vector<Joint> 				joints_;
	std::vector<JointOrientation> 	joint_orientations_;

	// Hand Data
	HandState				hand_right_state_;
	HandState				hand_left_state_;
	TrackingConfidence		hand_right_confidence_;
	TrackingConfidence		hand_left_confidence_;
	
	// Tracking ID
	UINT64	tracking_id_; 
	
	// Booleans
	int 	body_index_;
	bool 	is_tracked_;
	bool 	is_restricted_;
	
	// Lean
	PointF			lean_vector_;
	TrackingState	lean_tracking_state_;
	
	// DetectionResults
	DetectionResult	is_engaged_;
	DetectionResult	activity_detection_results_		[Activity_Count];
	DetectionResult	expression_detection_results_	[Expression_Count];
	DetectionResult	appearance_detection_results_	[Appearance_Count];
	
	// Misc
	DWORD clipped_edges_;
	
};
typedef boost::shared_ptr<IBody> IBodyPtr;

} // namespace kv2streamer