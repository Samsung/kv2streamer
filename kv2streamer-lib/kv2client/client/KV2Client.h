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
 * @brief This file contains KV2Client class definition.
 * 
 */
 
#pragma once

#include <string>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <oscpack-wrapper/oscSender.h>

#define DEFAULT_NETWORK_INTERFACE			"eth0"

#define DEFAULT_AUDIO_LATENCY				25
#define DEFAULT_BODY_LATENCY				50
#define DEFAULT_BODY_INDEX_LATENCY			50
#define DEFAULT_COLOR_LATENCY				25
#define DEFAULT_DEPTH_LATENCY				50
#define DEFAULT_COLORED_DEPTH_LATENCY		50
#define DEFAULT_COLOR_MASKED_USER_LATENCY	50

namespace kv2s {

class BodyFrameStreamer;	
typedef boost::shared_ptr<BodyFrameStreamer>	BodyFrameStreamerPtr;

class ColorFrameStreamer;
typedef boost::shared_ptr<ColorFrameStreamer>	ColorFrameStreamerPtr;

class DepthFrameStreamer;
typedef boost::shared_ptr<DepthFrameStreamer>	DepthFrameStreamerPtr;

class ColorMaskedUsersFrameStreamer;
typedef boost::shared_ptr<ColorMaskedUsersFrameStreamer>	ColorMaskedUsersFrameStreamerPtr;

class ColoredDepthFrameStreamer;
typedef boost::shared_ptr<ColoredDepthFrameStreamer>	ColoredDepthFrameStreamerPtr;

class BodyIndexFrameStreamer;
typedef boost::shared_ptr<BodyIndexFrameStreamer>	BodyIndexFrameStreamerPtr;

class AudioFrameStreamer;
typedef boost::shared_ptr<AudioFrameStreamer>	AudioFrameStreamerPtr;

class CoordinateMapperReceiver;
typedef boost::shared_ptr<CoordinateMapperReceiver> CoordinateMapperReceiverPtr;


/**
 * @class KV2Client
 * @brief KV2Client is the main entrypoint for the client-side API of KV2Streamer.
 * 
 * @section Sample Usage:
 * 
 * KV2ClientPtr client;
 * 
 * client.reset(new KV2Client("224.1.1.1", "eth1"));
 * client->OpenColorFrameStreamer(&colorStreamer);
 * client->OpenBodyFrameStreamer(&bodyStreamer);
 * ...
 */
class KV2Client
{
public:
	
	KV2Client(std::string ip_address, std::string network_interface = DEFAULT_NETWORK_INTERFACE); /*e.g. ("224.1.1.1", "eth1") */
	~KV2Client();
	
	void get_CoordinateMapper(CoordinateMapperReceiverPtr*	coordMapperReceiver);
	
	void OpenBodyFrameStreamer				(BodyFrameStreamerPtr* 				bodyStreamer, 		int latency_milis = DEFAULT_BODY_LATENCY);
	void OpenColorFrameStreamer				(ColorFrameStreamerPtr* 			colorStreamer, 		int latency_milis = DEFAULT_COLOR_LATENCY);
	void OpenDepthFrameStreamer				(DepthFrameStreamerPtr* 			depthStreamer, 		int latency_milis = DEFAULT_DEPTH_LATENCY);
	void OpenBodyIndexFrameStreamer			(BodyIndexFrameStreamerPtr*			bodyIndexStreamer, 	int latency_milis = DEFAULT_BODY_INDEX_LATENCY);
	void OpenAudioFrameStreamer				(AudioFrameStreamerPtr*				audioStreamer, 		int latency_milis = DEFAULT_AUDIO_LATENCY);
	void OpenColoredDepthFrameStreamer		(ColoredDepthFrameStreamerPtr* 		depthStreamer, 		int latency_milis = DEFAULT_COLORED_DEPTH_LATENCY);
	void OpenColorMaskedUsersFrameStreamer	(ColorMaskedUsersFrameStreamerPtr* 	depthStreamer, 		int latency_milis = DEFAULT_COLOR_MASKED_USER_LATENCY);

	void SendUpdateMessageToServer();

private:

	BodyFrameStreamerPtr				body_frame_streamer_ptr_;
	ColorFrameStreamerPtr 				color_frame_streamer_ptr_;
	DepthFrameStreamerPtr 				depth_frame_streamer_ptr_;
	BodyIndexFrameStreamerPtr 			body_index_frame_streamer_ptr_;
	AudioFrameStreamerPtr				audio_frame_streamer_ptr_;
	CoordinateMapperReceiverPtr 		coord_mapper_receiver_ptr_;
	ColoredDepthFrameStreamerPtr		colored_depth_frame_streamer_ptr_;
	ColorMaskedUsersFrameStreamerPtr	color_masked_users_frame_streamer_ptr_;
	
	std::string ip_address_;
	std::string network_interface_;
	
	oscSender	sender;
	
	boost::thread 	send_update_thread_;
	void		  	UpdatingServerThread();
};
typedef boost::shared_ptr<KV2Client> KV2ClientPtr;

} // namespace kv2streamer
