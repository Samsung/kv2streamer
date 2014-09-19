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


#include <gst-wrapper/GstreamerPipelines.h>

#include "DepthFrameStreamer.h"
#include "IDepthFrame.h"

namespace kv2s {

DepthFrameStreamer::DepthFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
: BaseFrameStreamer(client)
, multicastReceiver(ip_address, port)
{
	VideoReceivingMulticastPipelineCreationParameters input;
	input.multicastIP 		= ip_address;
	input.port				= port;
	input.networkInterface	= network_interface;
	input.pixelFormat		= DEPTH_PIXEL_FORMAT;
	input.outputWidth		= DEPTH_MULTICAST_WIDTH;
	input.outputHeight		= DEPTH_MULTICAST_HEIGHT;
	input.latency			= latency_milis;
	
	multicastReceiver.Initialize(CreateReceive_GRAY16BE_MulticastToAppSink(input));

	multicastReceiver.SetPipelineState(GST_STATE_PLAYING);	
	std::cout << "[DepthFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

DepthFrameStreamerPtr DepthFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

DepthFrameStreamer::~DepthFrameStreamer()
{
}

bool DepthFrameStreamer::AcquireLatestFrame(IDepthFramePtr* depthFrame)
{	
	if (multicastReceiver.GetIsNewFrameAvailable())
	{
		(*depthFrame) = IDepthFramePtr( new IDepthFrame( GetSharedPtr() ) );
		
		if (!multicastReceiver.GetLatestFrameBuffer((void**)(*depthFrame)->GetDepthFrameBufferReferencePtr()))
		{
			return false; 
		}
		return true;
	} else return false;
}

void DepthFrameStreamer::FrameDeleted() 
{
	multicastReceiver.ReleaseFrameBuffer();
}

void DepthFrameStreamer::PauseStreaming()
{
	BaseFrameStreamer::PauseStreaming();
	multicastReceiver.set_is_streaming(false);
}

void DepthFrameStreamer::ResumeStreaming()
{
	BaseFrameStreamer::ResumeStreaming();
	multicastReceiver.set_is_streaming(true);

}

bool DepthFrameStreamer::get_IsNewFrameAvailable()
{
	return multicastReceiver.GetIsNewFrameAvailable();
}

} // namespace kv2streamer
