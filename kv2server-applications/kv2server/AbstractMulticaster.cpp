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

#include "AbstractMulticaster.h"
#include <kv2client/SharedConfig.h>

AbstractMulticaster::AbstractMulticaster(std::string multicast_IP, int port_, MulticastingMode mode)
	: multicastIP(multicast_IP)
	, port(port_)
	, multicastingMode(mode)
{}

AbstractMulticaster::~AbstractMulticaster(){}

HRESULT AbstractMulticaster::Initialize(IKinectSensor* kinectSensor)
{
	// delegating to subclass
	HRESULT hr = InitializeMulticasterAndSubscribeHandle(kinectSensor, waitableHandle); 
	
	multicastingThread = boost::thread(&AbstractMulticaster::Multicasting, this);
	return hr;
}


void AbstractMulticaster::ResetTimeout()
{
	boost::mutex::scoped_lock(timerMutex);
	timer = 0;
}

void AbstractMulticaster::Multicasting()
{	
	timer = SERVER_STREAM_TIMEOUT_SECONDS*1000;
	time = timeGetTime();
	while(true)
	{
		if (WaitForSingleObject(reinterpret_cast<HANDLE>(waitableHandle), INFINITE) == WAIT_OBJECT_0) // event received
		{
			if (multicastingMode == BY_REQUEST_ONLY)
			{ // check if request is valid
				boost::mutex::scoped_lock(timerMutex);
				timer = min(timer + timeGetTime()-time, SERVER_STREAM_TIMEOUT_SECONDS*1000);
				time = timeGetTime();

				if (  timer >= SERVER_STREAM_TIMEOUT_SECONDS*1000) continue;
			}

			// delegating to subclass
			ProcessNewFrame(waitableHandle);
		}
	}
 }
