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

#include <iostream>
#include "GstreamerPipelines.h"
#include "GstAppSrcPipeline.h"

namespace kv2s {

GstAppSrcPipeline::GstAppSrcPipeline(std::string _multicastIP, int _port)
: needsData(false)
, multicastIP(_multicastIP)
, port(_port)
{}

GstAppSrcPipeline::~GstAppSrcPipeline(void){}

void GstAppSrcPipeline::Initialize(std::string pipelineString)
{
	GstPipelineWrapper::InitializePipelineWithString(pipelineString);

//	std::cout << pipelineString << std::endl;

	appsrc = GstPipelineWrapper::GetElementByName(APPSRC_NAME);

	GstAppSrcCallbacks appsrcCallbacks;

	appsrcCallbacks.need_data	= GstAppSrcPipeline::NeedDataCallback;
	appsrcCallbacks.enough_data	= GstAppSrcPipeline::EnoughDataCallback;
	appsrcCallbacks.seek_data	= GstAppSrcPipeline::SeekDataCallback;

	gst_app_src_set_callbacks(GST_APP_SRC(appsrc), &appsrcCallbacks, this, (GDestroyNotify) GstAppSrcPipeline::DestroyCallback);
	gst_app_src_set_max_bytes(GST_APP_SRC(appsrc), 0);

}

bool GstAppSrcPipeline::GetNeedsData() const
{
	boost::mutex::scoped_lock(pushBufferFlagMutex);
	return needsData;
}

bool GstAppSrcPipeline::SendFrame(unsigned char* frameBuffer, unsigned int bufferSize)
{
	boost::mutex::scoped_lock(pushBufferFlagMutex);
	if (!needsData) {
		std::cout << "CHANGED\n";	
		return false;
	}
	GstBuffer* buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, frameBuffer, bufferSize, 0, bufferSize, this, (GDestroyNotify) GstAppSrcPipeline::DestroyCallback);
	
	if (buffer)
	{	
		if (needsData) GstFlowReturn ret = gst_app_src_push_buffer((GstAppSrc*)appsrc, buffer); // buffer released automatically	
	}	
	//needsData = false;
	return true;
}

void GstAppSrcPipeline::DestroyCallback(GstAppSrcPipeline* multicaster)
{
	// DO NOTHING
}

void GstAppSrcPipeline::NeedDataCallback(GstAppSrc* appsrc, guint length, gpointer user_data)
{
	GstAppSrcPipeline* multicaster = (GstAppSrcPipeline*) user_data;
	boost::mutex::scoped_lock(multicaster->pushBufferFlagMutex);
	multicaster->needsData = true;
	//std::cout << "NEED\n";
}

void GstAppSrcPipeline::EnoughDataCallback(GstAppSrc* appsrc, gpointer user_data)
{
	GstAppSrcPipeline* multicaster = (GstAppSrcPipeline*) user_data;
	boost::mutex::scoped_lock(multicaster->pushBufferFlagMutex);
	multicaster->needsData = false;
	std::cout << "ENOUGH===================================================================================\n";
}

gboolean GstAppSrcPipeline::SeekDataCallback(GstAppSrc* appsrc, guint64 offset, gpointer user_data)
{
	std::cout << "SEEK DATA CALLBACK: SHOULD NEVER BE HERE!\n";
	return false;
}

} // namespace kv2streamer