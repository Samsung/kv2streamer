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
#include <oscpack/ip/UdpSocket.h>

namespace kv2s {

class oscMessage;

class oscSender
{
public:

	oscSender(std::string IP_address, int destination_port);
	~oscSender();
	
	void SetMulticastInterface(std::string interface_name); // WARNING: not implemented on Windows YET
	void Send (oscMessage &msg);

private:
	
	UdpTransmitSocket transmitSocket;
};

} // namespace kv2streamer
