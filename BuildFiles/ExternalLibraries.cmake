
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


# yaml-cpp Options
set(YAML_CPP_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL       OFF CACHE BOOL "" FORCE)


# GLFW Options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)


add_subdirectory( Vendor/glfw          )
add_subdirectory( Vendor/Glad          )
add_subdirectory( Vendor/glm           )
add_subdirectory( Vendor/stb           )
add_subdirectory( Vendor/yaml-cpp      )
add_subdirectory( Vendor/FastNoiseLite )
add_subdirectory( Vendor/spdlog        )
add_subdirectory( Vendor/QkTraits      )

include("BuildFiles/imgui.cmake")
include("BuildFiles/tracy.cmake")

target_link_libraries(${CURRENT_PROJ_NAME} PRIVATE 
    glfw 
    Glad
    glm
    imgui
    stb
    yaml-cpp
    FastNoiseLite
    spdlog
    TracyClient
    QkTraits
)

set_target_properties(
    Glad 
    glm
    imgui
    stb
    yaml-cpp
    FastNoiseLite
    spdlog
    TracyClient
    QkTraits
    PROPERTIES FOLDER "Dependencies"
)

set_target_properties(
    glfw 
    uninstall 
    update_mappings
    PROPERTIES FOLDER "Dependencies/GLFW3"
)

if (MSVC)
    # Remove any previous /W flags and suppress all warnings
    target_compile_options(yaml-cpp PRIVATE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/w>")
    # Also force it to not get /W3 from target_compile_options inside yaml-cpp
    set_target_properties(yaml-cpp PROPERTIES COMPILE_OPTIONS "")
else()
    target_compile_options(yaml-cpp PRIVATE -w)
endif()
