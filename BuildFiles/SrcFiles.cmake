
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


function(RegisterSourceFiles FILE_LIST_VAR PREFIX)
    if(DEFINED ${FILE_LIST_VAR})
        set(RESULT_LIST "${${FILE_LIST_VAR}}")
    else()
        set(RESULT_LIST "")
    endif()

    foreach(FILE_PATH ${ARGN})
        list(APPEND RESULT_LIST "${PREFIX}/${FILE_PATH}")
    endforeach()

    set(${FILE_LIST_VAR} "${RESULT_LIST}" PARENT_SCOPE)
endfunction(RegisterSourceFiles)


set(BASE_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Src")

# Source files in Core/Application
RegisterSourceFiles(SOURCE_FILES "${BASE_SOURCE_DIRECTORY}/Application"
    "Application.h"
    "Application.cpp"
    "GameLayer.h"
    "Layer.h"
    "main.cpp"
    "Window.h"
    "Window.cpp"
)


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCE_FILES})
