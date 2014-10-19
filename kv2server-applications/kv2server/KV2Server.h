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

