#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

class ColorMulticaster : public AbstractMulticaster
{
public:
	ColorMulticaster(std::string multicastIP, int port);
	~ColorMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IColorFrameReader*	colorFrameReader;
	kv2s::GstAppSrcPipeline	gstSender;
	void				ProcessNewColorFrame(IColorFrame* newFrame);
	RGBQUAD*			pBuffer;
};

