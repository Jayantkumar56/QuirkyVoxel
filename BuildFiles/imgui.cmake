
add_library("imgui" STATIC)

target_sources("imgui" PRIVATE 
    "Vendor/imgui/imconfig.h"
    "Vendor/imgui/imgui.h"
    "Vendor/imgui/imgui.cpp"
    "Vendor/imgui/imgui_draw.cpp"
    "Vendor/imgui/imgui_internal.h"
    "Vendor/imgui/imgui_tables.cpp"
    "Vendor/imgui/imgui_widgets.cpp"
    "Vendor/imgui/imstb_rectpack.h"
    "Vendor/imgui/imstb_textedit.h"
    "Vendor/imgui/imstb_truetype.h"
    "Vendor/imgui/imgui_demo.cpp"
    "Vendor/imgui/misc/cpp/imgui_stdlib.h"
    "Vendor/imgui/misc/cpp/imgui_stdlib.cpp"
    # Backend source files
    "Vendor/imgui/backends/imgui_impl_glfw.cpp"
    "Vendor/imgui/backends/imgui_impl_opengl3.cpp"
)

target_include_directories("imgui" PUBLIC "Vendor/imgui" "Vendor/imgui/backends" "Vendor/glfw/include")

target_compile_definitions("imgui" PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD=1)

add_compiler_flags_for("imgui")
enable_release_optimizations_for("imgui")
