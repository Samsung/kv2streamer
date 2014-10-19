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

#include "ColorMaskedUsersMulticaster.h"
#include <iostream>
#include <string>
#include "Utilities.h"
#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

ColorMaskedUsersMulticaster::ColorMaskedUsersMulticaster(std::string multicast_ip, int _port)
	: pInputFullRGBX(NULL)
	, pOutputColorMaskedUsers(NULL)
	, pColorSpacePoints(NULL)
	, gstSender(multicast_ip, _port)
	, AbstractMulticaster(multicast_ip, _port)
{
	pInputFullRGBX			= new RGBQUAD[K4W2_COLOR_WIDTH * K4W2_COLOR_HEIGHT];
	pOutputColorMaskedUsers = new RGBQUAD[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];
	pColorSpacePoints		= new ColorSpacePoint[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];

	default_black.rgbRed	= 0;
	default_black.rgbGreen	= 0;
	default_black.rgbBlue	= 0;
}

ColorMaskedUsersMulticaster::~ColorMaskedUsersMulticaster(void)
{
	if (pInputFullRGBX)				delete[] pInputFullRGBX;
	if (pOutputColorMaskedUsers)	delete[] pOutputColorMaskedUsers;
	if (pColorSpacePoints)			delete[] pColorSpacePoints;
}

HRESULT ColorMaskedUsersMulticaster::InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle)
{
	VideoMulticastingPipelineCreationParameters input;
	input.multicastIP		= multicastIP;
	input.port				= port;
	input.useHighestQuality = true;
	input.inputHeight		= K4W2_DEPTH_HEIGHT; 
	input.inputWidth		= K4W2_DEPTH_WIDTH;
	input.multicastHeight	= DEPTH_MULTICAST_HEIGHT;
	input.multicastWidth	= DEPTH_MULTICAST_WIDTH;
	input.pixelFormat		= COLOR_PIXEL_FORMAT_SERVER;
	
	//gstSender.Initialize(CreateAppSrcToVideoSinkPipeline(input));
	gstSender.Initialize(CreateAppSrc_RGB8_EncodedMulticastingPipeline(input));
	gstSender.SetPipelineState(GST_STATE_PLAYING);

	HRESULT hr;

    hr = kinectSensor->get_CoordinateMapper(&coordinateMapper);

    if (SUCCEEDED(hr))
    {
        hr = kinectSensor->OpenMultiSourceFrameReader(
            FrameSourceTypes::FrameSourceTypes_Depth | 
			FrameSourceTypes::FrameSourceTypes_Color |
			FrameSourceTypes::FrameSourceTypes_BodyIndex,
            &multiFrameReader);
    }
	multiFrameReader->SubscribeMultiSourceFrameArrived(&waitableHandle);
	return hr;
}


void ColorMaskedUsersMulticaster::ProcessNewFrame(WAITABLE_HANDLE &waitableHandle)
{
	if (!gstSender.GetNeedsData()) return;

	IMultiSourceFrameArrivedEventArgs*	frameArrivedEvent	= NULL;
	IMultiSourceFrameReference*			frameReference		= NULL;
	IMultiSourceFrame*					multiSourceFrame	= NULL;
    IDepthFrame*						depthFrame			= NULL;
    IColorFrame*						colorFrame			= NULL;
	IBodyIndexFrame*					bodyIndexFrame		= NULL;

	HRESULT hr = multiFrameReader->GetMultiSourceFrameArrivedEventData(waitableHandle, &frameArrivedEvent); if (!SUCCEEDED(hr)) return; 

	hr = frameArrivedEvent->get_FrameReference(&frameReference); 

	if (SUCCEEDED(hr))
	{
		hr = frameReference->AcquireFrame(&multiSourceFrame);
	}
	if (SUCCEEDED(hr))
	{// RETRIEVE DEPTH FRAME
		IDepthFrameReference* pDepthFrameReference = NULL;
		hr = multiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&depthFrame);
		}
		SafeRelease(pDepthFrameReference);
	}
	if (SUCCEEDED(hr))
	{// RETRIEVE COLOR FRAME
		IColorFrameReference* pColorFrameReference = NULL;
		hr = multiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&colorFrame);
		}
		SafeRelease(pColorFrameReference);
	}
	if (SUCCEEDED(hr))
	{// RETRIEVE BODY INDEX FRAME
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;
		hr = multiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&bodyIndexFrame);
		}
		SafeRelease(pBodyIndexFrameReference);
	}


	if (SUCCEEDED(hr))
	{
		ProcessNewColorAndDepthFrame(colorFrame, depthFrame, bodyIndexFrame);
	}
	SafeRelease(multiSourceFrame);
	SafeRelease(depthFrame);
	SafeRelease(colorFrame);
	SafeRelease(bodyIndexFrame);
	SafeRelease(frameReference);
	SafeRelease(frameArrivedEvent);
}

void ColorMaskedUsersMulticaster::ProcessNewColorAndDepthFrame(IColorFrame* colorFrame, IDepthFrame* depthFrame, IBodyIndexFrame* bodyIndexFrame)	
{
	HRESULT hr = S_OK;

	UINT16*	depthBuffer		= NULL;
	BYTE*	bodyIndexBuffer = NULL;	

	UINT depthBufferSize		= 0; 
	UINT colorBufferSize		= 0;
	UINT bodyIndexBufferSize	= 0;

	USHORT maxReliableDistance = 0; 
	USHORT minReliableDistance = 0;
	
	int depthWidth		= 0; int depthHeight		= 0;
	int colorWidth		= 0; int colorHeight		= 0;
	int bodyIndexWidth	= 0; int bodyIndexHeight	= 0;

    IFrameDescription* depthFrameDescription		= NULL;
    IFrameDescription* colorFrameDescription		= NULL;
	IFrameDescription* bodyIndexFrameDescription	= NULL;

	if (SUCCEEDED(hr))
	{
		hr = depthFrame->get_FrameDescription(&depthFrameDescription);
        
		if (SUCCEEDED(hr))
        {
            hr = depthFrameDescription->get_Width(&depthWidth);
        }
        if (SUCCEEDED(hr))
        {
            hr = depthFrameDescription->get_Height(&depthHeight);
        }
		if (SUCCEEDED(hr))
        {
            hr = depthFrame->get_DepthMaxReliableDistance(&maxReliableDistance);
        }
		if (SUCCEEDED(hr))
        {
            hr = depthFrame->get_DepthMinReliableDistance(&minReliableDistance);
        }
		if (SUCCEEDED(hr))
        {
            hr = depthFrame->AccessUnderlyingBuffer(&depthBufferSize, &depthBuffer);            
        }
	}

	if (SUCCEEDED(hr))
	{
		hr = colorFrame->get_FrameDescription(&colorFrameDescription);
        
		if (SUCCEEDED(hr))
        {
            hr = colorFrameDescription->get_Width(&colorWidth);
        }
        if (SUCCEEDED(hr))
        {
            hr = colorFrameDescription->get_Height(&colorHeight);
        }
        if (SUCCEEDED(hr))
        {
            hr = colorFrame->CopyConvertedFrameDataToArray(K4W2_COLOR_HEIGHT * K4W2_COLOR_WIDTH * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(pInputFullRGBX), ColorImageFormat_Rgba);            
        }
	}

	if (SUCCEEDED(hr))
	{
		hr = bodyIndexFrame->get_FrameDescription(&bodyIndexFrameDescription);
        
		if (SUCCEEDED(hr))
        {
			hr = bodyIndexFrameDescription->get_Width(&bodyIndexWidth);
        }
        if (SUCCEEDED(hr))
        {
            hr = bodyIndexFrameDescription->get_Height(&bodyIndexHeight);
        }
        if (SUCCEEDED(hr))
        { 
            hr = bodyIndexFrame->AccessUnderlyingBuffer(&bodyIndexBufferSize, &bodyIndexBuffer);            
        }
	}

	if (!SUCCEEDED(hr)) return;

	if (coordinateMapper && pColorSpacePoints && pOutputColorMaskedUsers && 
        depthBuffer && (depthWidth == K4W2_DEPTH_WIDTH) && (depthHeight == K4W2_DEPTH_HEIGHT) && 
		bodyIndexBuffer && (bodyIndexWidth == K4W2_DEPTH_WIDTH) && (bodyIndexHeight == K4W2_DEPTH_HEIGHT) && 
		pInputFullRGBX && (colorWidth == K4W2_COLOR_WIDTH) && (colorHeight == K4W2_COLOR_HEIGHT))
	{	
		hr = coordinateMapper->MapDepthFrameToColorSpace(
			K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT, 
			(UINT16*)depthBuffer, 
			K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT, 
			pColorSpacePoints);

		if (SUCCEEDED(hr))
		{
			ComposeOutputColorMaskedUsersBuffer(depthBuffer, maxReliableDistance, minReliableDistance, bodyIndexBuffer);
			gstSender.SendFrame((unsigned char*) pOutputColorMaskedUsers, K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT * sizeof(RGBQUAD));
		}
	}
	SafeRelease(depthFrameDescription);
	SafeRelease(colorFrameDescription);
	SafeRelease(bodyIndexFrameDescription);
}

void ColorMaskedUsersMulticaster::ComposeOutputColorMaskedUsersBuffer(UINT16* depthBuffer, USHORT maxReliableDist, USHORT minReliableDist, BYTE* bodyIndexBuffer)
{
    // loop over pixel of the output
    for (int depthIndex = 0; depthIndex < (K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT); ++depthIndex)
    {
        // default setting source to copy from the background pixel
        const RGBQUAD* pixel = &default_black; 
		                
		BYTE player = bodyIndexBuffer[depthIndex];

		if (player != 0xff) // 255 for no user
		{
			// retrieve the depth to color mapping for the current depth pixel
			ColorSpacePoint colorPoint = pColorSpacePoints[depthIndex];

			// make sure the depth pixel maps to a valid point in color space
			int colorX = (int)(floor(colorPoint.X + 0.5));
			int colorY = (int)(floor(colorPoint.Y + 0.5));
			if ((colorX >= 0) && (colorX < K4W2_COLOR_WIDTH) && (colorY >= 0) && (colorY < K4W2_COLOR_HEIGHT) &&
				(depthBuffer[depthIndex] >= minReliableDist ) && (depthBuffer[depthIndex] <= maxReliableDist))
			{
				// calculate index into color array
				int colorIndex = colorX + (colorY * K4W2_COLOR_WIDTH);
            
				// set source for copy to the color pixel
				pixel = pInputFullRGBX + colorIndex;
			}
		}
        // write output
        pOutputColorMaskedUsers[depthIndex] = *pixel;
    }
}
