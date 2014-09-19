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
#include "oscReceiver.h"
#include "oscMessage.h"

namespace kv2s {

oscReceiver::oscReceiver(int port, std::string multicast_IP, MessageRetrievalMode retrievalMode_)
: socket(IpEndpointName(multicast_IP.c_str(), port), this), isListening(false)
, currentMessage(0)
, retrievalMode(retrievalMode_)
{}

oscReceiver::oscReceiver(int port, MessageRetrievalMode retrievalMode_)
: socket(IpEndpointName( IpEndpointName::ANY_ADDRESS, port ), this ), isListening(false)
, currentMessage(0)
, retrievalMode(retrievalMode_)
{}

oscReceiver::~oscReceiver()
{
	socket.AsynchronousBreak();	
	listeningThread.join();
}

void oscReceiver::ProcessMessage(const osc::ReceivedMessage &_m, const IpEndpointName &remoteEndpoint)
{
	boost::mutex::scoped_lock lock(listenerMutex);
	
	if (retrievalMode == SIGNAL_PARSER_ONLY_AFTER_RELEASE)
	{
		if (currentMessage != 0) return;
	}
	
	osc::ReceivedMessage::const_iterator arg = _m.ArgumentsBegin();
	
	currentMessage = new oscMessage(_m.AddressPattern(), oscMessage::NO_BUFFER);
	
	// set source IP
	char sourceString[ IpEndpointName::ADDRESS_STRING_LENGTH ];
    remoteEndpoint.AddressAsString( sourceString  );
	currentMessage->sourceIP = sourceString;
	
	while ( arg != _m.ArgumentsEnd() )
	{
		if 		( arg->IsBool() 	) { currentMessage->AppendBoolean	(arg->AsBool());}
		else if ( arg->IsInt32() 	) { currentMessage->AppendInteger	(arg->AsInt32());}
		else if ( arg->IsFloat() 	) { currentMessage->AppendFloat		(arg->AsFloat());}
		else if ( arg->IsString() 	) { currentMessage->AppendString	(arg->AsString());}
		else { std::cout << "Unrecognized osc argument type\n"; }
		arg++;
	}
	
	signalNewOscMessageReceived(currentMessage);
}

void oscReceiver::ReleaseMessage()
{
	boost::mutex::scoped_lock lock(listenerMutex);
	if (currentMessage) delete currentMessage;
	currentMessage = 0;
}

void oscReceiver::listening() 
{
	socket.Run();
}

} // namespace kv2streamer
