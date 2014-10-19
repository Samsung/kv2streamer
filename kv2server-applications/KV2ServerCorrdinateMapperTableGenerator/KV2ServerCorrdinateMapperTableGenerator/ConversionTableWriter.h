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

#include <string>
#include "Kinect.h" 

#define K4W2_DEPTH_WIDTH	512
#define K4W2_DEPTH_HEIGHT	424

class ConversionTableWriter
{
public:

	ConversionTableWriter(void);
	~ConversionTableWriter(void);

	void PrintTable();
	void RetrieveUniqueKinectId();

	void WriteTableToBinaryFile(std::string output_filename);
	void ValidateBinaryFile(std::string filename);

private:

	IKinectSensor*		kinectSensor;
	ICoordinateMapper*	coordinateMapper;

	PointF*				conversionTable;
};

