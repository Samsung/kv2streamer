 #
 # Copyright (c) 2014 Samsung Electronics Corporation.
 # 
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 # 
 #    http://www.apache.org/licenses/LICENSE-2.0
 # 
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.


## BASE_STREAMER
SET (BASE_STREAMER_SRC
     base_streamer/BaseFrameStreamer.cpp
    )

set (BASE_STREAMER_HEADERS
     base_streamer/BaseFrameStreamer.h
    )

## AUDIO
SET (AUDIO_SRC
     audio/AudioFrameStreamer.cpp
     audio/IAudioBeamSubFrame.cpp
    )

set (AUDIO_HEADERS
     audio/AudioFrameStreamer.h
     audio/IAudioBeamSubFrame.h
    )

## BODY
SET (BODY_SRC
     body/BodyFrameStreamer.cpp
     body/IBodyFrame.cpp
     body/IBody.cpp
    )

set (BODY_HEADERS
     body/BodyFrameStreamer.h
     body/IBodyFrame.h
     body/IBody.h
    )

## BODY_INDEX
SET (BODY_INDEX_SRC
     body_index/BodyIndexFrameStreamer.cpp
     body_index/IBodyIndexFrame.cpp
    )

set (BODY_INDEX_HEADERS
     body_index/BodyIndexFrameStreamer.h
     body_index/IBodyIndexFrame.h
    )

## CLIENT
SET (CLIENT_SRC
     client/KV2Client.cpp
    )

set (CLIENT_HEADERS
     client/KV2Client.h
    )

## COLOR
SET (COLOR_SRC
     color/ColorFrameStreamer.cpp
     color/IColorFrame.cpp
    )

set (COLOR_HEADERS
     color/ColorFrameStreamer.h
     color/IColorFrame.h
    )

## DEPTH
SET (DEPTH_SRC
     depth/DepthFrameStreamer.cpp
     depth/IDepthFrame.cpp
    )

set (DEPTH_HEADERS
     depth/DepthFrameStreamer.h
     depth/IDepthFrame.h
    )

## COLORED_DEPTH
SET (COLORED_DEPTH_SRC
     colored_depth/ColoredDepthFrameStreamer.cpp
     colored_depth/IColoredDepthFrame.cpp
    )

set (COLORED_DEPTH_HEADERS
     colored_depth/ColoredDepthFrameStreamer.h
     colored_depth/IColoredDepthFrame.h
    )

## COLOR_MASKED_USERS
SET (COLOR_MASKED_USERS_SRC
     color_masked_users/ColorMaskedUsersFrameStreamer.cpp
     color_masked_users/IColorMaskedUsersFrame.cpp
    )

set (COLOR_MASKED_USERS_HEADERS
     color_masked_users/ColorMaskedUsersFrameStreamer.h
     color_masked_users/IColorMaskedUsersFrame.h
    )

## COORDINATE_MAPPER
SET (COORDINATE_MAPPER_SRC
     coordinate_mapper/CoordinateMapperReceiver.cpp
    )

set (COORDINATE_MAPPER_HEADERS
     coordinate_mapper/CoordinateMapperReceiver.h
    )

## UTILS
set (UTILS_HEADERS
     utils/Enumerations.h
     utils/Structures.h
    )

################################################
SET (SOURCES
     ${BASE_STREAMER_SRC} 
     ${AUDIO_SRC} 
     ${BODY_SRC} 
     ${BODY_INDEX_SRC}
     ${CLIENT_SRC} 
     ${DEPTH_SRC} 
     ${COLOR_SRC} 
     ${COLORED_DEPTH_SRC}
     ${COLOR_MASKED_USERS_SRC}
     ${COORDINATE_MAPPER_SRC} 
    )

set (HEADERS
    KV2ClientCommon.h
    SharedConfig.h
   )

#add_library (kv2client ${SOURCES})
add_library (kv2client SHARED ${SOURCES})

install (TARGETS kv2client DESTINATION lib)

install (FILES ${HEADERS} DESTINATION include/kv2client)
install (FILES ${BASE_STREAMER_HEADERS} DESTINATION include/kv2client/base_streamer)
install (FILES ${AUDIO_HEADERS} DESTINATION include/kv2client/audio)
install (FILES ${BODY_HEADERS} DESTINATION include/kv2client/body)
install (FILES ${BODY_INDEX_HEADERS} DESTINATION include/kv2client/body_index)
install (FILES ${CLIENT_HEADERS} DESTINATION include/kv2client/client)
install (FILES ${COLOR_HEADERS} DESTINATION include/kv2client/color)
install (FILES ${COORDINATE_MAPPER_HEADERS} DESTINATION include/kv2client/coordinate_mapper)
install (FILES ${DEPTH_HEADERS} DESTINATION include/kv2client/depth)
install (FILES ${COLORED_DEPTH_HEADERS} DESTINATION include/kv2client/colored_depth)
install (FILES ${COLOR_MASKED_USERS_HEADERS} DESTINATION include/kv2client/color_masked_users)
install (FILES ${UTILS_HEADERS} DESTINATION include/kv2client/utils)
