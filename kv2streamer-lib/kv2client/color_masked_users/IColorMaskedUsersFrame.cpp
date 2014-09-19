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

#include <kv2client/SharedConfig.h>

#include "IColorMaskedUsersFrame.h"

namespace kv2s {

IColorMaskedUsersFrame::IColorMaskedUsersFrame(ColorMaskedUsersFrameStreamerPtr _streamer)
: streamer(_streamer)
{}

IColorMaskedUsersFrame::~IColorMaskedUsersFrame()
{
	streamer->FrameDeleted();
}

unsigned char** IColorMaskedUsersFrame::GetColorMaskedUsersFrameBufferReferencePtr()
{
	return &frameBufferReference;
}

bool IColorMaskedUsersFrame::AccessRawUnderlyingBuffer(UINT* capacity, unsigned char** buffer)
{
	*capacity = DEPTH_MULTICAST_WIDTH * DEPTH_MULTICAST_HEIGHT * 3;
	*buffer	  = frameBufferReference;
	
	return true;
}

} // namespace kv2streamer
