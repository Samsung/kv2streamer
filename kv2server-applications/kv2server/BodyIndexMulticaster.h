#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

class BodyIndexMulticaster : public AbstractMulticaster
{
public:
	BodyIndexMulticaster(std::string multicastIP, int port);
	~BodyIndexMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IBodyIndexFrameReader*	bodyIndexFrameReader;
	kv2s::GstAppSrcPipeline	gstSender;
	void					ProcessNewBodyIndexFrame(IBodyIndexFrame* newFrame);
	BYTE*					pBuffer;
	BYTE*					pScaledBuffer;
	bool					frameReady;
};

