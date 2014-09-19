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

#include "AudioFrameStreamer.h"
#include "IAudioBeamSubFrame.h"

namespace kv2s {

AudioFrameStreamer::AudioFrameStreamer(std::string ip_address, int port, int latency_milis, std::string network_interface, KV2Client* client)
: BaseFrameStreamer(client)
, multicastReceiver(ip_address, port)
{
	AudioReceivingMulticastPipelineCreationParameters input;
	input.multicastIP 		= ip_address;
	input.port				= port;
	input.networkInterface	= network_interface;

	input.format			= AUDIO_FORMAT;
	input.latency			= latency_milis;
	input.numChannels		= AUDIO_NUM_CHANNELS;
	input.samplingRate		= AUDIO_SAMPLE_RATE;
	
	multicastReceiver.Initialize(CreateReceiveOpusAudioMulticastToAppSink(input));

	multicastReceiver.SetPipelineState(GST_STATE_PLAYING);	
	std::cout << "[AudioFrameStreamer] Listening to " << ip_address << " at port " << port << std::endl;
}

AudioFrameStreamerPtr AudioFrameStreamer::GetSharedPtr()
{
	if (thisPtr == 0)
	{
		thisPtr.reset(this);
	}
	return thisPtr;
}

AudioFrameStreamer::~AudioFrameStreamer()
{
}

bool AudioFrameStreamer::AcquireLatestAudioBeamSubFrame(IAudioBeamSubFramePtr* audioSubFrame)
{	
	if (multicastReceiver.GetIsNewFrameAvailable())
	{
		(*audioSubFrame) = IAudioBeamSubFramePtr( new IAudioBeamSubFrame( GetSharedPtr() ) );
		
		if (!multicastReceiver.GetLatestFrameBuffer((void**)(*audioSubFrame)->GetAudioBeamSubFrameBufferReferencePtr()))
		{
			return false; 
		}
		return true;
	} else return false;
}

void AudioFrameStreamer::FrameDeleted() 
{
	multicastReceiver.ReleaseFrameBuffer();
}

void AudioFrameStreamer::PauseStreaming()
{
	BaseFrameStreamer::PauseStreaming();
	multicastReceiver.set_is_streaming(false);
}

void AudioFrameStreamer::ResumeStreaming()
{
	BaseFrameStreamer::ResumeStreaming();
	multicastReceiver.set_is_streaming(true);
}

bool AudioFrameStreamer::get_IsNewFrameAvailable()
{
	return multicastReceiver.GetIsNewFrameAvailable();
}

} // namespace kv2streamer
