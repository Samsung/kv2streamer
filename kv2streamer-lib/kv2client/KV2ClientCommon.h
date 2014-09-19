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

#include <kv2client/SharedConfig.h>

#include <kv2client/client/KV2Client.h>

#include <kv2client/body/BodyFrameStreamer.h>
#include <kv2client/body/IBodyFrame.h>
#include <kv2client/body/IBody.h>

#include <kv2client/color/ColorFrameStreamer.h>
#include <kv2client/color/IColorFrame.h>

#include <kv2client/depth/DepthFrameStreamer.h>
#include <kv2client/depth/IDepthFrame.h>

#include <kv2client/color_masked_users/ColorMaskedUsersFrameStreamer.h>
#include <kv2client/color_masked_users/IColorMaskedUsersFrame.h>

#include <kv2client/colored_depth/ColoredDepthFrameStreamer.h>
#include <kv2client/colored_depth/IColoredDepthFrame.h>

#include <kv2client/body_index/BodyIndexFrameStreamer.h>
#include <kv2client/body_index/IBodyIndexFrame.h>

#include <kv2client/audio/AudioFrameStreamer.h>
#include <kv2client/audio/IAudioBeamSubFrame.h>

#include <kv2client/coordinate_mapper/CoordinateMapperReceiver.h>
