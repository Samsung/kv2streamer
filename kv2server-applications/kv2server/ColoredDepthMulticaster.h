#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

class ColoredDepthMulticaster : public AbstractMulticaster
{
public:
	ColoredDepthMulticaster(std::string multicastIP, int port);
	~ColoredDepthMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IMultiSourceFrameReader*	multiFrameReader;
	ICoordinateMapper*			coordinateMapper;

	kv2s::GstAppSrcPipeline		gstSender;
	
	RGBQUAD						default_black; 

	void						ProcessNewColorAndDepthFrame(IColorFrame* colorFrame, IDepthFrame* depthFrame);
	void						ComposeOutputColoredDepthBuffer(UINT16* depthBuffer, USHORT maxReliableDistance, USHORT minReliableDist);

	ColorSpacePoint*			pColorSpacePoints;
	RGBQUAD*					pInputFullRGBX;
	RGBQUAD*					pOutputColoredDepth;						
};

