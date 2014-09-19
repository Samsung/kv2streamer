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

/**********************************
	CLIENT-SERVER COMMUNICATION
***********************************/
#define SERVER_UPDATE_MESSAGE_PERIOD_SECONDS 	1
#define SERVER_STREAM_TIMEOUT_SECONDS			2*SERVER_UPDATE_MESSAGE_PERIOD_SECONDS

typedef enum StreamRequestTag_
{
	BodyStreamRequestTag 				= 0,
	ColorStreamRequestTag 				= 1,
	DepthStreamRequestTag				= 2,
	BodyIndexStreamRequestTag			= 3,
	AudioStreamRequestTag				= 4,
	ColoredDepthStreamRequestTag		= 5,
	ColorMaskedUsersStreamRequestTag	= 6,
	CoordinateMapperSyncRequestTag		= 7,

} StreamRequestTag;

/**********************************
	SHARED KINECT CONSTANTS
***********************************/

#define K4W2_COLOR_WIDTH		1920
#define K4W2_COLOR_HEIGHT		1080
 
#define K4W2_DEPTH_WIDTH		512
#define K4W2_DEPTH_HEIGHT		424

#define K4W2_BODY_INDEX_WIDTH	K4W2_DEPTH_WIDTH
#define K4W2_BODY_INDEX_HEIGHT	K4W2_DEPTH_HEIGHT

/********************************** 
	SHARED MULTICAST CONSTANTS
***********************************/

#define CLIENT_PORT				61400
#define BODY_PORT				61402
#define COLOR_PORT				61404
#define DEPTH_PORT				61406
#define BODY_INDEX_PORT			61408
#define AUDIO_PORT				61410
#define COLORED_DEPTH_PORT		61412
#define COLOR_MASKED_USERS_PORT	61414
#define COORDINATE_MAPPER_PORT	61416

#define COLOR_MULTICAST_WIDTH	1280
#define COLOR_MULTICAST_HEIGHT	720

#define DEPTH_MULTICAST_WIDTH	K4W2_DEPTH_WIDTH
#define DEPTH_MULTICAST_HEIGHT	K4W2_DEPTH_HEIGHT

#define BODY_INDEX_MULTICAST_WIDTH	K4W2_BODY_INDEX_WIDTH
#define BODY_INDEX_MULTICAST_HEIGHT	K4W2_BODY_INDEX_HEIGHT

#define COLOR_PIXEL_FORMAT_CLIENT		"RGB"
#define COLOR_PIXEL_FORMAT_SERVER		"RGBA"
#define DEPTH_PIXEL_FORMAT				"GRAY16_BE"
#define BODY_INDEX_PIXEL_FORMAT			"GRAY8"
#define AUDIO_FORMAT					"F32LE"
#define AUDIO_SAMPLE_RATE				16000
#define AUDIO_NUM_CHANNELS				1
#define AUDIO_SUBFRAME_BUFFER_SIZE		1024

/* From: http://www.tldp.org/HOWTO/Multicast-HOWTO-6.html
6.2 IP_MULTICAST_TTL.

If not otherwise specified, multicast datagrams are sent with a default value of 1, to prevent them to be forwarded beyond the local network. To change the TTL to the value you desire (from 0 to 255), put that value into a variable (here I name it "ttl") and write somewhere in your program:

    u_char ttl;
    setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

The behavior with getsockopt() is similar to the one seen on IP_MULTICAST_LOOP. 
*/

/**********************************
	BODY MESSAGE TAGS
***********************************/
//TODO: finish documenting tags

// default: tracking id, index, istracked, engaged

#define JOINT_MESSAGE_TAG				"joint"
/* Sends 25 joint structures, ...*/

#define JOINT_ORIENTATION_MESSAGE_TAG	"orientation"

#define HAND_MESSAGE_TAG				"hand"

#define ACTIVITY_MESSAGE_TAG			"activity"

#define EXPRESSION_MESSAGE_TAG			"expression"

#define APPEARANCE_MESSAGE_TAG			"appearance"

#define	LEAN_MESSAGE_TAG				"lean"

#define MISCELLANEA_MESSAGE_TAG			"misc"
// clipped edges, lean, lean tracking state

#define END_OF_BODY_TAG					"end"