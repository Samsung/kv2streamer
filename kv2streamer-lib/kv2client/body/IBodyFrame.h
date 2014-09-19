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
 * @brief This file contains a KV2Client IBodyFrame class definition.
 * 
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include "BodyFrameStreamer.h"

#define BODY_COUNT 6

namespace kv2s {

class IBodyFrameSource; 
class IBody;	
typedef boost::shared_ptr<IBody> IBodyPtr;


/**
 * @class IBodyFrame
 * @brief Manages the body data retrieved from sensor.
 */
class IBodyFrame
{
public:

	/**
	 * Retrieves and/or udpates an array of IBody's using data in this IBodyFrame.
	 * Use this method to access the body data in this frame.
	 * User OWNS the IBodyPtr's returned.
	 * 
	 * @param UINT capacity: the array size of the next parameter (bodies).
	 * @param IBodyPtr* bodies: an array of IBodyPtr's, non-initialized pointers are ok. Data will be loaded into this array.
	 * @return true if successful, false otherwise.
	 */
	bool GetAndRefreshBodyData(UINT capacity, IBodyPtr* bodies);
	
	/**
	 * Retrieves the floor clip plane.
	 * 
	 * @param Vector4* floorClipPlane:
	 * @return true if successful, false otherwise.
	 */
	bool get_FloorClipPlane(Vector4* floorClipPlane);
	
	/**
	 * Retrieves the relative time. 
	 * 
	 * @param TIMESPAN* relativeTime (signed long long*): time stamp of the frame from the server. 
	 * @return true if successful, false otherwise.
	 */
	bool get_RelativeTime(TIMESPAN *relativeTime);
	
	/**
	 * Retrieves the IBodyFrameSource. This API is provided in the K4W2 SDK but
	 * does not make sense to have in KV2Client, so this method will ALWAYS return false.
	 * and does NOTHING with the passed pointer.
	 * 
	 * @param IBodyFrameSource* bodyFrameSource object. 
	 * @return false always, never true.
	 */
	bool get_BodyFrameSource(IBodyFrameSource* bodyFrameSource);

	/**
	 * Destructor of IBodyFrame.
	 * 
	 * @remarks More importantly, notifies the BodyFrameStreamer that this frame has been deleted.
	 * so the user can retrieve new frames from it. The user will not be able to retrieve new
	 * frames from a FrameStreamer until the last retrieved frame is released.
	 * 
	 * @note The use of shared_ptr should guarantee the destruction of a frame that goes out of scope.
	 */
	~IBodyFrame();


private:
	friend class BodyFrameStreamer;
	IBodyFrame(BodyFrameStreamerPtr streamer); /** Private constructor: can only be called by BodyFrameStreamer */
	
private:
	void ParseBodyFrameMessage	(oscMessage* msg);
	void ReceiveBodyFrame		(oscMessage* msg, int &i);

	/** 
	 * Pointer to the streamer that generates this frame.
	 * Used in the destructor to notify the streamer
	 * that this frame has been destroyed, thereby allowing 
	 * the user to pull more frames.
	 * */
	BodyFrameStreamerPtr		streamer_;
	
	/** Frame Data */
	TIMESPAN 					relative_time_;
	Vector4						floor_clip_plane_;
	oscMessage*					current_body_frame_msg_;
};

} // namespace kv2streamer
