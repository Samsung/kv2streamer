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

/**
 * @file 
 * @author Kevin Wong <k.wong2@partner.samsung.com, kevinwinatawong@gmail.com>
 * @date August 4th, 2014
 * 
 * @brief This file contains ColorFrameStreamer class definition.
 * 
 */

#pragma once

#include <string>
#include <kv2client/client/KV2Client.h>
#include <kv2client/utils/Structures.h>
#include <gst-wrapper/GstAppSinkPipeline.h>
#include <kv2client/base_streamer/BaseFrameStreamer.h>

namespace kv2s {

class IColorFrame;
typedef boost::shared_ptr<IColorFrame> IColorFramePtr;

/**
 * @class ColorFrameStreamer
 * @brief Manages gstreamer connection with KV2Server and mimics K4W2 IColorFrameReader API.
 * 
 * //Initialization:
 * KV2ClientPtr client;
 * std::string multicast_ip = "224.1.1.1";
 * client.reset(new KV2Client(multicast_ip));
 * ColorFrameStreamerPtr color_streamer;
 * client->OpenColorFrameStreamer(&color_streamer);
 * 
 * //Retrieving data:
 * IColorFramePtr color_frame;
 * 
 * bool result = color_streamer->AcquireLatestFrame(&color_frame);
 * 
 * if (result) { see IColorFrame docs for more details.. }
 * 
 */
 
class ColorFrameStreamer : public BaseFrameStreamer
{
public:
	
	virtual bool get_IsNewFrameAvailable();	
	
	/**
	 * WILL NOT SUCCEED IF most recent frame (shared_ptr) has not been .reset();
	 * return hr; SUCCEEEDED(hr) = true, if new frame is ready
	 */
	bool AcquireLatestFrame(IColorFramePtr* colorFrame);
	virtual void PauseStreaming();
	virtual void ResumeStreaming();

	~ColorFrameStreamer();

private:
	friend class KV2Client;
	ColorFrameStreamerPtr GetSharedPtr();
	
	friend class IColorFrame;
	void FrameDeleted();

private:
	ColorFrameStreamer(std::string IP_addres, int port, int latency_milis, std::string network_interface, KV2Client* client);
	ColorFrameStreamerPtr thisPtr;

	boost::mutex	frameMutex;
	bool			isNewFrameAvailable;
	
	GstAppSinkPipeline multicastReceiver;
	
};

} // namespace kv2streamer
