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

#include <sstream>
#include <kv2client/SharedConfig.h>

#define LINK			" ! "
#define NEW_LINKAGE		" "
#define PAD				"."

#define APPSRC_NAME		"appsrc_name"
#define APPSINK_NAME	"appsink_name"

#define RTPBIN_NAME		"rtpbin_name"

#define TEE_NAME		"tee_name"

/**********************************************
	SERVER-SIDE PIPELINES
**********************************************/

static std::string CreateAppSrcAudioSinkPipeline()
{
	std::stringstream pipelineString;

	pipelineString

	<< "appsrc name=" << APPSRC_NAME << " is-live=true block=true stream-type=0 format=3 do-timestamp=true"
	<< LINK << "audio/x-raw, format=F32LE, channels=1, rate=16000" << LINK
	<< "audioconvert"
	<< LINK
	<< "audioresample"
	<< LINK
	<< "opusenc audio=false"
	<< LINK
	<< "rtpopuspay"
	<< LINK
	<< "rtpopusdepay"
	<< LINK
	<< "opusdec"
	<< LINK
	<< "autoaudiosink"
	;

	return pipelineString.str();
}

struct AudioMulticastingPipelineCreationParameters
{
	std::string multicastIP;
	int			port;
	std::string format;
	int			numChannels;
	int			samplingRate;
};

static std::string CreateAppSrcAudioMulticastingPipeline(AudioMulticastingPipelineCreationParameters input)
{
	std::stringstream pipelineString;
	
	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME

	<< NEW_LINKAGE

	<< "appsrc name=" << APPSRC_NAME << " is-live=true block=true stream-type=0 format=3 do-timestamp=true"
	<< LINK << "audio/x-raw, format=" << input.format << ", channels=" << input.numChannels << ", rate="<< input.samplingRate << LINK
	<< "audioconvert"
	<< LINK
	<< "audioresample"
	<< LINK
	<< "opusenc audio=false"
	<< LINK
	<< "rtpopuspay"
	<< LINK
	<< RTPBIN_NAME << PAD << "send_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD << "send_rtp_src_0"
	<< LINK
	<< "udpsink port=" << input.port << " host=" << input.multicastIP << " auto-multicast=true" 
	;
	return pipelineString.str();
}

struct VideoMulticastingPipelineCreationParameters
{
	std::string multicastIP;
	int			port;
	std::string pixelFormat;
	int			inputWidth;
	int			inputHeight;
	int			multicastWidth;
	int			multicastHeight;
	bool		useHighestQuality;
};

static std::string CreateAppSrcToVideoSinkPipeline(VideoMulticastingPipelineCreationParameters input)
{
	std::stringstream pipelineString;

	pipelineString
	<< "appsrc name=" << APPSRC_NAME << " is-live=true block=true stream-type=0 format=time do-timestamp=true" 
	<< LINK << "video/x-raw, format=" << input.pixelFormat << ", width=" << input.inputWidth << ", height=" << input.inputHeight << ", framerate=30/1" << LINK
	<< "videoconvert"
	<< LINK
	<< "autovideosink"
	;
	return pipelineString.str();
}

// RGB Multicasting Pipeline
static std::string CreateAppSrc_RGB8_EncodedMulticastingPipeline(VideoMulticastingPipelineCreationParameters input)
{	
	std::stringstream pipelineString;

	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME

	<< NEW_LINKAGE

	<< "appsrc name=" << APPSRC_NAME << " is-live=true block=true stream-type=0 format=3 do-timestamp=true min-latency=0" 
	<< LINK << "video/x-raw, format=" << input.pixelFormat << ", width=" << input.inputWidth << ", height=" << input.inputHeight << ", framerate=30/1" << LINK
	<< "videoconvert"
	<< LINK
	<< "videoscale"
	<< LINK << "video/x-raw, width=" << input.multicastWidth << ", height=" << input.multicastHeight << LINK 
	<< "queue"
	<< LINK
	<< "x264enc tune=zerolatency speed-preset=ultrafast"; if (input.useHighestQuality) pipelineString << " qp-min=0 qp-max=0 qp-step=0"; pipelineString
	<< LINK << "video/x-h264, profile=baseline" <<  LINK
	<< "rtph264pay"
	<< LINK
	<< RTPBIN_NAME << PAD << "send_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD << "send_rtp_src_0"
	<< LINK
	<< "udpsink port=" << input.port << " host=" << input.multicastIP << " auto-multicast=true" 
	;
	return pipelineString.str();
}

// DEPTH MULTICASTING PIPELINE
static std::string CreateAppSrc_GRAY16BE_EncodedMulticastingPipeline(VideoMulticastingPipelineCreationParameters input)
{	
	std::stringstream pipelineString;

	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME

	<< NEW_LINKAGE

	<< "appsrc name=" << APPSRC_NAME << " is-live=true block=true stream-type=0 format=3 do-timestamp=true min-latency=0" 
	<< LINK << "video/x-raw, format=" << input.pixelFormat << ", width=" << input.inputWidth << ", height=" << input.inputHeight << ", framerate=30/1"
	<< LINK	
	<< "queue"
	<< LINK
	<< "pngenc"
	<< LINK 	
	<< "rtpgstpay"
	<< LINK
	<< RTPBIN_NAME << PAD << "send_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD << "send_rtp_src_0"
	<< LINK
	<< "udpsink port=" << input.port << " host=" << input.multicastIP << " auto-multicast=true" 
	;
	return pipelineString.str();
}

/**********************************************
	CLIENT-SIDE PIPELINES
**********************************************/

static std::string CreateReceiveMulticastToDecoderToVideoSinkPipeline(std::string multicastIP, int port)
{
	std::stringstream pipelineString;

	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME << " latency=25"

	<< NEW_LINKAGE
	
	<< "udpsrc port=" << port << " address=" << multicastIP << " auto-multicast=true multicast-iface=eth1" 
	<< LINK << "application/x-rtp, media=video, payload=96, clock-rate=90000, encoding-name=H264" << LINK
	<< RTPBIN_NAME << PAD <<"recv_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD
	<< LINK
	<< "rtph264depay"
	<< LINK
	<< "queue"
	<< LINK
	<< "avdec_h264"
	<< LINK
	<< "videoconvert"
	<< LINK
	<< "autovideosink"
	;
	return pipelineString.str();
}

struct VideoReceivingMulticastPipelineCreationParameters
{
	std::string multicastIP;
	int			port;
	std::string networkInterface;
	std::string pixelFormat;
	int			outputWidth;
	int			outputHeight;
	int			latency;
};

// RGB Receiver
static std::string CreateReceive_RGB8_MulticastToAppSink(VideoReceivingMulticastPipelineCreationParameters input)
{
	std::stringstream pipelineString;

	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME << " latency=" << input.latency

	<< NEW_LINKAGE
	
	<< "udpsrc port=" << input.port << " address=" << input.multicastIP << " auto-multicast=true multicast-iface=" << input.networkInterface 
	<< LINK << "application/x-rtp, media=video, payload=96, clock-rate=90000, encoding-name=H264" << LINK
	<< RTPBIN_NAME << PAD <<"recv_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD
	<< LINK
	<< "rtph264depay"
	<< LINK
	<< "avdec_h264"// skip-frame=1"
	<< LINK
	<< "videoconvert"
	<< LINK << "video/x-raw, format=" << input.pixelFormat << ", width=" << input.outputWidth << ", height=" << input.outputHeight << "" << LINK
	<< "appsink qos=true name=" << APPSINK_NAME
	;
	return pipelineString.str();
}

// DEPTH RECEIVER
static std::string CreateReceive_GRAY16BE_MulticastToAppSink(VideoReceivingMulticastPipelineCreationParameters input)
{
	std::stringstream pipelineString;

	// caps=aW1hZ2UvcG5nLGZyYW1lcmF0ZT0zMC8xCg" //caps retrieved through calling "echo image/png,framerate=30/1 | base64"
	// not sure if we need: media=application, payload=96, clock-rate=90000, encoding-name=X-GST
	
	pipelineString


	<< "rtpbin name=" << RTPBIN_NAME << " latency=" << input.latency

	<< NEW_LINKAGE
	
	<< "udpsrc port=" << input.port << " address=" << input.multicastIP << " auto-multicast=true multicast-iface=" << input.networkInterface 
	<< LINK << "application/x-rtp, media=application, payload=96, clock-rate=90000, encoding-name=X-GST, caps=aW1hZ2UvcG5nLGZyYW1lcmF0ZT0zMC8xCg" << LINK
	<< RTPBIN_NAME << PAD <<"recv_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD
	<< LINK
	<< "rtpgstdepay"
	<< LINK
	<< "pngdec"
	<< LINK 

	<< "appsink qos=true name=" << APPSINK_NAME
	;
	/*
	 * we removed it on September 10th: Nicolas suggested it was not necessary for this pipeline
		<< "videoconvert"
	<< LINK << "video/x-raw, format=" << input.pixelFormat << ", width=" << input.outputWidth << ", height=" << input.outputHeight << "" << LINK
	*/
	return pipelineString.str();
}

struct AudioReceivingMulticastPipelineCreationParameters
{
	std::string multicastIP;
	int			port;
	std::string networkInterface;
	int			latency;

	std::string format;
	int			numChannels;
	int			samplingRate;
};

static std::string CreateReceiveOpusAudioMulticastToAppSink(AudioReceivingMulticastPipelineCreationParameters input)
{
	std::stringstream pipelineString;

	pipelineString

	<< "rtpbin name=" << RTPBIN_NAME << " latency=" << input.latency

	<< NEW_LINKAGE
	
	<< "udpsrc port=" << input.port << " address=" << input.multicastIP << " auto-multicast=true multicast-iface=" << input.networkInterface 
	<< LINK << "application/x-rtp, media=audio, payload=96, clock-rate=48000, encoding-name=X-GST-OPUS-DRAFT-SPITTKA-00" << LINK
	<< RTPBIN_NAME << PAD <<"recv_rtp_sink_0"

	<< NEW_LINKAGE

	<< RTPBIN_NAME << PAD
	<< LINK
	<< "rtpopusdepay"
	<< LINK
	<< "opusdec"
	<< LINK
	<< "audioconvert"
	<< LINK << "audio/x-raw, format=" << input.format << ", channels=" << input.numChannels << ", rate="<< input.samplingRate << LINK
	<< "appsink qos=true name=" << APPSINK_NAME
	;

	return pipelineString.str();
}


static std::string CreateReceiveH264MulticastToAppSinkWithoutRTPBIN(std::string multicastIP, int port, int width, int height, std::string pixel_format, int latency)
{
	std::stringstream pipelineString;

	pipelineString
	
	<< "udpsrc port=" << port << " address=" << multicastIP << " auto-multicast=true multicast-iface=eth1" 
	<< LINK << "application/x-rtp, media=video, payload=96, clock-rate=90000, encoding-name=H264" << LINK
	<< "rtpjitterbuffer drop-on-latency=true latency=" << latency// << ", mode=1"
	<< LINK
	<< "rtph264depay"
	<< LINK
	<< "queue"
	<< LINK
	<< "avdec_h264"// skip-frame=1"
	<< LINK
	<< "videoconvert"
	<< LINK
	<< "queue"
	<< LINK << "video/x-raw, format=" << pixel_format << ", width=" << width << ", height=" << height << "" << LINK
	<< "appsink qos=true name=" << APPSINK_NAME
	;
	return pipelineString.str();
}

