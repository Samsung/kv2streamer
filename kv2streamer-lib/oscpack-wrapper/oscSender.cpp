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

#include "oscSender.h"
#include "oscMessage.h"

namespace kv2s {

oscSender::oscSender(std::string IP_address, int destination_port)
: transmitSocket(IpEndpointName(IP_address.c_str(), destination_port))
{}

oscSender::~oscSender()
{}

void oscSender::Send(oscMessage &msg)
{
	msg.endMessage();
	transmitSocket.Send(msg.packetStream->Data(), msg.packetStream->Size());
}

void oscSender::SetMulticastInterface(std::string interface_name)
{
	transmitSocket.SetMulticastInterface(interface_name.c_str());
}

} // namespace kv2streamer
