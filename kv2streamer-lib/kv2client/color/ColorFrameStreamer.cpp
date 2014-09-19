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

#include "ColorFrameStreamer.h"
#include "IColorFrame.h"

namespace kv2s {

ColorFrameStreamer::ColorFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
: BaseFrameStreamer(client)
, multicastReceiver(ip_address, port)
{
	VideoReceivingMulticastPipelineCreationParameters input;
	input.multicastIP 		= ip_address;
	input.port				= port;
	input.networkInterface	= network_interface;
	input.pixelFormat		= COLOR_PIXEL_FORMAT_CLIENT;
	input.outputWidth		= COLOR_MULTICAST_WIDTH;
	input.outputHeight		= COLOR_MULTICAST_HEIGHT;
	input.latency			= latency_milis;
	
	multicastReceiver.Initialize(CreateReceive_RGB8_MulticastToAppSink(input));
	multicastReceiver.SetPipelineState(GST_STATE_PLAYING);
	
	std::cout << "[ColorFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

ColorFrameStreamerPtr ColorFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

ColorFrameStreamer::~ColorFrameStreamer()
{
}

bool ColorFrameStreamer::AcquireLatestFrame(IColorFramePtr* colorFrame)
{	
	if (multicastReceiver.GetIsNewFrameAvailable())
	{
		(*colorFrame) = IColorFramePtr( new IColorFrame( GetSharedPtr() ) );
		
		if (!multicastReceiver.GetLatestFrameBuffer((void**)(*colorFrame)->GetColorFrameBufferReferencePtr()))
		{
			return false; 
		}
		return true;
	} else return false;
}

void ColorFrameStreamer::FrameDeleted() 
{
	multicastReceiver.ReleaseFrameBuffer();
}

void ColorFrameStreamer::PauseStreaming()
{
	BaseFrameStreamer::PauseStreaming();
	multicastReceiver.set_is_streaming(false);
}

void ColorFrameStreamer::ResumeStreaming()
{
	BaseFrameStreamer::ResumeStreaming();
	multicastReceiver.set_is_streaming(true);
}

bool ColorFrameStreamer::get_IsNewFrameAvailable()
{
	return multicastReceiver.GetIsNewFrameAvailable();
}

} // namespace kv2streamer
