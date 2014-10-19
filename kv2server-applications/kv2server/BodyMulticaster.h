#pragma once

#include <oscpack-wrapper/oscWrapper.h>
#include "AbstractMulticaster.h"

class BodyMulticaster : public AbstractMulticaster
{
public:
	BodyMulticaster(std::string IP, int port);
	~BodyMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:
	kv2s::oscSender sender;

	IBodyFrameReader*	bodyFrameReader;
	
	void ProcessNewBodyFrame(IBodyFrame* newBodyFrame);
	void SendBodyFrame		(IBodyFrame* bodyFrame, kv2s::oscMessage &msg);
	void SendBody			(IBody* body, int index, kv2s::oscMessage &msg);
	
	bool	isSendingJoints;
	bool	isSendingHands;
	bool	isSendingActivity;
	bool	isSendingExpression;
	bool	isSendingJointOrientations;
	bool	isSendingAppearance;
	bool	isSendingLean;
	bool	isSendingMiscInfo;


	void SendJoints					(kv2s::oscMessage &msg, IBody* body);
	void SendHandStateAndConfidence	(kv2s::oscMessage &msg, IBody* body);
	void SendActivity				(kv2s::oscMessage &msg, IBody* body);
	void SendExpression				(kv2s::oscMessage &msg, IBody* body);
	void SendJointOrientations		(kv2s::oscMessage &msg, IBody* body);
	void SendAppearance				(kv2s::oscMessage &msg, IBody* body);
	void SendLean					(kv2s::oscMessage &msg, IBody* body);
	void SendMiscellaneousInfo		(kv2s::oscMessage &msg, IBody* body);
};

