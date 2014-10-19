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

#include "KV2ClientExample.h"
#include <gst-wrapper/GstreamerPipelines.h>

using namespace kv2s;

void KV2ClientExample::Setup()
{		
	// KV2Client Setup:
	client.reset(new KV2Client("224.1.1.1", "eth1"));
	client->OpenColorFrameStreamer(&colorStreamer);
	client->OpenBodyFrameStreamer(&bodyStreamer);
	client->OpenDepthFrameStreamer(&depthStreamer);
	client->OpenBodyIndexFrameStreamer(&bodyIndexStreamer);
	client->OpenAudioFrameStreamer(&audioStreamer);
	client->OpenColoredDepthFrameStreamer(&coloredDepthStreamer);
	client->OpenColorMaskedUsersFrameStreamer(&colorMaskedUserStreamer);
	client->get_CoordinateMapper(&coordMapper);
}

void KV2ClientExample::Update()
{
	AcquireAndProcessAudioFrame();
	AcquireAndProcessBodyFrame();
	AcquireAndProcessBodyIndexFrame();
	AcquireAndProcessColorFrame();
	AcquireAndProcessDepthFrame();
	AcquireAndProcessColoredDepthFrame();
	AcquireAndProcessColorMaskedUsersFrame();
}

void KV2ClientExample::AcquireAndProcessColorFrame()
{
	IColorFramePtr colorFrame;
	if (colorStreamer->AcquireLatestFrame(&colorFrame))
	{
		UINT bufferSize; 
		unsigned char*  buffer;
		colorFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
		
		colorImg.Update(COLOR_MULTICAST_WIDTH, COLOR_MULTICAST_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	}
}

void KV2ClientExample::AcquireAndProcessColoredDepthFrame()
{
	IColoredDepthFramePtr coloredDepthFrame;
	if (coloredDepthStreamer->AcquireLatestFrame(&coloredDepthFrame))
	{
		UINT bufferSize; 
		unsigned char*  buffer;
		coloredDepthFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
		
		coloredDepthImg.Update(DEPTH_MULTICAST_WIDTH, DEPTH_MULTICAST_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	}
}

void KV2ClientExample::AcquireAndProcessColorMaskedUsersFrame()
{
	IColorMaskedUsersFramePtr colorMaskedUsersFrame;
	if (colorMaskedUserStreamer->AcquireLatestFrame(&colorMaskedUsersFrame))
	{
		UINT bufferSize; 
		unsigned char*  buffer;
		colorMaskedUsersFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
		
		colorMaskedUsersImg.Update(DEPTH_MULTICAST_WIDTH, DEPTH_MULTICAST_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	}
}

void KV2ClientExample::AcquireAndProcessBodyFrame()
{
	IBodyFramePtr bodyFrame;
	if (bodyStreamer->AcquireLatestFrame(&bodyFrame))
	{
		bodyFrame->GetAndRefreshBodyData(BODY_COUNT, bodies);
		//printBodies(bodies);
	}
	// frame released by shared_ptr
}

void KV2ClientExample::AcquireAndProcessDepthFrame()
{
	IDepthFramePtr depthFrame;
	if (depthStreamer->AcquireLatestFrame(&depthFrame))
	{
		UINT bufferSize;
		unsigned short*  buffer;
		depthFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
		
		unsigned short * output = depthFrameRenderBuffer;
		
		const unsigned short* bufferEnd = buffer + DEPTH_MULTICAST_WIDTH*DEPTH_MULTICAST_HEIGHT;
		
		while(buffer < bufferEnd)
		{
			//*output = ((*buffer << 8) & 0xf0) | ((*buffer >> 8) & 0x0f);
			*output = *buffer << 4;
			
			output++;
			buffer++;
		}

		depthImg.Update(DEPTH_MULTICAST_WIDTH, DEPTH_MULTICAST_HEIGHT, GL_LUMINANCE, GL_UNSIGNED_SHORT, depthFrameRenderBuffer);
		depthFrame.reset();
	}
	// frame released by shared_ptr
}
/*
{
	IDepthFramePtr depthFrame;
	if (depthStreamer->AcquireLatestFrame(&depthFrame))
	{
		UINT bufferSize;
		unsigned short*  buffer;
		depthFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
	
		unsigned char* output = depthFrameRenderBuffer;
		
		const unsigned short* bufferEnd = buffer + DEPTH_MULTICAST_WIDTH*DEPTH_MULTICAST_HEIGHT;
		
		while(buffer < bufferEnd)
		{
			unsigned short depth = *buffer;
			unsigned char intensity = static_cast<unsigned char>(depth<<4);
			*output = intensity; ++output;
			++buffer;
		}
		depthImg.Update(DEPTH_MULTICAST_WIDTH, DEPTH_MULTICAST_HEIGHT, GL_LUMINANCE, GL_UNSIGNED_BYTE, depthFrameRenderBuffer);
		depthFrame.reset();
	}
	// frame released by shared_ptr
}*/

void KV2ClientExample::AcquireAndProcessBodyIndexFrame()
{
	IBodyIndexFramePtr bodyIndexFrame;
	if (bodyIndexStreamer->AcquireLatestFrame(&bodyIndexFrame))
	{
		UINT bufferSize;
		signed char*  buffer;
		bodyIndexFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
	
		unsigned char* output = bodyIndexFrameRenderBuffer;
		
		const signed char* bufferEnd = buffer + DEPTH_MULTICAST_WIDTH*DEPTH_MULTICAST_HEIGHT;
		
		while(buffer < bufferEnd)
		{
			signed char index = *buffer;

			*output = color_mapping[3*(index+1)+0]; ++output;
			*output = color_mapping[3*(index+1)+1]; ++output;
			*output = color_mapping[3*(index+1)+2]; ++output;
			++buffer;
		}
		bodyIndexImg.Update(DEPTH_MULTICAST_WIDTH, DEPTH_MULTICAST_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, bodyIndexFrameRenderBuffer);
		bodyIndexFrame.reset();
	}
	// frame released by shared_ptr
}

void KV2ClientExample::AcquireAndProcessAudioFrame()
{
	IAudioBeamSubFramePtr audioFrame;
	if (audioStreamer->AcquireLatestAudioBeamSubFrame(&audioFrame))
	{
		UINT bufferSize;
		unsigned char*  buffer;

		audioFrame->AccessRawUnderlyingBuffer(&bufferSize, &buffer);
		//audioPlayer.SendFrame(buffer, bufferSize);
		audioFrame.reset();
	}
	// frame released by shared_ptr
}

void KV2ClientExample::Draw()
{			
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 540, 960, 540);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-.9f, .9f, -.9f, .9f, 0.1f, 6.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(1.f, 1.f, -1.f);

	DrawBodies();
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-960, 652*2,-height/2, height/2,0.1,1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	colorImg.Draw(-960, -540, 960, 540,-10);
	depthImg.Draw(0, 0, 540*DEPTH_MULTICAST_WIDTH/float(DEPTH_MULTICAST_HEIGHT), 540, -10);
	bodyIndexImg.Draw(0, -540, 540*DEPTH_MULTICAST_WIDTH/float(DEPTH_MULTICAST_HEIGHT), 540, -10);
	colorMaskedUsersImg.Draw(652, 0, 540*DEPTH_MULTICAST_WIDTH/float(DEPTH_MULTICAST_HEIGHT), 540, -10);
	coloredDepthImg.Draw(652, -540, 540*DEPTH_MULTICAST_WIDTH/float(DEPTH_MULTICAST_HEIGHT), 540, -10);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

KV2ClientExample::KV2ClientExample(int WIDTH, int HEIGHT, const char* AppName) : GLFWBaseApp(WIDTH, HEIGHT, AppName), audioPlayer("0.0.0.0", 0)
{
	Bone bone;
	bone.EndJoint1 = JointType_Head; 			bone.EndJoint2 = JointType_Neck; 				BoneList.push_back(bone);
	bone.EndJoint1 = JointType_Neck; 			bone.EndJoint2 = JointType_SpineShoulder; 		BoneList.push_back(bone);
	bone.EndJoint1 = JointType_SpineShoulder; 	bone.EndJoint2 = JointType_SpineMid;			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_SpineMid;		bone.EndJoint2 = JointType_SpineBase;	 		BoneList.push_back(bone);
	
	bone.EndJoint1 = JointType_SpineBase; 		bone.EndJoint2 = JointType_HipLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HipLeft; 		bone.EndJoint2 = JointType_KneeLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_KneeLeft; 		bone.EndJoint2 = JointType_AnkleLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_AnkleLeft; 		bone.EndJoint2 = JointType_FootLeft; 			BoneList.push_back(bone);
	
	bone.EndJoint1 = JointType_SpineBase; 		bone.EndJoint2 = JointType_HipRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HipRight; 		bone.EndJoint2 = JointType_KneeRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_KneeRight; 		bone.EndJoint2 = JointType_AnkleRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_AnkleRight; 		bone.EndJoint2 = JointType_FootRight; 			BoneList.push_back(bone);
	
	bone.EndJoint1 = JointType_SpineShoulder; 	bone.EndJoint2 = JointType_ShoulderLeft; 		BoneList.push_back(bone);
	bone.EndJoint1 = JointType_ShoulderLeft; 	bone.EndJoint2 = JointType_ElbowLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_ElbowLeft; 		bone.EndJoint2 = JointType_WristLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_WristLeft; 		bone.EndJoint2 = JointType_HandLeft; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HandLeft; 		bone.EndJoint2 = JointType_HandTipLeft; 		BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HandLeft; 		bone.EndJoint2 = JointType_ThumbLeft; 			BoneList.push_back(bone);

	bone.EndJoint1 = JointType_SpineShoulder; 	bone.EndJoint2 = JointType_ShoulderRight; 		BoneList.push_back(bone);
	bone.EndJoint1 = JointType_ShoulderRight; 	bone.EndJoint2 = JointType_ElbowRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_ElbowRight; 		bone.EndJoint2 = JointType_WristRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_WristRight; 		bone.EndJoint2 = JointType_HandRight; 			BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HandRight; 		bone.EndJoint2 = JointType_HandTipRight; 		BoneList.push_back(bone);
	bone.EndJoint1 = JointType_HandRight; 		bone.EndJoint2 = JointType_ThumbRight; 			BoneList.push_back(bone);

	depthFrameRenderBuffer = new unsigned short[DEPTH_MULTICAST_WIDTH*DEPTH_MULTICAST_HEIGHT];
	bodyIndexFrameRenderBuffer = new unsigned char[DEPTH_MULTICAST_WIDTH*DEPTH_MULTICAST_HEIGHT*3];
	
	int i = -1;
	color_mapping[(i+1)*3+0] = (unsigned char) 40;
	color_mapping[(i+1)*3+1] = (unsigned char) 40;
	color_mapping[(i+1)*3+2] = (unsigned char) 40;
	i = 0;
	color_mapping[(i+1)*3+0] = (unsigned char) 255;
	color_mapping[(i+1)*3+1] = (unsigned char) 0;
	color_mapping[(i+1)*3+2] = (unsigned char) 0;
	i = 1;
	color_mapping[(i+1)*3+0] = (unsigned char) 255;
	color_mapping[(i+1)*3+1] = (unsigned char) 169;
	color_mapping[(i+1)*3+2] = (unsigned char) 0;
	i = 2;
	color_mapping[(i+1)*3+0] = (unsigned char) 255;
	color_mapping[(i+1)*3+1] = (unsigned char) 255;
	color_mapping[(i+1)*3+2] = (unsigned char) 0;
	i = 3;
	color_mapping[(i+1)*3+0] = (unsigned char) 0;
	color_mapping[(i+1)*3+1] = (unsigned char) 255;
	color_mapping[(i+1)*3+2] = (unsigned char) 0;
	i = 4;
	color_mapping[(i+1)*3+0] = (unsigned char) 0;
	color_mapping[(i+1)*3+1] = (unsigned char) 0;
	color_mapping[(i+1)*3+2] = (unsigned char) 255;
	i = 5;
	color_mapping[(i+1)*3+0] = (unsigned char) 255;
	color_mapping[(i+1)*3+1] = (unsigned char) 0;
	color_mapping[(i+1)*3+2] = (unsigned char) 255;
	
	time = glfwGetTime();	
	
	// setup audio player
	audioPlayer.Initialize(CreateAppSrcAudioSinkPipeline());
	audioPlayer.SetPipelineState(GST_STATE_PLAYING);
}

KV2ClientExample::~KV2ClientExample()
{
}

void KV2ClientExample::printBodies(IBodyPtr* bodies)
{
	int trackedBodies = 0;
	for (int i = 0; i < BODY_COUNT; i++)
	{
		BOOLEAN isTracked = false;
		bodies[i]->get_IsTracked((BOOLEAN*)&isTracked);
		
		if (isTracked)
		{
			trackedBodies++;
			std::stringstream str;
			str << "USER " << bodies[i]->getIndex() << "\n";
			
			// print hand:
			HandState left, right;
			bodies[i]->get_HandRightState(&right);
			bodies[i]->get_HandLeftState(&left);
			str << "Hand:\nLeft : ";
			if (left == HandState_Unknown) {str << "unknown";}
			else if (left == HandState_NotTracked) {str << "not tracked";}
			else if (left == HandState_Closed) {str << "closed";}
			else if (left == HandState_Open) {str << "open";}
			else if (left == HandState_Lasso) {str << "lasso";}
			str << "\nRight: ";
			if (right == HandState_Unknown) {str << "unknown";}
			else if (right == HandState_NotTracked) {str << "not tracked";}
			else if (right == HandState_Closed) {str << "closed";}
			else if (right == HandState_Open) {str << "open";}
			else if (right == HandState_Lasso) {str << "lasso";}
			str << "\n\n";
			
			std::cout << str.str();

		} 
	}
	
	std::cout << "Tracking " << trackedBodies << " bodies.\n\n";	
}

void KV2ClientExample::DrawBodies()
{
	glBegin(GL_LINES);
	glLineWidth(10.f);

	for (int i = 0; i < BODY_COUNT; i++)
	{
		if (bodies[i])
		{
			BOOLEAN isTracked;
			bodies[i]->get_IsTracked(&isTracked);
			if (isTracked)
			{
				bodies[i]->GetJoints(JointType_Count, joints);
				
				int index = bodies[i]->getIndex();
				
				float r = color_mapping[3*(index+1)+0]/255.f;
				float g = color_mapping[3*(index+1)+1]/255.f;
				float b = color_mapping[3*(index+1)+2]/255.f;
				
				glColor3f(r,g,b);
				

				for (unsigned b = 0; b < BoneList.size(); b++)
				{
					Bone bone = BoneList[b];
					DrawBone(bone.EndJoint1, bone.EndJoint2);
				}
			}
		}
	}
	glEnd();
	glColor3f(1.f,1.f,1.f);

}

void KV2ClientExample::DrawBone(JointType joint1, JointType joint2)
{
	Joint Joint1 = joints[joint1];
	Joint Joint2 = joints[joint2];	
	glVertex3f(Joint1.Position.X, Joint1.Position.Y, Joint1.Position.Z);	
	glVertex3f(Joint2.Position.X, Joint2.Position.Y, Joint2.Position.Z);
}


void KV2ClientExample::Destroy(){
	delete[] depthFrameRenderBuffer;
}

