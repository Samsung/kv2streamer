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

#include "ColorMaskedUsersFrameStreamer.h"
#include "IColorMaskedUsersFrame.h"

namespace kv2s {

ColorMaskedUsersFrameStreamer::ColorMaskedUsersFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
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
	std::cout << "[ColorMaskedUsersFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

ColorMaskedUsersFrameStreamerPtr ColorMaskedUsersFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

ColorMaskedUsersFrameStreamer::~ColorMaskedUsersFrameStreamer()
{
}

bool ColorMaskedUsersFrameStreamer::AcquireLatestFrame(IColorMaskedUsersFramePtr* coloredMaskedUsersFrame)
{	
	if (multicastReceiver.GetIsNewFrameAvailable())
	{
		(*coloredMaskedUsersFrame) = IColorMaskedUsersFramePtr( new IColorMaskedUsersFrame( GetSharedPtr() ) );
		
		if (!multicastReceiver.GetLatestFrameBuffer((void**)(*coloredMaskedUsersFrame)->GetColorMaskedUsersFrameBufferReferencePtr()))
		{
			return false; 
		}
		return true;
	} else return false;
}

void ColorMaskedUsersFrameStreamer::FrameDeleted() 
{
	multicastReceiver.ReleaseFrameBuffer();
}

void ColorMaskedUsersFrameStreamer::PauseStreaming()
{
	BaseFrameStreamer::PauseStreaming();
	multicastReceiver.set_is_streaming(false);
}

void ColorMaskedUsersFrameStreamer::ResumeStreaming()
{
	BaseFrameStreamer::ResumeStreaming();
	multicastReceiver.set_is_streaming(true);

}

bool ColorMaskedUsersFrameStreamer::get_IsNewFrameAvailable()
{
	return multicastReceiver.GetIsNewFrameAvailable();
}

} // namespace kv2streamer
