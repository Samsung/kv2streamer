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

#include "KV2Server.h"
#include "Utilities.h"

#include <kv2client/SharedConfig.h>

using namespace kv2s;

KV2Server::KV2Server(std::string IP, std::string filename)
	: bodyMulticaster(IP, BODY_PORT)
	, colorMulticaster(IP, COLOR_PORT)
	, depthMulticaster(IP, DEPTH_PORT)
	, bodyIndexMulticaster(IP, BODY_INDEX_PORT)
	, audioMulticaster(IP, AUDIO_PORT)
	, coloredDepthMulticaster(IP, COLORED_DEPTH_PORT)
	, colorMaskedUsersMulticaster(IP, COLOR_MASKED_USERS_PORT)
	, coordinateMapperSender(IP, COORDINATE_MAPPER_PORT)
	, receiver(CLIENT_PORT, IP, oscReceiver::SIGNAL_PARSER_WHEN_MSG_ARRIVES)
	, multicastIP(IP)
	, tableFilename(filename)
{
bool	enableBodyMulticaster				= true;
bool    enableColorMulticaster				= true;
bool	enableDepthMulticaster				= true;
bool    enableBodyIndexMulticaster			= true;
bool	enableAudioMulticaster				= true;
bool	enableColoredDepthMulticaster		= true;
bool	enableColoredMaskedUserMulticaster	= true;


	std::cout << "Starting up server with multicast IP: " << IP << "\n......\n";
	std::cout << "Clients will use " << filename << ".kv2dat as depth->camera conversion table.\n\n";
	HRESULT hr;

	hr = GetDefaultKinectSensor(&kinectSensor);

	if (FAILED(hr))
	{
		std::cout << "FAILED: GetDefaultKinectSensor()!\n"; 
		return;
	}

	hr = kinectSensor->Open();
	
	if (enableBodyMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing Body Multicaster" << std::endl;
		hr = bodyMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized Body Multicaster\n"; 
	}


	if (enableDepthMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing Depth Multicaster" << std::endl;
		hr = depthMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized Depth Multicaster\n"; 
	}	
	
	if (enableColorMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing Color Multicaster" << std::endl;
		hr = colorMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized Color Multicaster\n"; 
	}


	if (enableBodyIndexMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing BodyIndex Multicaster" << std::endl;
		hr = bodyIndexMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized BodyIndex Multicaster\n"; 
	}

	if (enableAudioMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing Audio Multicaster" << std::endl;
		hr = audioMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized Audio Multicaster\n"; 
	}

	if (enableColoredDepthMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing ColoredDepth Multicaster" << std::endl;
		hr = coloredDepthMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized ColoredDepth Multicaster\n"; 
	}

	if (enableColoredMaskedUserMulticaster && SUCCEEDED(hr))
	{
		std::cout << std::endl << "Initializing ColoredMaskedUser Multicaster" << std::endl;
		hr = colorMaskedUsersMulticaster.Initialize(kinectSensor);
		std::cout << "Initialized ColoredMaskedUser Multicaster\n"; 
	}

	std::cout << std::endl;

	receiver.ListenWithParser(boost::bind(&KV2Server::ReceiveClientMessage, this, _1));
}

void KV2Server::ReceiveClientMessage(oscMessage * msg)
{
	std::string address = msg->GetAddress();
	std::string sourceIP = msg->GetSourceIP();
	
	std::cout << sourceIP << " requested the following data: ";

	const int num_arguments = msg->GetNumberOfArguments();
		
	int i = 0;
	while ( i < num_arguments )
	{
		StreamRequestTag tag = (StreamRequestTag) msg->GetArgumentAtIndex(i++).GetIntegerValue();

		if (tag == BodyStreamRequestTag)
		{	
			std::cout << "BODY ";
			bodyMulticaster.ResetTimeout();
		}
		else if (tag == ColorStreamRequestTag)
		{
			std::cout << "COLOR ";
			colorMulticaster.ResetTimeout();
		}
		else if (tag == DepthStreamRequestTag)
		{
			std::cout << "DEPTH ";
			depthMulticaster.ResetTimeout();
		}
		else if (tag == BodyIndexStreamRequestTag)
		{
			std::cout << "BODYINDEX ";
			bodyIndexMulticaster.ResetTimeout();
		}
		else if (tag == AudioStreamRequestTag)
		{
			std::cout << "AUDIO ";
			audioMulticaster.ResetTimeout();
		}
		else if (tag == ColoredDepthStreamRequestTag)
		{
			std::cout << "COLORED_DEPTH ";
			coloredDepthMulticaster.ResetTimeout();
		}
		else if (tag == ColorMaskedUsersStreamRequestTag)
		{
			std::cout << "COLOR_MASKED_USER ";
			colorMaskedUsersMulticaster.ResetTimeout();
		}

		else if (tag == CoordinateMapperSyncRequestTag)
		{
			std::cout << "COORD_MAPPER_SYNC ";
			oscMessage msg("conversiontable");
			msg.AppendString(tableFilename);
			coordinateMapperSender.Send(msg);
		}
		else
		{
			std::cout << "WARNING: Unrecognized stream request tag!\n";
		}
	}
	std::cout << std::endl;
	delete msg;
} 


KV2Server::~KV2Server(void)
{
	if (kinectSensor)
    {
        kinectSensor->Close();
    }
	SafeRelease(kinectSensor);
}

void KV2Server::Block()
{
	int i = 0;

	while (true)
	{
		Sleep(5000);
		std::cout << "[" << (((i++)%2 == 0) ? "|" : "-") << "] Server " << multicastIP << " is running/waiting... \n";
	}
}

