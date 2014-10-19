#pragma once

#include "Kinect.h"
#include <boost/thread.hpp>

class AbstractMulticaster
{
public:

	enum MulticastingMode
	{
		ALWAYS_MULTICASTING,
		BY_REQUEST_ONLY,
	};

	AbstractMulticaster(std::string multicastIP, int port, MulticastingMode mode = BY_REQUEST_ONLY);
	virtual ~AbstractMulticaster(void);

	HRESULT Initialize(IKinectSensor* kinectSensor);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle) = 0;
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle) = 0;

	void ResetTimeout();

protected:
	std::string multicastIP;
	int			port;

private:
	WAITABLE_HANDLE		waitableHandle;
	
	boost::mutex		timerMutex;
	int					timer;
	int					time;

	boost::thread		multicastingThread;
	void				Multicasting();
	MulticastingMode	multicastingMode;
};

