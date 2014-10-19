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

#pragma once

#include "GLFWBaseApp.h"
#include <kv2client/KV2ClientCommon.h>

#include "glTexture2DWrapper.hpp"

#include <gst-wrapper/GstAppSrcPipeline.h>

class KV2ClientExample : public GLFWBaseApp
{
public:
	KV2ClientExample(int WIDTH, int HEIGHT, const char* AppName);
	~KV2ClientExample();
	
	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Draw();

private:

	void AcquireAndProcessColorFrame();
	void AcquireAndProcessBodyFrame();
	void AcquireAndProcessDepthFrame();
	void AcquireAndProcessColoredDepthFrame();
	void AcquireAndProcessColorMaskedUsersFrame();
	void AcquireAndProcessBodyIndexFrame();
	void AcquireAndProcessAudioFrame();

private:

	kv2s::KV2ClientPtr 						client;
	kv2s::BodyFrameStreamerPtr 				bodyStreamer;
	kv2s::ColorFrameStreamerPtr 			colorStreamer;
	kv2s::DepthFrameStreamerPtr 			depthStreamer;
	kv2s::BodyIndexFrameStreamerPtr 		bodyIndexStreamer;
	kv2s::AudioFrameStreamerPtr				audioStreamer;
	kv2s::CoordinateMapperReceiverPtr		coordMapper;
	kv2s::ColorMaskedUsersFrameStreamerPtr	colorMaskedUserStreamer;
	kv2s::ColoredDepthFrameStreamerPtr		coloredDepthStreamer;
	
	kv2s::IBodyPtr 			bodies[BODY_COUNT]; 
	Joint					joints[JointType_Count];
	
	double time;
	bool newBuffer;
	
	unsigned short* depthFrameRenderBuffer;
	unsigned char* bodyIndexFrameRenderBuffer;
	unsigned char color_mapping[3*7];
		
	void printBodies(kv2s::IBodyPtr* bodies);
	void DrawBodies();
	void DrawBone(JointType joint1, JointType joint2);
	
	std::vector<Bone> BoneList;
	
	glTexture2DWrapper colorImg;
	glTexture2DWrapper depthImg;
	glTexture2DWrapper coloredDepthImg;
	glTexture2DWrapper colorMaskedUsersImg;
	glTexture2DWrapper bodyIndexImg;
	
	kv2s::GstAppSrcPipeline audioPlayer;
};

