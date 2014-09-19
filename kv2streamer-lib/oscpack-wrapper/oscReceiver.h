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
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

#include <oscpack/osc/OscReceivedElements.h>
#include <oscpack/osc/OscPacketListener.h>
#include <oscpack/ip/UdpSocket.h>

namespace kv2s {

class oscMessage;
class oscReceiver : public osc::OscPacketListener
{
public:

	enum MessageRetrievalMode
	{
		SIGNAL_PARSER_WHEN_MSG_ARRIVES, // User OWNS the msg, so user responsible for deleting. Parser will receive all messages.
		SIGNAL_PARSER_ONLY_AFTER_RELEASE // User must call ReleaseMessage() before user can acquire next message (messages might be dropped);
	};

	oscReceiver(int port, std::string multicast_IP, MessageRetrievalMode retrievalMode = SIGNAL_PARSER_ONLY_AFTER_RELEASE);
	oscReceiver(int port, MessageRetrievalMode retrievalMode = SIGNAL_PARSER_ONLY_AFTER_RELEASE);
	~oscReceiver();

	void set_retrieval_mode(MessageRetrievalMode retrieval_mode);
	void ReleaseMessage();


	template<typename T>
	void ListenWithParser(const T &message_parser)
	{
		if (isListening) 
		{
			std::cout << "OSC receiver is already listening, returning...\n";
			return;
		}
		boost::mutex::scoped_lock lock(listenerMutex);
		signalNewOscMessageReceived.connect(message_parser);
		listeningThread = boost::thread(&oscReceiver::listening, this);
		
		isListening = true;
	}
	
private:
	UdpListeningReceiveSocket 	socket;

	bool 						isListening;
	oscMessage*					currentMessage;
	
	boost::thread				listeningThread;
	boost::mutex				listenerMutex;
	boost::signals2::signal<void (oscMessage*)> 	signalNewOscMessageReceived;
	
	MessageRetrievalMode retrievalMode;

	virtual void ProcessMessage(const osc::ReceivedMessage &m, const IpEndpointName &remoteEndpoint);
	
	void listening();
};

} // namespace kv2streamer
