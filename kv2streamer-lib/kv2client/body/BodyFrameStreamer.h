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
 * @brief This file contains BodyFrameStreamer class definition.
 * 
 */

#pragma once

#include <string>
#include <oscpack-wrapper/oscWrapper.h>
#include <kv2client/client/KV2Client.h>
#include <kv2client/base_streamer/BaseFrameStreamer.h>
#include <kv2client/utils/Structures.h>

namespace kv2s {

class IBodyFrame;
typedef boost::shared_ptr<IBodyFrame> IBodyFramePtr;

/**
 * @class BodyFrameStreamer
 * @brief Manages OSC-based connection with KV2Server and mimics K4W2 IBodyFrameReader API.
 * 
 * //Initialization:
 * KV2ClientPtr client;
 * std::string multicast_ip = "224.1.1.1";
 * client.reset(new KV2Client(multicast_ip));
 * BodyFrameStreamerPtr body_streamer;
 * client->OpenBodyFrameStreamer(&body_streamer);
 * 
 * //Retrieving data:
 * IBodyFramePtr body_frame;
 * 
 * bool result = body_streamer->AcquireLatestFrame(&body_frame);
 * 
 * if (result) { see IBodyFrame docs for more details.. }
 * 
 */
class BodyFrameStreamer : public BaseFrameStreamer
{
public:
		
	/**
	 * Retrieves the most recent frame.
	 * @remarks WILL NOT SUCCEED IF the last acquired frame has not been deleted.
	 * @param IBodyFramePtr* bodyFrame: a pointer to IBodyFramePtr, the IBodyFramePtr will be
	 * set to point to the most recent frame.
	 * @return true if successful, false otherwise.
	 */
	bool AcquireLatestFrame(IBodyFramePtr* bodyFrame);
	
	virtual bool get_IsNewFrameAvailable();

public:
	~BodyFrameStreamer(); /* Destructor, should only be called automatically by shared_ptr. */

private:
	friend class KV2Client;
	BodyFrameStreamer(std::string IP_addres, int port, int latency_milis, std::string network_interface, KV2Client* client);
	BodyFrameStreamerPtr GetSharedPtr();
	void AppendBodyStreamingStatusMessage(oscMessage& msg);

	friend class IBodyFrame;
	void FrameDeleted();	

private:	
	BodyFrameStreamerPtr thisPtr;

	oscReceiver 	receiver;
	boost::mutex	frameMutex;
	oscMessage*		currentFrameMessage; 
	bool			isNewFrameAvailable;
	
	bool 			isFrameRetrieved;
	
	void ReceiveBodyFrameMessage( oscMessage *msg );
};

} // namespace kv2streamer
