
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


set(TRACY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Vendor/tracy")


add_library(TracyClient STATIC "${TRACY_DIR}/public/TracyClient.cpp")

target_include_directories(TracyClient SYSTEM PUBLIC "${TRACY_DIR}/public")

target_compile_definitions(TracyClient PUBLIC 
    $<$<CONFIG:Debug>:TRACY_ENABLE>
    $<$<CONFIG:RelWithDebInfo>:TRACY_ENABLE>

    # This ensures tracy don't use ram and cpu when it's not connected.
    TRACY_ON_DEMAND
)


# System Links (Required by Tracy)
if(WIN32)
    target_link_libraries(TracyClient PUBLIC ws2_32 dbghelp)
elseif(UNIX)
    target_link_libraries(TracyClient PUBLIC pthread dl)
endif()


if(MSVC)
    target_compile_options(TracyClient PRIVATE "/w")
endif()
