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

#include "ColoredDepthFrameStreamer.h"
#include "IColoredDepthFrame.h"

namespace kv2s {

ColoredDepthFrameStreamer::ColoredDepthFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
: BaseFrameStreamer(client)
, multicastReceiver(ip_address, port)
{
	VideoReceivingMulticastPipelineCreationParameters input;
	input.multicastIP 		= ip_address;
	input.port				= port;
	input.networkInterface	= network_interface;
	input.pixelFormat		= COLOR_PIXEL_FORMAT_CLIENT;
	input.outputWidth		= DEPTH_MULTICAST_WIDTH;
	input.outputHeight		= DEPTH_MULTICAST_HEIGHT;
	input.latency			= latency_milis;
	
	multicastReceiver.Initialize(CreateReceive_RGB8_MulticastToAppSink(input));

	multicastReceiver.SetPipelineState(GST_STATE_PLAYING);	
	std::cout << "[ColoredDepthFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

ColoredDepthFrameStreamerPtr ColoredDepthFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

ColoredDepthFrameStreamer::~ColoredDepthFrameStreamer()
{
}

bool ColoredDepthFrameStreamer::AcquireLatestFrame(IColoredDepthFramePtr* coloredDepthFrame)
{	
	if (multicastReceiver.GetIsNewFrameAvailable())
	{
		(*coloredDepthFrame) = IColoredDepthFramePtr( new IColoredDepthFrame( GetSharedPtr() ) );
		
		if (!multicastReceiver.GetLatestFrameBuffer((void**)(*coloredDepthFrame)->GetColoredDepthFrameBufferReferencePtr()))
		{
			return false; 
		}
		return true;
	} else return false;
}

void ColoredDepthFrameStreamer::FrameDeleted() 
{
	multicastReceiver.ReleaseFrameBuffer();
}

void ColoredDepthFrameStreamer::PauseStreaming()
{
	BaseFrameStreamer::PauseStreaming();
	multicastReceiver.set_is_streaming(false);
}

void ColoredDepthFrameStreamer::ResumeStreaming()
{
	BaseFrameStreamer::ResumeStreaming();
	multicastReceiver.set_is_streaming(true);

}

bool ColoredDepthFrameStreamer::get_IsNewFrameAvailable()
{
	return multicastReceiver.GetIsNewFrameAvailable();
}

} // namespace kv2streamer
