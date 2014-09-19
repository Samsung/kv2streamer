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

#include <cmath>
#include "IBodyIndexFrame.h"

namespace kv2s {

IBodyIndexFrame::IBodyIndexFrame(BodyIndexFrameStreamerPtr _streamer)
: streamer(_streamer)
, decoded(false)
{}

IBodyIndexFrame::~IBodyIndexFrame()
{
	streamer->FrameDeleted();
}

unsigned char** IBodyIndexFrame::GetBodyIndexFrameBufferReferencePtr()
{
	return &frameBufferReference;
}

bool IBodyIndexFrame::AccessRawUnderlyingBuffer(UINT* capacity, signed char** buffer)
{
	*capacity = K4W2_BODY_INDEX_WIDTH * K4W2_BODY_INDEX_HEIGHT;
	
	
	/********************************************
	 * Current possible values of body index:
	 * [-1, 0, 1, 2, 3, 4, 5]
	 * 
	 * Sending the raw values results in distortions
	 * of the data, so space them out a little bit:
	 * 
	 * encoded = 40 * (raw + 1)
	 * 
	 * giving: [-1, 0, 1, 2, 3, 4, 5] -> [0, 40, 80, 120, 160, 200, 240]
	 * 
	 * Decoding is simple:
	 * 
	 * raw = decoded = round(encoded/40.f) - 1
	 * 
	 * The rounding will take care of the distortions caused
	 * by H264 encoding/decoding.
	 * 
	 ********************************************/
	if (!decoded)
	{
		unsigned char* input = frameBufferReference;
		signed char* output = decodedFrameBuffer;
	
		const unsigned char* end = input + BODY_INDEX_MULTICAST_WIDTH * BODY_INDEX_MULTICAST_HEIGHT;
		
		while (input < end)
		{
			*output = (signed char) (round((*input)/40.f - 1));
			++input;
			++output;
		}
		decoded = true;
	}
	
	*buffer	  = decodedFrameBuffer;
	
	return true;
}

} // namespace kv2streamer
