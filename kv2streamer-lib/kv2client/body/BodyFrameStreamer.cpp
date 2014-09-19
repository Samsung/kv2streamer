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

#include "BodyFrameStreamer.h"
#include "IBodyFrame.h"

namespace kv2s {

BodyFrameStreamer::BodyFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
: BaseFrameStreamer(client)
, receiver(port, ip_address)
, isNewFrameAvailable(false)
, isFrameRetrieved(false)
{
	receiver.ListenWithParser(boost::bind(&BodyFrameStreamer::ReceiveBodyFrameMessage, this, _1));	
	std::cout << "[BodyFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

void BodyFrameStreamer::AppendBodyStreamingStatusMessage(oscMessage& msg)
{
	
}


BodyFrameStreamerPtr BodyFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

BodyFrameStreamer::~BodyFrameStreamer()
{
	std::cout << "bodyframestreamer released\n";
}

void BodyFrameStreamer::ReceiveBodyFrameMessage(oscMessage* msg)
{
	boost::mutex::scoped_lock(frameMutex);
	currentFrameMessage = msg;
	isNewFrameAvailable = true;
}

bool BodyFrameStreamer::AcquireLatestFrame(IBodyFramePtr* bodyFrame)
{
	boost::mutex::scoped_lock(frameMutex);
	if (!isFrameRetrieved && isNewFrameAvailable)
	{
		(*bodyFrame) = IBodyFramePtr( new IBodyFrame( GetSharedPtr() ) ); 		
		isFrameRetrieved = true;

		(*bodyFrame)->ParseBodyFrameMessage(currentFrameMessage);
		isNewFrameAvailable = false;			
		return true;
	} 
	else return false;
}

void BodyFrameStreamer::FrameDeleted() 
{
	isFrameRetrieved = false;
	receiver.ReleaseMessage();
}

bool BodyFrameStreamer::get_IsNewFrameAvailable()
{
	boost::mutex::scoped_lock(frameMutex);
	return (!isFrameRetrieved && isNewFrameAvailable);
}

  
} // namespace kv2streamer
