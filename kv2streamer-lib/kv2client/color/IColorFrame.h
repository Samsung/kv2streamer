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
 * @brief This file contains IColorFrame class definition.
 * 
 */

#pragma once

#include "ColorFrameStreamer.h"

namespace kv2s {

/**
 * @class IColorFrame
 * @brief Manages the color data retrieved from sensor.
 */
class IColorFrame
{
public:

	/**
	 * Retrieves the RGBA color buffer data from color frame, including its size.
	 * 
	 * @param UINT* capacity: writes the size of the buffer to this param.
	 * @param unsigned char** buffer: make buffer points to the color buffer
	 * @return true if successful, false otherwise.
	 */
	bool AccessRawUnderlyingBuffer(UINT* capacity, unsigned char** buffer);
	
	/**
	 * Destructor of IColorFrame.
	 * 
	 * @remarks More importantly, notifies the ColorFrameStreamer that this frame has been deleted.
	 * so the user can retrieve new frames from it. The user will not be able to retrieve new
	 * frames from a FrameStreamer until the last retrieved frame is released.
	 * 
	 * @note The use of shared_ptr should guarantee the destruction of a frame that goes out of scope.
	 */

	~IColorFrame();

private:
	friend class ColorFrameStreamer;
	IColorFrame(ColorFrameStreamerPtr streamer);
	
private:
	ColorFrameStreamerPtr streamer;
	
	unsigned char* frameBufferReference;
	unsigned char** GetColorFrameBufferReferencePtr();
};

} // namespace kv2streamer
