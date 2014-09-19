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

#include "IBodyFrame.h"
#include "IBody.h"

/*************************************
 * K4W2 API and other public interface
 *************************************/

namespace kv2s {
  
bool IBodyFrame::get_FloorClipPlane(Vector4* _floorClipPlane)
{
	(*_floorClipPlane) = floor_clip_plane_;
	return S_OK;
}

bool IBodyFrame::get_RelativeTime(TIMESPAN *_relativeTime)
{
	(*_relativeTime) = relative_time_;
	return S_OK;
}

bool IBodyFrame::get_BodyFrameSource(IBodyFrameSource* bodyFrameSource)
{
	// doesn't make sense in client API, will always return failure
	return ((HRESULT)-1);
}

bool IBodyFrame::GetAndRefreshBodyData(UINT capacity, IBodyPtr* bodies)
{
	int index = 0;
	
	ReceiveBodyFrame(current_body_frame_msg_, index);
	
	for (unsigned int b = 0; b < capacity; b++)
	{
		if (bodies[b] == 0)
		{
			bodies[b].reset(new IBody());
		}
		bodies[b]->UpdateBody(current_body_frame_msg_, index);
	} 
	return S_OK;
}

/*************************************
 *  Other helper methods
 *************************************/

IBodyFrame::IBodyFrame(BodyFrameStreamerPtr _streamer)
: streamer_(_streamer)
{}

IBodyFrame::~IBodyFrame()
{
	streamer_->FrameDeleted();
}

void IBodyFrame::ParseBodyFrameMessage(oscMessage* msg)
{
	current_body_frame_msg_ = msg;
}

void IBodyFrame::ReceiveBodyFrame(oscMessage* msg, int &i)
{
	relative_time_ = std::strtoll(msg->GetArgumentAtIndex(i++).GetStringValue().c_str(), 0, 10);
	
	floor_clip_plane_.x = msg->GetArgumentAtIndex(i++).GetFloatValue();
	floor_clip_plane_.y = msg->GetArgumentAtIndex(i++).GetFloatValue();
	floor_clip_plane_.z = msg->GetArgumentAtIndex(i++).GetFloatValue();
	floor_clip_plane_.w = msg->GetArgumentAtIndex(i++).GetFloatValue();
}

} // namespace kv2streamer
