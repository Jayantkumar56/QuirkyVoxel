
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


# GLFW Options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)


add_subdirectory( Vendor/glfw )
add_subdirectory( Vendor/Glad )
add_subdirectory( Vendor/glm  )

include("BuildFiles/imgui.cmake")

target_link_libraries(${CURRENT_PROJ_NAME} PRIVATE 
    glfw 
    Glad
    glm
    imgui
)

set_target_properties(
    Glad 
    glm
    imgui
    PROPERTIES FOLDER "Dependencies"
)

set_target_properties(
    glfw 
    uninstall 
    update_mappings
    PROPERTIES FOLDER "Dependencies/GLFW3"
)
