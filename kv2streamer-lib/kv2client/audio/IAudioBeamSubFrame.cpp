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

#include "IAudioBeamSubFrame.h"
#include <kv2client/SharedConfig.h>

namespace kv2s {

IAudioBeamSubFrame::IAudioBeamSubFrame(AudioFrameStreamerPtr _streamer)
: streamer(_streamer)
{}

IAudioBeamSubFrame::~IAudioBeamSubFrame()
{
	streamer->FrameDeleted();
}

unsigned char** IAudioBeamSubFrame::GetAudioBeamSubFrameBufferReferencePtr()
{
	return &frameBufferReference;
}

bool IAudioBeamSubFrame::AccessRawUnderlyingBuffer(UINT* capacity, unsigned char** buffer)
{
	*capacity = AUDIO_SUBFRAME_BUFFER_SIZE;
	*buffer	  = frameBufferReference;
	
	return true;
}

} // namespace kv2streamer
