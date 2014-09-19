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

#include <fstream>
#include <kv2client/SharedConfig.h>

#include "CoordinateMapperReceiver.h"

namespace kv2s {

CoordinateMapperReceiver::CoordinateMapperReceiver(int port)
: receiver(port, oscReceiver::SIGNAL_PARSER_ONLY_AFTER_RELEASE)
, is_table_loaded_(false)
{
	receiver.ListenWithParser(boost::bind(&CoordinateMapperReceiver::ReceiveConversionTableMsg, this, _1));	
	conversion_table_ = new PointF[K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT];
}

CoordinateMapperReceiver::~CoordinateMapperReceiver()
{
	if (conversion_table_) delete[] conversion_table_;
}

void CoordinateMapperReceiver::ReceiveConversionTableMsg( oscMessage *msg )
{
	if (!is_table_loaded_ && msg->GetAddress() == "conversiontable")
	{
		std::string filename = msg->GetArgumentAtIndex(0).GetStringValue() + ".kv2dat";
		
		std::ifstream instream(filename.c_str(), std::ios::in | std::ios::binary);

		std::cout << "Attempting to load " << filename << std::endl;
		if (!instream.is_open())
		{
		  throw std::runtime_error("ERROR: conversion table for coord mapper not found!\n Press Ctrl+C to exit!"); 
		}
		instream.read((char*) conversion_table_, K4W2_DEPTH_WIDTH * K4W2_DEPTH_HEIGHT * sizeof(PointF));
		instream.close();
		
		std::cout << "Successfully loaded depth->world conversion table from " << filename << std::endl;
		is_table_loaded_ = true;
	}
	receiver.ReleaseMessage();
}

bool CoordinateMapperReceiver::get_is_table_loaded() const
{
	return is_table_loaded_;
}

bool CoordinateMapperReceiver::MapDepthSpaceToCameraSpace(DepthSpacePoint depthPoint, unsigned short depth, CameraSpacePoint *cameraPoint)
{
	if (!is_table_loaded_) return false;
	
	PointF ray = conversion_table_[(int)(depthPoint.X + depthPoint.Y * K4W2_DEPTH_WIDTH)];
	
	CameraSpacePoint output;
	
	output.X = ray.X * depth;
	output.Y = ray.Y * depth;
	output.Z = depth;
	
	*cameraPoint = output;
	return true;;
}

} // namespace kv2streamer
