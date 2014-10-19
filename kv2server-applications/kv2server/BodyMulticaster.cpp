#include "BodyMulticaster.h"

#include "Utilities.h"
#include <kv2client/SharedConfig.h>

using namespace kv2s;

BodyMulticaster::BodyMulticaster(std::string IP, int port)
: sender(IP, port)
, AbstractMulticaster(IP, port)
, isSendingJoints			(false)
, isSendingHands			(false)
, isSendingActivity			(false)
, isSendingExpression		(false)
, isSendingJointOrientations(false)
, isSendingAppearance		(false)
, isSendingLean				(false)
, isSendingMiscInfo			(false)
{

	isSendingJoints = true;
	isSendingJointOrientations = true;
	isSendingHands = true;
	isSendingActivity = true;
	isSendingExpression = true;
	isSendingAppearance = true;
	isSendingLean = true;
	isSendingMiscInfo = true;
}


BodyMulticaster::~BodyMulticaster(void)
{
	SafeRelease(bodyFrameReader);
}

HRESULT BodyMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{
	HRESULT hr;
	
	IBodyFrameSource* bodyFrameSource = NULL;
	if (kinectSensor)
	{
		hr = kinectSensor->get_BodyFrameSource(&bodyFrameSource);

		if (SUCCEEDED(hr))
		{
			hr = bodyFrameSource->OpenReader(&bodyFrameReader);
		}
	}
	SafeRelease(bodyFrameSource);

	bodyFrameReader->SubscribeFrameArrived(&waitableHandle);

	return hr;
}

void BodyMulticaster::ProcessNewBodyFrame(IBodyFrame* newBodyFrame)
{
	IBody*	ppBodies[BODY_COUNT] = {NULL};
	HRESULT hr = newBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies); 
	if (!SUCCEEDED(hr)) return;

	oscMessage msg("body", oscMessage::LARGE_BUFFER);
	SendBodyFrame(newBodyFrame, msg);

	for (int i = 0; i < BODY_COUNT; i++)
	{
		if (ppBodies[i] != NULL)
		{
			SendBody(ppBodies[i], i, msg);
		}
	}
	sender.Send(msg);

	// release all the bodies
	for (int i = 0; i < BODY_COUNT; i++)
	{
		SafeRelease(ppBodies[i]);
	}
}

// @pre: GetAndRefreshBodyData called with ppBodies
void BodyMulticaster::SendBodyFrame(IBodyFrame * bodyFrame, oscMessage &msg)
{	
	TIMESPAN relativeTime;
	bodyFrame->get_RelativeTime(&relativeTime);

	Vector4 floorClipPlane;
	bodyFrame->get_FloorClipPlane(&floorClipPlane);

	msg.AppendString(std::to_string(relativeTime)); // [0] frame id
	msg.AppendFloat(floorClipPlane.x);
	msg.AppendFloat(floorClipPlane.y);
	msg.AppendFloat(floorClipPlane.z);
	msg.AppendFloat(floorClipPlane.w);
}

void BodyMulticaster::SendBody(IBody* body, int index, oscMessage &msg)
{
	BOOLEAN isBodyTracked = false;
	body->get_IsTracked(&isBodyTracked);
	
	msg.AppendInteger(index);			//1
	msg.AppendBoolean((bool)isBodyTracked);	//2
	
	if (isBodyTracked)
	{
		UINT64 trackingID; 
		body->get_TrackingId(&trackingID);
		
		msg.AppendString(std::to_string(trackingID));	//

		if (isSendingJoints)			SendJoints					(msg, body);
		if (isSendingHands)				SendHandStateAndConfidence	(msg, body);
		if (isSendingActivity)			SendActivity				(msg, body);
		if (isSendingExpression)		SendExpression				(msg, body);
		if (isSendingJointOrientations)	SendJointOrientations		(msg, body);
		if (isSendingAppearance)		SendAppearance				(msg, body);
		if (isSendingLean)				SendLean					(msg, body);
		if (isSendingMiscInfo)			SendMiscellaneousInfo		(msg, body);

		msg.AppendString(END_OF_BODY_TAG);
	}
}


void BodyMulticaster::SendJoints(oscMessage &msg, IBody* body)
{
	Joint joints[JointType_Count];
	HRESULT hr = body->GetJoints(JointType_Count, joints); // retrieve to placeholder

	if (!SUCCEEDED(hr)) return;

	msg.AppendString(JOINT_MESSAGE_TAG);

	for (unsigned i = 0; i < JointType_Count; i++)
	{
		msg.AppendFloat(joints[i].Position.X);
		msg.AppendFloat(joints[i].Position.Y);
		msg.AppendFloat(joints[i].Position.Z);
		msg.AppendInteger((int) joints[i].TrackingState);
	}
}

void BodyMulticaster::SendHandStateAndConfidence(oscMessage &msg, IBody* body)
{
	HRESULT				hr;
	HandState			handLeftState,		handRightState;
	TrackingConfidence	handLeftConfidence,	handRightConfidence;

	hr = body->get_HandLeftState(&handLeftState);		if (!SUCCEEDED(hr)) return;
	hr = body->get_HandRightState(&handRightState);		if (!SUCCEEDED(hr)) return;
		
	hr = body->get_HandLeftConfidence(&handLeftConfidence);		if (!SUCCEEDED(hr)) return;
	hr = body->get_HandRightConfidence(&handRightConfidence);	if (!SUCCEEDED(hr)) return;

	msg.AppendString(HAND_MESSAGE_TAG);

	msg.AppendInteger((int) handLeftState);
	msg.AppendInteger((int) handRightState);
	msg.AppendInteger((int) handLeftConfidence);
	msg.AppendInteger((int) handRightConfidence);
}

void BodyMulticaster::SendActivity(oscMessage &msg, IBody* body)
{
	DetectionResult detectionResults[Activity_Count];
	HRESULT hr = body->GetActivityDetectionResults(Activity_Count, detectionResults);
	
	if (!SUCCEEDED(hr)) return;

	msg.AppendString(ACTIVITY_MESSAGE_TAG);

	for (int i = 0; i < Activity_Count; i++)
	{
		msg.AppendInteger((int) detectionResults[i]);
	}
}
void BodyMulticaster::SendExpression(oscMessage &msg, IBody* body)
{
	DetectionResult detectionResults[Expression_Count];
	HRESULT hr = body->GetExpressionDetectionResults(Expression_Count, detectionResults);
	
	if (!SUCCEEDED(hr)) return;
	
	msg.AppendString(EXPRESSION_MESSAGE_TAG);

	for (int i = 0; i < Expression_Count; i++)
	{
		msg.AppendInteger((int) detectionResults[i]);
	}
}
void BodyMulticaster::SendJointOrientations(oscMessage &msg, IBody* body)
{
	JointOrientation jointOrientations[JointType_Count];
	HRESULT hr = body->GetJointOrientations(JointType_Count, jointOrientations);
	
	if (!SUCCEEDED(hr)) return;

	msg.AppendString(JOINT_ORIENTATION_MESSAGE_TAG);

	for (int i = 0; i < JointType_Count; i++)
	{
		msg.AppendFloat(jointOrientations[i].Orientation.x);
		msg.AppendFloat(jointOrientations[i].Orientation.y);
		msg.AppendFloat(jointOrientations[i].Orientation.z);
		msg.AppendFloat(jointOrientations[i].Orientation.w);
	}
}

void BodyMulticaster::SendAppearance(oscMessage &msg, IBody* body)
{
	DetectionResult detectionResults[Appearance_Count];
	HRESULT hr = body->GetExpressionDetectionResults(Appearance_Count, detectionResults);

	if (!SUCCEEDED(hr)) return;
	
	msg.AppendString(APPEARANCE_MESSAGE_TAG);
	for (int i = 0; i < Appearance_Count; i++)
	{
		msg.AppendInteger((int) detectionResults[i]);
	}
}

void BodyMulticaster::SendLean(oscMessage &msg, IBody* body)
{
	HRESULT hr;

	PointF lean;
	hr = body->get_Lean(&lean); if (!SUCCEEDED(hr)) return;

	TrackingState trackingState;
	hr = body->get_LeanTrackingState(&trackingState); if (!SUCCEEDED(hr)) return;
	
	msg.AppendString(LEAN_MESSAGE_TAG);
	msg.AppendFloat(lean.X);
	msg.AppendFloat(lean.Y);
	msg.AppendInteger((int) trackingState);
}

void BodyMulticaster::SendMiscellaneousInfo(oscMessage &msg, IBody* body)
{
	HRESULT hr;

	DetectionResult isEngaged;
	hr = body->get_Engaged(&isEngaged); if (!SUCCEEDED(hr)) return;

	DWORD clippedEdges;
	hr = body->get_ClippedEdges(&clippedEdges); if (!SUCCEEDED(hr)) return;

	BOOLEAN isRestricted;
	hr = body->get_IsRestricted(&isRestricted); if (!SUCCEEDED(hr)) return;

	msg.AppendString(MISCELLANEA_MESSAGE_TAG);
	msg.AppendInteger(isEngaged);
	msg.AppendString(std::to_string(clippedEdges));
	msg.AppendBoolean((bool) isRestricted);
}

void BodyMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{	
	HRESULT hr;			
			
	IBodyFrameArrivedEventArgs* frameArrivedEvent	= NULL;
	IBodyFrameReference*		frameReference		= NULL;
	IBodyFrame*					bodyFrame			= NULL;

	// Acquire Frame
	hr = bodyFrameReader->GetFrameArrivedEventData(waitableHandle, &frameArrivedEvent);
			
	if (SUCCEEDED(hr))
	{
		hr = frameArrivedEvent->get_FrameReference(&frameReference);
	}
	if (SUCCEEDED(hr))
	{
		hr = frameReference->AcquireFrame(&bodyFrame);
	}
	if (SUCCEEDED(hr))
	{
		if (bodyFrame) ProcessNewBodyFrame(bodyFrame);
	}
	// Release frame to receive new frames
	SafeRelease(bodyFrame);
	SafeRelease(frameReference);
	SafeRelease(frameArrivedEvent);
}

