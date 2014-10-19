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

#include "AudioMulticaster.h"
#include <iostream>
#include <string>
#include "Utilities.h"

#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

AudioMulticaster::AudioMulticaster(std::string multicast_ip, int _port)
	: gstSender(multicast_ip, _port)
	, AbstractMulticaster(multicast_ip, _port)
	, audioSubFrameBuffer(NULL)
{} 


AudioMulticaster::~AudioMulticaster(void)
{
	if (audioSubFrameBuffer) delete [] audioSubFrameBuffer;
}      

HRESULT AudioMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{

	AudioMulticastingPipelineCreationParameters input;
	input.multicastIP	= multicastIP;
	input.port			= port;
	input.format		= AUDIO_FORMAT;
	input.numChannels	= AUDIO_NUM_CHANNELS;
	input.samplingRate	= AUDIO_SAMPLE_RATE;

	 gstSender.Initialize(CreateAppSrcAudioMulticastingPipeline(input));
	//gstSender.Initialize(CreateAppSrcAudioSinkPipeline());
	gstSender.SetPipelineState(GST_STATE_PLAYING);

	HRESULT hr;
	
	IAudioSource* audioSource = NULL;

	if (kinectSensor)
	{
		hr = kinectSensor->get_AudioSource(&audioSource);

		if (SUCCEEDED(hr))
		{
			hr = audioSource->get_SubFrameLengthInBytes(&subframeLength);

			std::cout << "[AudioMulticaster] Retrieved SubFrame Length = " << subframeLength << std::endl; 
			audioSubFrameBuffer = new BYTE[subframeLength];

			hr = audioSource->OpenReader(&audioBeamFrameReader);
		}
	}
	SafeRelease(audioSource);
	audioBeamFrameReader->SubscribeFrameArrived(&waitableHandle);
	 
	return hr;
}

void AudioMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{
	HRESULT hr;			
			
	IAudioBeamFrameArrivedEventArgs*	frameArrivedEvent	= NULL;
	IAudioBeamFrameReference*			frameReference		= NULL;
	IAudioBeamFrameList*				audioBeamFrameList	= NULL;
	IAudioBeamFrame*					frame				= NULL;

	// Acquire Frame
	hr = audioBeamFrameReader->GetFrameArrivedEventData(waitableHandle, &frameArrivedEvent);
			
	if (SUCCEEDED(hr))
	{ 
		//std::cout << "Audio\n";  

		if (gstSender.GetNeedsData())
		{
			hr = frameArrivedEvent->get_FrameReference(&frameReference);
											
			if (SUCCEEDED(hr))
			{
				frameReference->AcquireBeamFrames(&audioBeamFrameList);
			}
			if (SUCCEEDED(hr) && audioBeamFrameList)
			{
				UINT count;
				audioBeamFrameList->get_BeamCount(&count);
				if (count != 1) std::cout << "count:" << count << "=======================" << std::endl;
				if (count != 1) std::cout << "count:" << count << "=======================" << std::endl;
				if (count != 1) std::cout << "count:" << count << "=======================" << std::endl;
				if (count != 1) std::cout << "count:" << count << "=======================" << std::endl;

				if (count > 0)
				{
					hr = audioBeamFrameList->OpenAudioBeamFrame(0, &frame);

					if (SUCCEEDED(hr) && frame)
					{
						UINT subframeCount;
						frame->get_SubFrameCount(&subframeCount);
								
						for (int i = 0; i < subframeCount; i++)
						{
							IAudioBeamSubFrame* subFrame = NULL;
							hr = frame->GetSubFrame(i, &subFrame);

							if (SUCCEEDED(hr) && subFrame)
							{
								ProcessNewAudioSubFrame(subFrame);
								//std::cout << "process audio\n";
							}
							SafeRelease(subFrame);
						}
					}
				}
			}
			////std::cout << timeGetTime() - time << std::endl;
			//time = timeGetTime();

			// Release frame to receive new frames
		} else {
			//std::cout << "Audio no need for data\n";
		}
		//std::cout << "endAudio\n";
	}
	SafeRelease(audioBeamFrameList);
	SafeRelease(frame);
	SafeRelease(frameReference);
	SafeRelease(frameArrivedEvent);
 }

void AudioMulticaster::ProcessNewAudioSubFrame(IAudioBeamSubFrame* subFrame)
{
	HRESULT hr;

	UINT length;
	hr = subFrame->get_FrameLengthInBytes(&length);
	
	if (length != subframeLength) std::cout << "unequal: " << length << std::endl;

	hr = subFrame->CopyFrameDataToArray(subframeLength, audioSubFrameBuffer);

	if (SUCCEEDED(hr))
	{
		gstSender.SendFrame((unsigned char*) audioSubFrameBuffer, subframeLength);
	}
}


