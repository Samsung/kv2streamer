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

