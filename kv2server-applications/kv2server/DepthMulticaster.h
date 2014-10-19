#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

class DepthMulticaster : public AbstractMulticaster
{
public:
	DepthMulticaster(std::string multicastIP, int port);
	~DepthMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IDepthFrameReader*		depthFrameReader;
	kv2s::GstAppSrcPipeline	gstSender;
	void					ProcessNewDepthFrame(IDepthFrame* newFrame);
	UINT16*					pBuffer;
};

