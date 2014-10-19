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

#include "BodyIndexMulticaster.h"
#include <iostream>
#include <string>

#include "Utilities.h"
#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

BodyIndexMulticaster::BodyIndexMulticaster(std::string multicast_ip, int _port)
	: pBuffer(NULL)
	, gstSender(multicast_ip, _port)
	, AbstractMulticaster(multicast_ip, _port)
	, frameReady(false)
{
	pScaledBuffer = new BYTE[K4W2_BODY_INDEX_WIDTH * K4W2_BODY_INDEX_HEIGHT];
}


BodyIndexMulticaster::~BodyIndexMulticaster(void)
{
	delete[] pScaledBuffer;
}

HRESULT BodyIndexMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{
	VideoMulticastingPipelineCreationParameters input;
	input.multicastIP		= multicastIP;
	input.port				= port;
	input.useHighestQuality = true;
	input.inputHeight		= K4W2_BODY_INDEX_HEIGHT;
	input.inputWidth		= K4W2_BODY_INDEX_WIDTH;
	input.multicastHeight	= BODY_INDEX_MULTICAST_HEIGHT;
	input.multicastWidth	= BODY_INDEX_MULTICAST_WIDTH;
	input.pixelFormat		= BODY_INDEX_PIXEL_FORMAT;
	gstSender.Initialize(CreateAppSrc_RGB8_EncodedMulticastingPipeline(input));
	gstSender.SetPipelineState(GST_STATE_PLAYING);

	HRESULT hr;
	
	IBodyIndexFrameSource* bodyIndexFrameSource = NULL;
	if (kinectSensor)
	{
		hr = kinectSensor->get_BodyIndexFrameSource(&bodyIndexFrameSource);

		if (SUCCEEDED(hr))
		{
			hr = bodyIndexFrameSource->OpenReader(&bodyIndexFrameReader);
		}
	}
	SafeRelease(bodyIndexFrameSource);
	bodyIndexFrameReader->SubscribeFrameArrived(&waitableHandle);

	return hr;
}

void BodyIndexMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{	
	HRESULT hr;			
	IBodyIndexFrameArrivedEventArgs*	frameArrivedEvent	= NULL;
	IBodyIndexFrameReference*			frameReference		= NULL;
	IBodyIndexFrame*					frame				= NULL;

	// Acquire Frame
	hr = bodyIndexFrameReader->GetFrameArrivedEventData(waitableHandle, &frameArrivedEvent);
			
	if (SUCCEEDED(hr))
	{				
		////std::cout << "index\n";

		if (gstSender.GetNeedsData())
		{
			hr = frameArrivedEvent->get_FrameReference(&frameReference);
				
			if (SUCCEEDED(hr))
			{
				hr = frameReference->AcquireFrame(&frame);
			}
			if (SUCCEEDED(hr))	
			{
				if (frame) ProcessNewBodyIndexFrame(frame);
			}
			// Release frame to receive new frames
			SafeRelease(frame);
			SafeRelease(frameReference);
		} else {
			//std::cout << "Body Index no need for data\n";
		}
		////std::cout << "end index\n";

	}
	SafeRelease(frameArrivedEvent);
 }

void BodyIndexMulticaster::ProcessNewBodyIndexFrame(IBodyIndexFrame* frame)
{
	IFrameDescription* frameDescription = NULL;
	frame->get_FrameDescription(&frameDescription);

	int width = 0;
	int height = 0;
	frameDescription->get_Height(&height);
	frameDescription->get_Width(&width);

	UINT nBufferSize = height*width*sizeof(BYTE);

	UINT capacity;

	HRESULT hr = frame->AccessUnderlyingBuffer(&capacity, &pBuffer); if (!SUCCEEDED(hr)) return;

	if (pBuffer && (width == K4W2_BODY_INDEX_WIDTH) && (height == K4W2_BODY_INDEX_HEIGHT))
	{
		// send previous frame first

		// encode current frame, will be send in the next cycle
		BYTE* pInput	= pBuffer;
		BYTE* pOutput	= pScaledBuffer;

		const BYTE* pEnd = pInput + (width * height);

		while (pInput < pEnd)
		{
			BYTE index = *pInput;
			*pOutput = ((signed char)index + 1)*40;
            
			++pOutput;
			++pInput;
		}
		gstSender.SendFrame((unsigned char*) pScaledBuffer, nBufferSize);
	}
	SafeRelease(frameDescription);
}


