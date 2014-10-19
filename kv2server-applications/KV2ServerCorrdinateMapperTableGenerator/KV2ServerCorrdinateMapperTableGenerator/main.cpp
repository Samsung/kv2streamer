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

#include <iostream>

#include "ConversionTableWriter.h"

int main(int argc, char** argv)
{
	std::string filename;

	std::cout << "This application will generate a .kv2dat binary which contains the conversion table retrieved from a specific Kinect V2 sensor.\n\n";
	std::cout << "Please enter the name of your table WITHOUT the .kv2dat extension (e.g. KV2SENSOR01 ):\n";
	std::cin >> filename;

	filename += ".kv2dat";

	ConversionTableWriter writer;

	//writer.RetrieveUniqueKinectId();
	//writer.PrintTable();
	writer.WriteTableToBinaryFile(filename);
	writer.ValidateBinaryFile(filename);

	std::cout << "Reached end of program: Press Ctrl+C to quit.\n";
	while(1);
	return 0;
}
