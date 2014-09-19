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

#pragma once

#include <gst/app/gstappsink.h>
#include "GstPipelineWrapper.h"

namespace kv2s {

class GstAppSinkPipeline : public GstPipelineWrapper
{
public:
	GstAppSinkPipeline(std::string multicastIP, int port);
	~GstAppSinkPipeline();

	void Initialize(std::string pipelineString);

	bool GetIsNewFrameAvailable();
	bool GetLatestFrameBuffer(void** frameBuffer);
	void ReleaseFrameBuffer();

	void set_is_streaming(bool is_streaming);

private:
	bool 			is_streaming_;

	std::string		multicastIP;
	int				port;

	GstElement* 	appsink;

	boost::mutex	bufferMutex;
	void			ReceiveNewSample();
	GstSample*		retrievedBuffer;
	GstSample*		currentBuffer;

	static void				DestroyCallback		(gpointer data);
	static void				EndOfStreamCallback	(GstAppSink* appsink, gpointer user_data);
	static GstFlowReturn	NewPrerollCallback	(GstAppSink* appsink, gpointer user_data);
	static GstFlowReturn	NewSampleCallback	(GstAppSink* appsink, gpointer user_data);
};

} // namespace kv2streamer
