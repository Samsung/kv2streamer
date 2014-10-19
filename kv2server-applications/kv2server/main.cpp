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

#include "KV2Server.h"

int main(int argc, char **argv)
{
	std::string multicastIP, filename;
	if (argc == 3)
	{
		multicastIP = argv[1];
		filename	= argv[2];
	}
	else
	{
		std::cout << "Incorrect arguments specified, please specify the multicast IP address and sensor's conversion table filename excluding extension: \n";
		std::cout << "Example: kv2server_application.exe 224.1.1.1 kv2sensor01\n";
		std::cout << "\n Press Ctrl+C to quit...";
		while(1);
		return -1;
	}

	KV2Server server(multicastIP, filename);
	server.Block();
	return 0;
}
