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

#include "DepthMulticaster.h"
#include <iostream>
#include <string>
#include "Utilities.h"
#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

DepthMulticaster::DepthMulticaster(std::string multicast_ip, int _port)
	: pBuffer(NULL)
	, gstSender(multicast_ip, _port)
	, AbstractMulticaster(multicast_ip, _port)
{
	pBuffer = new UINT16[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];
}

DepthMulticaster::~DepthMulticaster(void)
{
	if (pBuffer) delete[] pBuffer;
}

HRESULT DepthMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{
	VideoMulticastingPipelineCreationParameters input;
	input.multicastIP		= multicastIP;
	input.port				= port;
	input.useHighestQuality = true;
	input.inputHeight		= K4W2_DEPTH_HEIGHT;
	input.inputWidth		= K4W2_DEPTH_WIDTH;
	input.multicastHeight	= DEPTH_MULTICAST_HEIGHT;
	input.multicastWidth	= DEPTH_MULTICAST_WIDTH;
	input.pixelFormat		= DEPTH_PIXEL_FORMAT;

	gstSender.set_is_verbose(true);
	gstSender.Initialize(CreateAppSrc_GRAY16BE_EncodedMulticastingPipeline(input));
	gstSender.SetPipelineState(GST_STATE_PLAYING);

	HRESULT hr;
	
	IDepthFrameSource* depthFrameSource = NULL;
	if (kinectSensor)
	{
		hr = kinectSensor->get_DepthFrameSource(&depthFrameSource);

		if (SUCCEEDED(hr))
		{
			hr = depthFrameSource->OpenReader(&depthFrameReader);
		}
	}
	SafeRelease(depthFrameSource);
	depthFrameReader->SubscribeFrameArrived(&waitableHandle);

	return hr;
}


void DepthMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{	
	HRESULT hr;

	IDepthFrameArrivedEventArgs*	frameArrivedEvent	= NULL;
	IDepthFrameReference*			frameReference		= NULL;
	IDepthFrame*					frame				= NULL;

	// Acquire Frame
	hr = depthFrameReader->GetFrameArrivedEventData(waitableHandle, &frameArrivedEvent);
			
	if (SUCCEEDED(hr))
	{
		//std::cout << "depth\n";

		if (gstSender.GetNeedsData())
		{
			hr = frameArrivedEvent->get_FrameReference(&frameReference);
				
			if (SUCCEEDED(hr))
			{
				hr = frameReference->AcquireFrame(&frame);
			}
			if (SUCCEEDED(hr))	
			{
				if (frame) ProcessNewDepthFrame(frame);
			}
			else
				std::cout << "Did not acquire depth frame" << std::endl;

			// Release frame to receive new frames
			SafeRelease(frame);
			SafeRelease(frameReference);
		} else {
			//std::cout << "Depth no need for data\n";
		}
		//std::cout << "end depth\n";

	}
	SafeRelease(frameArrivedEvent);
 }

void DepthMulticaster::ProcessNewDepthFrame(IDepthFrame* frame)
{
	IFrameDescription* frameDescription = NULL;
	frame->get_FrameDescription(&frameDescription);

	int width = 0;
	int height = 0;
	frameDescription->get_Height(&height);
	frameDescription->get_Width(&width);

	UINT nBufferSize = height*width*sizeof(UINT16);

//	UINT capacity;
	HRESULT hr = frame->CopyFrameDataToArray(K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT, pBuffer);
	if (!SUCCEEDED(hr)) return;

	if (pBuffer && (width == K4W2_DEPTH_WIDTH) && (height == K4W2_DEPTH_HEIGHT))
	{
		gstSender.SendFrame((unsigned char*) pBuffer, nBufferSize);
	}

	SafeRelease(frameDescription);
}
