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

#include "Kinect.h"

#include <oscpack-wrapper/oscReceiver.h>

#include "BodyMulticaster.h"
#include "ColorMulticaster.h"
#include "DepthMulticaster.h"
#include "BodyIndexMulticaster.h"
#include "AudioMulticaster.h"
#include "ColoredDepthMulticaster.h"
#include "ColorMaskedUsersMulticaster.h"

class KV2Server
{ 
public:
	KV2Server(std::string IP, std::string tableFilename);
	~KV2Server();
	void Block();

private:

	IKinectSensor*	kinectSensor;

	BodyMulticaster				bodyMulticaster;
	ColorMulticaster			colorMulticaster;
	DepthMulticaster			depthMulticaster;
	BodyIndexMulticaster		bodyIndexMulticaster;
	AudioMulticaster			audioMulticaster;
	ColoredDepthMulticaster		coloredDepthMulticaster;
	ColorMaskedUsersMulticaster	colorMaskedUsersMulticaster;

	kv2s::oscSender		coordinateMapperSender;
	kv2s::oscReceiver	receiver;
	std::string			multicastIP;
	std::string			tableFilename;

	void ReceiveClientMessage(kv2s::oscMessage * msg);
};

