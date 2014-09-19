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

#include <string>
#include <kv2client/SharedConfig.h>
#include "IBody.h"

/**************************************************
 * 	K4W2 API and other public API Implementation
 *************************************************/
namespace kv2s {

bool IBody::get_IsTracked(BOOLEAN* tracked)
{
	(*tracked) = is_tracked_;
	return true;
}

bool IBody::get_TrackingId(UINT64* _trackingId)
{
	if (!is_tracked_) return false;
	(*_trackingId) = tracking_id_;
	return true;
}

// Hand
bool IBody::get_HandLeftState	(HandState* handState)
{
	if (!is_tracked_ || !has_received_hands_) return false;
	(*handState) = hand_left_state_;
	return true;
}
bool IBody::get_HandRightState	(HandState* handState)
{
	if (!is_tracked_ || !has_received_hands_) return false;
	(*handState) = hand_right_state_;
	return true;

}
bool IBody::get_HandLeftConfidence	(TrackingConfidence* confidence)
{
	if (!is_tracked_ || !has_received_hands_) return false;
	(*confidence) = hand_left_confidence_;
	return true;
}
bool IBody::get_HandRightConfidence	(TrackingConfidence* confidence)
{
	if (!is_tracked_ || !has_received_hands_) return false;
	(*confidence) = hand_right_confidence_;
	return true;
}

// Joints
bool IBody::GetJoints(UINT capacity, Joint* _joints)
{
	if (!is_tracked_ || !has_received_joints_) return false;
	
	for (unsigned int i = 0; i < capacity; i++)
	{
		_joints[i] = joints_[i];
	}
	return true;
}

bool IBody::GetJointOrientations(UINT capacity, JointOrientation* _jointOrientations)
{
	if (!is_tracked_ || !has_received_joint_orientations_) return false;
	
	for (unsigned int i = 0; i < capacity; i++)
	{
		_jointOrientations[i] = joint_orientations_[i];
	}
	return true;
}

// Lean Vector
bool IBody::get_Lean(PointF* amount)
{
	if (!is_tracked_ || !has_received_lean_) return false;
	(*amount) = lean_vector_;
	return true;
}
bool IBody::get_LeanTrackingState(TrackingState* trackingState)
{
	if (!is_tracked_ || !has_received_lean_) return false;
	(*trackingState) = lean_tracking_state_;
	return true;
}

// Activity
bool IBody::GetActivityDetectionResults(UINT capacity, DetectionResult* detectionResults)
{
	if (!is_tracked_ || !has_received_activity_) return false;
	for (unsigned int i = 0; i < capacity; i++)
	{
		detectionResults[i] = activity_detection_results_[i];
	}
	return true;
}

// Expression
bool IBody::GetExpressionDetectionResults(UINT capacity, DetectionResult* detectionResults)
{
	if (!is_tracked_ || !has_received_expression_) return false;
	for (unsigned int i = 0; i < capacity; i++)
	{
		detectionResults[i] = expression_detection_results_[i];
	}
	return true;
}

// Appearance
bool IBody::GetAppearanceDetectionResults(UINT capacity, DetectionResult* detectionResults)
{
	if (!is_tracked_ || !has_received_appearance_) return false;
	for (unsigned int i = 0; i < capacity; i++)
	{
		detectionResults[i] = appearance_detection_results_[i];
	}
	return true;
}

// Misc
bool IBody::get_Engaged(DetectionResult* detectionResult)
{
	if (!is_tracked_ || !has_received_miscellanous_info_) return false;
	(*detectionResult) = is_engaged_;
	return true;
}

bool IBody::get_ClippedEdges(DWORD* _clippedEdges)
{
	if (!is_tracked_ || !has_received_miscellanous_info_) return false;
	(*_clippedEdges) = clipped_edges_;
	return true;
}

bool IBody::get_IsRestricted(BOOLEAN* _isRestricted)
{
	if (!is_tracked_ || !has_received_miscellanous_info_) return false;
	(*_isRestricted) = is_restricted_;
	return true;
}

// Body Index
int IBody::getIndex()
{
	return body_index_;
}

/**************************************************
 * 	Helper methods
 *************************************************/
 
IBody::IBody()
: is_tracked_(false)
{
	// Initialize 25 joints_ and joint orientations
	for (unsigned i = 0; i < JointType_Count; ++i)
	{
		Joint joint;
		joint.JointType 	= (JointType) i;
		joint.Position.X  	= 0.f;
		joint.Position.Y	= 0.f;
		joint.Position.Z	= 0.f;
		joint.TrackingState	= TrackingState_NotTracked; 
		joints_.push_back(joint);
		
		JointOrientation joint_orientation;
		joint_orientation.JointType = (JointType) i;
		joint_orientation.Orientation.x = 0.f;
		joint_orientation.Orientation.y = 0.f;
		joint_orientation.Orientation.z = 0.f;
		joint_orientation.Orientation.w = 0.f;
		joint_orientations_.push_back(joint_orientation);
	}
}

IBody::~IBody()
{
}

void IBody::UpdateBody(oscMessage* msg, int &i)
{
	body_index_ = msg->GetArgumentAtIndex(i++).GetIntegerValue();
	is_tracked_	= msg->GetArgumentAtIndex(i++).GetBooleanValue();
	
	if (is_tracked_)
	{
		tracking_id_ = std::strtoull(msg->GetArgumentAtIndex(i++).GetStringValue().c_str(), 0, 10);

		has_received_joints_				= false;
		has_received_hands_					= false;
		has_received_activity_				= false;
		has_received_expression_			= false;
		has_received_joint_orientations_	= false;
		has_received_appearance_			= false;
		has_received_lean_					= false;
		has_received_miscellanous_info_		= false;

		while(msg->GetArgumentAtIndex(i).GetStringValue() != END_OF_BODY_TAG)
		{
			std::string tag = msg->GetArgumentAtIndex(i++).GetStringValue();
			if 		(tag == JOINT_MESSAGE_TAG)				{ReceiveJoints					(msg, i);}
			else if (tag == HAND_MESSAGE_TAG)				{ReceiveHandStateAndConfidence	(msg, i);}
			else if (tag == ACTIVITY_MESSAGE_TAG)			{ReceiveActivity				(msg, i);}
			else if (tag == EXPRESSION_MESSAGE_TAG)			{ReceiveExpression				(msg, i);}
			else if (tag == JOINT_ORIENTATION_MESSAGE_TAG)	{ReceiveJointOrientations		(msg, i);}
			else if (tag == APPEARANCE_MESSAGE_TAG)			{ReceiveAppearance				(msg, i);}
			else if (tag == LEAN_MESSAGE_TAG)				{ReceiveLean					(msg, i);}
			else if (tag == MISCELLANEA_MESSAGE_TAG)		{ReceiveMiscellaneousInfo		(msg, i);}
			else {std::cout << "Error: Unrecognized OSC message tag: (" << tag << ")!\n";}
		} i++;
	}
}

void IBody::ReceiveJoints(oscMessage* msg, int &i)
{
	for (unsigned j = 0; j < JointType_Count; j++)
	{
		joints_[j].Position.X = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joints_[j].Position.Y = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joints_[j].Position.Z = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joints_[j].TrackingState = (TrackingState) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	}
	has_received_joints_ = true;
}
void IBody::ReceiveJointOrientations(oscMessage* msg, int &i)
{
	for (unsigned j = 0; j < JointType_Count; j++)
	{
		joint_orientations_[j].Orientation.x = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joint_orientations_[j].Orientation.y = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joint_orientations_[j].Orientation.z = msg->GetArgumentAtIndex(i++).GetFloatValue();
		joint_orientations_[j].Orientation.w = msg->GetArgumentAtIndex(i++).GetFloatValue();
	}
	has_received_joint_orientations_ = true;
}

void IBody::ReceiveHandStateAndConfidence(oscMessage* msg, int &i)
{
	hand_left_state_ 	= (HandState) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	hand_right_state_ 	= (HandState) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	
	hand_left_confidence_	= (TrackingConfidence) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	hand_right_confidence_	= (TrackingConfidence) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	
	has_received_hands_ = true;
}

void IBody::ReceiveActivity(oscMessage* msg, int &i)
{
	for (int j = 0; j < Activity_Count; j++)
	{
		activity_detection_results_[j] = (DetectionResult) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	}
	has_received_activity_ = true;
}

void IBody::ReceiveExpression(oscMessage* msg, int &i)
{
	for (int j = 0; j < Expression_Count; j++)
	{
		expression_detection_results_[j] = (DetectionResult) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	}
	has_received_expression_ = true;
}

void IBody::ReceiveAppearance(oscMessage* msg, int &i)
{
	for (int j = 0; j < Appearance_Count; j++)
	{
		appearance_detection_results_[j] = (DetectionResult) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	}
	has_received_appearance_ = true;
}

void IBody::ReceiveLean(oscMessage* msg, int &i)
{
	lean_vector_.X = msg->GetArgumentAtIndex(i++).GetFloatValue();
	lean_vector_.Y = msg->GetArgumentAtIndex(i++).GetFloatValue();
	lean_tracking_state_ = (TrackingState) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	
	has_received_lean_ = true;
}

void IBody::ReceiveMiscellaneousInfo(oscMessage* msg, int &i)
{
	is_engaged_ = (DetectionResult) msg->GetArgumentAtIndex(i++).GetIntegerValue();
	clipped_edges_ = (DWORD) std::strtoul(msg->GetArgumentAtIndex(i++).GetStringValue().c_str(), 0, 10);
	is_restricted_ = msg->GetArgumentAtIndex(i++).GetBooleanValue(); 
	
	has_received_miscellanous_info_ = true;
}

} // namespace kv2streamer