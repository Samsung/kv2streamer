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

#include "ColorMulticaster.h"
#include <iostream>
#include <string>
#include "Utilities.h"

#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

ColorMulticaster::ColorMulticaster(std::string multicast_ip, int _port)
	: pBuffer(NULL)
	, gstSender(multicast_ip, _port)
	, AbstractMulticaster(multicast_ip, _port)
{
	pBuffer = new RGBQUAD[K4W2_COLOR_WIDTH * K4W2_COLOR_HEIGHT];
}

ColorMulticaster::~ColorMulticaster(void)
{
	if (pBuffer)
    {
        delete [] pBuffer;
        pBuffer = NULL;
    }
}

HRESULT ColorMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{
	VideoMulticastingPipelineCreationParameters input;
	input.multicastIP		= multicastIP;
	input.port				= port;
	input.useHighestQuality = false;
	input.inputHeight		= K4W2_COLOR_HEIGHT;
	input.inputWidth		= K4W2_COLOR_WIDTH;
	input.multicastHeight	= COLOR_MULTICAST_HEIGHT;
	input.multicastWidth	= COLOR_MULTICAST_WIDTH;
	input.pixelFormat		= COLOR_PIXEL_FORMAT_SERVER;
	gstSender.Initialize(CreateAppSrc_RGB8_EncodedMulticastingPipeline(input));
	gstSender.SetPipelineState(GST_STATE_PLAYING);

	HRESULT hr;
	
	IColorFrameSource* colorFrameSource = NULL;
	if (kinectSensor)
	{
		hr = kinectSensor->get_ColorFrameSource(&colorFrameSource);

		if (SUCCEEDED(hr))
		{
			hr = colorFrameSource->OpenReader(&colorFrameReader);
		}
	}
	SafeRelease(colorFrameSource);
	colorFrameReader->SubscribeFrameArrived(&waitableHandle);
	 
	return hr;
}

void ColorMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{	
	HRESULT hr;			
			
	IColorFrameArrivedEventArgs*	frameArrivedEvent	= NULL;
	IColorFrameReference*			frameReference		= NULL;
	IColorFrame*					frame				= NULL;

	// Acquire Frame
	hr = colorFrameReader->GetFrameArrivedEventData(waitableHandle, &frameArrivedEvent);
			
	if (SUCCEEDED(hr))
	{ 
		if (gstSender.GetNeedsData())
		{
			hr = frameArrivedEvent->get_FrameReference(&frameReference);
					
			if (SUCCEEDED(hr))
			{
				hr = frameReference->AcquireFrame(&frame);
			}
			if (SUCCEEDED(hr))
			{
				if (frame)	ProcessNewColorFrame(frame);
			}
			// Release frame to receive new frames
			SafeRelease(frame);
			SafeRelease(frameReference);
		}
	}
	SafeRelease(frameArrivedEvent);
 }

void ColorMulticaster::ProcessNewColorFrame(IColorFrame* frame)
{
	ColorImageFormat format;
	frame->get_RawColorImageFormat(&format);

	IFrameDescription* frameDescription = NULL;
	frame->get_FrameDescription(&frameDescription);

	int width = 0;
	int height = 0;
	frameDescription->get_Height(&height);
	frameDescription->get_Width(&width);

	UINT nBufferSize = height*width*sizeof(RGBQUAD);
	
	HRESULT hr = frame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Rgba);
	if (!SUCCEEDED(hr)) return;

	if (pBuffer && (width == K4W2_COLOR_WIDTH) && (height == K4W2_COLOR_HEIGHT))
	{
		gstSender.SendFrame((unsigned char*) pBuffer, 4*width*height);
	}

	SafeRelease(frameDescription);
}


