
# SPDX-License-Identifier: MIT
# Copyright (c) 2025 Jayantkumar56


# GLFW Options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)


add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/Glad)

target_link_libraries(${CURRENT_PROJ_NAME} PRIVATE glfw Glad)

set_target_properties(Glad PROPERTIES FOLDER "Dependencies")

set_target_properties(glfw PROPERTIES FOLDER "Dependencies/GLFW3")
set_target_properties(uninstall PROPERTIES FOLDER "Dependencies/GLFW3")
set_target_properties(update_mappings PROPERTIES FOLDER "Dependencies/GLFW3")
