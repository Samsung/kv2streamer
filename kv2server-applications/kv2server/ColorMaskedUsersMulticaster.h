#pragma once

#include "AbstractMulticaster.h"
#include <gst-wrapper/GstAppSrcPipeline.h>

class ColorMaskedUsersMulticaster : public AbstractMulticaster
{
public:
	ColorMaskedUsersMulticaster(std::string multicastIP, int port);
	~ColorMaskedUsersMulticaster(void);

	virtual HRESULT InitializeMulticasterAndSubscribeHandle(IKinectSensor* kinectSensor, WAITABLE_HANDLE &waitableHandle);
	virtual void ProcessNewFrame(WAITABLE_HANDLE &waitableHandle);

private:

	IMultiSourceFrameReader*	multiFrameReader;
	ICoordinateMapper*			coordinateMapper;

	kv2s::GstAppSrcPipeline		gstSender;
	
	RGBQUAD						default_black; 

	void						ProcessNewColorAndDepthFrame(IColorFrame* colorFrame, IDepthFrame* depthFrame, IBodyIndexFrame* bodyIndexFrame);
	void						ComposeOutputColorMaskedUsersBuffer(UINT16* depthBuffer, USHORT maxReliableDistance, USHORT minReliableDist, BYTE* bodyIndexBuffer);

	ColorSpacePoint*			pColorSpacePoints;
	RGBQUAD*					pInputFullRGBX;
	RGBQUAD*					pOutputColorMaskedUsers;						
};

