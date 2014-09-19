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

#include <kv2client/client/KV2Client.h>
#include <kv2client/body/BodyFrameStreamer.h>
#include <kv2client/color/ColorFrameStreamer.h>
#include <kv2client/depth/DepthFrameStreamer.h>
#include <kv2client/colored_depth/ColoredDepthFrameStreamer.h>
#include <kv2client/color_masked_users/ColorMaskedUsersFrameStreamer.h>
#include <kv2client/body_index/BodyIndexFrameStreamer.h>
#include <kv2client/audio/AudioFrameStreamer.h>
#include <kv2client/coordinate_mapper/CoordinateMapperReceiver.h>
#include <kv2client/SharedConfig.h>

namespace kv2s {

KV2Client::KV2Client(std::string _IP_address, std::string interface)
: ip_address_(_IP_address)
, sender(_IP_address, CLIENT_PORT)
, network_interface_(interface)
{
	sender.SetMulticastInterface(network_interface_);
	std::cout << "KV2Client is listening to: " << ip_address_ << std::endl;
	send_update_thread_ = boost::thread(&KV2Client::UpdatingServerThread, this);
}

void KV2Client::UpdatingServerThread()
{
	while(true)
	{		
		SendUpdateMessageToServer();
		boost::this_thread::sleep_for(boost::chrono::seconds(SERVER_UPDATE_MESSAGE_PERIOD_SECONDS));
	}
}

void KV2Client::SendUpdateMessageToServer()
{	
	oscMessage msg("client");
	
	if (body_frame_streamer_ptr_ != 0 && body_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(BodyStreamRequestTag);
	}
	if (depth_frame_streamer_ptr_ != 0 && depth_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(DepthStreamRequestTag);
	}
	if (color_frame_streamer_ptr_ != 0 && color_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(ColorStreamRequestTag);
	}
	if (colored_depth_frame_streamer_ptr_ != 0 && colored_depth_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(ColoredDepthStreamRequestTag);
	}
	if (color_masked_users_frame_streamer_ptr_ != 0 && color_masked_users_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(ColorMaskedUsersStreamRequestTag);
	}
	if (body_index_frame_streamer_ptr_ != 0 && body_index_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(BodyIndexStreamRequestTag);
	}
	if (audio_frame_streamer_ptr_ != 0 && audio_frame_streamer_ptr_->get_is_streaming())
	{
		msg.AppendInteger(AudioStreamRequestTag);
	}
	if (coord_mapper_receiver_ptr_ != 0 && !coord_mapper_receiver_ptr_->get_is_table_loaded())
	{
		msg.AppendInteger(CoordinateMapperSyncRequestTag);
	}
	
	sender.Send(msg);
}

KV2Client::~KV2Client()
{
}

void KV2Client::OpenBodyFrameStreamer(BodyFrameStreamerPtr* bodyStreamer, int latency_milis)
{
	if ( body_frame_streamer_ptr_ == 0 ) 
	{
		body_frame_streamer_ptr_ = (new BodyFrameStreamer(ip_address_, BODY_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*bodyStreamer = body_frame_streamer_ptr_;
}

void KV2Client::OpenColorFrameStreamer(ColorFrameStreamerPtr* colorStreamer, int latency_milis)
{
	if ( color_frame_streamer_ptr_ == 0 )
	{
		color_frame_streamer_ptr_ = (new ColorFrameStreamer(ip_address_, COLOR_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*colorStreamer = color_frame_streamer_ptr_;
}

void KV2Client::OpenDepthFrameStreamer(DepthFrameStreamerPtr* depthStreamer, int latency_milis)
{
	if ( depth_frame_streamer_ptr_ == 0 )
	{
		depth_frame_streamer_ptr_ = (new DepthFrameStreamer(ip_address_, DEPTH_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*depthStreamer = depth_frame_streamer_ptr_;
}

void KV2Client::OpenBodyIndexFrameStreamer(BodyIndexFrameStreamerPtr* bodyIndexStreamer, int latency_milis)
{
	if ( body_index_frame_streamer_ptr_ == 0 )
	{
		body_index_frame_streamer_ptr_ = (new BodyIndexFrameStreamer(ip_address_, BODY_INDEX_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*bodyIndexStreamer = body_index_frame_streamer_ptr_;
}

void KV2Client::OpenAudioFrameStreamer(AudioFrameStreamerPtr* audioStreamer, int latency_milis)
{
	if ( audio_frame_streamer_ptr_ == 0 )
	{
		audio_frame_streamer_ptr_ = (new AudioFrameStreamer(ip_address_, AUDIO_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*audioStreamer = audio_frame_streamer_ptr_;
}

void KV2Client::OpenColoredDepthFrameStreamer(ColoredDepthFrameStreamerPtr* coloredDepthStreamer, int latency_milis)
{
	if ( colored_depth_frame_streamer_ptr_ == 0 )
	{
		colored_depth_frame_streamer_ptr_ = (new ColoredDepthFrameStreamer(ip_address_, COLORED_DEPTH_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*coloredDepthStreamer = colored_depth_frame_streamer_ptr_;
}

void KV2Client::OpenColorMaskedUsersFrameStreamer(ColorMaskedUsersFrameStreamerPtr* colorMaskedUsersStreamer, int latency_milis)
{
	if ( color_masked_users_frame_streamer_ptr_ == 0 )
	{
		color_masked_users_frame_streamer_ptr_ = (new ColorMaskedUsersFrameStreamer(ip_address_, COLOR_MASKED_USERS_PORT, latency_milis, network_interface_, this))->GetSharedPtr();
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*colorMaskedUsersStreamer = color_masked_users_frame_streamer_ptr_;
}

void KV2Client::get_CoordinateMapper(CoordinateMapperReceiverPtr* coordMapperReceiver)
{
	if ( coord_mapper_receiver_ptr_ == 0)
	{
		coord_mapper_receiver_ptr_.reset(new CoordinateMapperReceiver(COORDINATE_MAPPER_PORT));
	} else std::cout << "WARNING: OPENING STREAMER MORE THAN ONCE, PREVIOUSLY OPENED RETURNED WITH PREVIOUSLY SET VALUE OF LATENCY!\n";
	*coordMapperReceiver = coord_mapper_receiver_ptr_;
	SendUpdateMessageToServer();
}

} // namespace kv2streamer
