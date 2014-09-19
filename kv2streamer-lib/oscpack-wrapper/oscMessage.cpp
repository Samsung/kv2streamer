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
#include "oscMessage.h"

namespace kv2s {

/******************************
 OSC Argument
*******************************/

oscArgument::oscArgument(std::string	arg) {argType = String; 	stringValue  = arg;}
oscArgument::oscArgument(int			arg) {argType = Integer; 	integerValue = arg;}
oscArgument::oscArgument(float 			arg) {argType = Float; 		floatValue   = arg;}
oscArgument::oscArgument(bool 			arg) {argType = Boolean; 	boolValue    = arg;}

// type checking:
bool oscArgument::IsBoolean() 	const {return argType == Boolean;}
bool oscArgument::IsInteger() 	const {return argType == Integer;}
bool oscArgument::IsFloat()		const {return argType == Float;}
bool oscArgument::IsString()	const {return argType == String;}
oscArgument::argumentType oscArgument::getArgType() const {return argType;}
	
// value getters:
bool 		oscArgument::GetBooleanValue()	const	{return boolValue;}
int 		oscArgument::GetIntegerValue()	const	{return integerValue;}
float		oscArgument::GetFloatValue()	const	{return floatValue;}
std::string	oscArgument::GetStringValue()	const	{return stringValue;}

oscArgument& oscArgument::operator= (const oscArgument &rhs){
	if( this != &rhs )
	{
		this->argType 		= rhs.getArgType();
		this->stringValue 	= rhs.GetStringValue();
		this->integerValue 	= rhs.GetIntegerValue();
		this->floatValue 	= rhs.GetFloatValue();
		this->boolValue 	= rhs.GetBooleanValue();
	}
	return *this;
}

oscArgument::oscArgument(const oscArgument &rhs){
	this->argType 		= rhs.getArgType();
	this->stringValue 	= rhs.GetStringValue();
	this->integerValue 	= rhs.GetIntegerValue();
	this->floatValue 	= rhs.GetFloatValue();
	this->boolValue 	= rhs.GetBooleanValue();
}

/******************************
 OSC Message
*******************************/
void 	oscMessage::AppendString	(std::string arg) 	{arguments.push_back(oscArgument(arg)); if (packetStream) *packetStream << arg.c_str();}
void 	oscMessage::AppendFloat 	(float arg)			{arguments.push_back(oscArgument(arg)); if (packetStream) *packetStream << arg;}
void 	oscMessage::AppendInteger 	(int   arg)			{arguments.push_back(oscArgument(arg)); if (packetStream) *packetStream << arg;}
void 	oscMessage::AppendBoolean	(bool  arg)			{arguments.push_back(oscArgument(arg)); if (packetStream) *packetStream << arg;}
	
int		oscMessage::GetNumberOfArguments()		  const {return arguments.size();}
	
oscArgument	oscMessage::GetArgumentAtIndex(int i) const	{return arguments[i];}
std::string	oscMessage::GetAddress()			  const	{return address;}
std::string	oscMessage::GetSourceIP()			  const {return sourceIP;}


oscMessage::oscMessage(std::string _address, OSC_MESSAGE_BUFFER_SIZE osc_buffer_size)
{
	switch (osc_buffer_size)
	{
	case NO_BUFFER:
		{
			buffer = 0;
			packetStream = 0;
			break;
		}
	case SMALL_BUFFER:
		{
			buffer = new char[SMALL_BUFFER_SIZE];
			packetStream = new osc::OutboundPacketStream(buffer, SMALL_BUFFER_SIZE);
			break;
		}
	case LARGE_BUFFER:
		{
			buffer = new char[LARGE_BUFFER_SIZE];
			packetStream = new osc::OutboundPacketStream(buffer, LARGE_BUFFER_SIZE);
			break;
		}
	case HUGE_BUFFER:
		{
			buffer = new char[HUGE_BUFFER_SIZE];
			packetStream = new osc::OutboundPacketStream(buffer, HUGE_BUFFER_SIZE);
			break;
		}

	}

	if (packetStream) *packetStream << osc::BeginMessage(_address.c_str());
	address = _address;
	hasEnded = false;
	sourceIP = "0.0.0.0";
}

oscMessage& oscMessage::operator= (const oscMessage &rhs)
{
	if( this != &rhs )
	{	
		this->address 	= rhs.GetAddress();
		this->sourceIP  = rhs.GetSourceIP();
		this->hasEnded 	= rhs.hasEnded;		
		this->arguments = rhs.arguments;
	}
	return *this;
}

oscMessage::~oscMessage()
{
	if (packetStream)	delete		packetStream;
	if (buffer)			delete[]	buffer;
}

void oscMessage::endMessage()
{
	if (hasEnded) return;
	
	*packetStream << osc::EndMessage;
	hasEnded = true;
}

} // namespace kv2streamer
