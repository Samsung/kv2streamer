#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

#define SUBFRAME_LENGTH 1024

class AudioMulticaster : public AbstractMulticaster
{
public:
	AudioMulticaster(std::string multicastIP, int port);
	~AudioMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IAudioBeamFrameReader*	audioBeamFrameReader;
	kv2s::GstAppSrcPipeline	gstSender;
	UINT					subframeLength;
	BYTE*					audioSubFrameBuffer;
	void					ProcessNewAudioSubFrame(IAudioBeamSubFrame* subFrame);
};

