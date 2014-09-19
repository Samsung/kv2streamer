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
#include <vector>
#include <oscpack/osc/OscOutboundPacketStream.h>

#define SMALL_BUFFER_SIZE	512
#define LARGE_BUFFER_SIZE	16000
#define HUGE_BUFFER_SIZE	3000000

namespace kv2s {

class oscArgument
{
public:
	enum argumentType
	{
		Integer,
		String,
		Boolean,
		Float,
	};

	// constructors:
	oscArgument(std::string arg);
	oscArgument(int			arg);
	oscArgument(float 		arg);
	oscArgument(bool 		arg);

	// type checking:
	bool IsBoolean()	const;
	bool IsInteger() 	const;
	bool IsFloat()		const;
	bool IsString()		const;
	argumentType getArgType() const;
	
	// value getters:
	bool 		GetBooleanValue() const;
	int 		GetIntegerValue() const;
	float		GetFloatValue()	  const; 
	std::string	GetStringValue()  const;
	
protected:
	argumentType argType;
	
	std::string stringValue;
	int			integerValue;
	float		floatValue;
	bool		boolValue;
	
public:
	oscArgument& operator= (const oscArgument &rhs);
	oscArgument(const oscArgument &rhs);
};

/*DOES NOT SUPPORT RESENDING RECEIVED MESSAGE YET*/
class oscMessage
{
public:

	enum OSC_MESSAGE_BUFFER_SIZE
	{
		NO_BUFFER,
		SMALL_BUFFER,
		LARGE_BUFFER,
		HUGE_BUFFER
	};
	
	oscMessage(std::string address, OSC_MESSAGE_BUFFER_SIZE buffer_size = SMALL_BUFFER);
	~oscMessage();
	
	oscMessage& operator= (const oscMessage &rhs);
	
	void 	AppendString	(std::string arg);
	void 	AppendFloat 	(float arg);
	void 	AppendInteger 	(int   arg);
	void 	AppendBoolean	(bool  arg);
	
	int		GetNumberOfArguments()		  const;
	
	oscArgument	GetArgumentAtIndex(int i) const; 
	std::string	GetAddress()			  const;
	std::string	GetSourceIP()			  const;	

	
friend class oscSender;
friend class oscReceiver; 
private:

	std::string						address;
	std::string						sourceIP; // "" by default, for outgoing message
	
	osc::OutboundPacketStream*	packetStream;

	char*						buffer;
	
	bool 						hasEnded;
	std::vector<oscArgument> 	arguments;
	
	void endMessage();
};

} // namespace kv2streamer
