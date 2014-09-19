set (HEADERS
     "${PROJECT_BINARY_DIR}/tool/kv2client-config.h"
    )

# add the binary tree to the search path for include files
# so that we will find nexd-config.h
include_directories("${PROJECT_BINARY_DIR}")


# Add executable 
add_executable (kv2client-version
                kv2client-version.cxx)

install (TARGETS kv2client-version DESTINATION bin)
