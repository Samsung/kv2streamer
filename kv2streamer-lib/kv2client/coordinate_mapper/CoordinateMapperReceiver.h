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

#include <oscpack-wrapper/oscWrapper.h>
#include <kv2client/utils/Structures.h>

namespace kv2s {

class CoordinateMapperReceiver
{
public:
	CoordinateMapperReceiver(int port);
	~CoordinateMapperReceiver();

	bool get_is_table_loaded() const;
	
	bool MapDepthSpaceToCameraSpace(DepthSpacePoint depthPoint, unsigned short depth, CameraSpacePoint *cameraPoint);
	
private:
	
	bool	is_table_loaded_;
	PointF* conversion_table_;
	
	oscReceiver	receiver;
	void ReceiveConversionTableMsg( oscMessage *msg );

};

} // namespace kv2streamer
