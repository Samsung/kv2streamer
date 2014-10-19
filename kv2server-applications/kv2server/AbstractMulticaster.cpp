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
