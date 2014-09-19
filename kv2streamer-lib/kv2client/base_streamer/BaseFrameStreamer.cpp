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

#include "BaseFrameStreamer.h"

namespace kv2s {

BaseFrameStreamer::BaseFrameStreamer(KV2Client* client)
: is_streaming_(true)
{
	signalSendUpdateToServer.connect(boost::bind(&KV2Client::SendUpdateMessageToServer, client));
}

BaseFrameStreamer::~BaseFrameStreamer()
{
}

void BaseFrameStreamer::PauseStreaming()
{
	if (!is_streaming_) return;
	signalSendUpdateToServer();
	is_streaming_ = false;
}

void BaseFrameStreamer::ResumeStreaming()
{
	if (is_streaming_) return;
	signalSendUpdateToServer();
	is_streaming_ = true;
}

bool BaseFrameStreamer::get_is_streaming() const
{
	return is_streaming_;
}

} // namespace kv2streamer