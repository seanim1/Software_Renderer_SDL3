# ── iOS Only ──────────────────────────────────────────────────────────

cmake_minimum_required(VERSION 3.22)
project(SDL3HelloTriangle VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL3/build")
find_package(SDL3 REQUIRED CONFIG)

execute_process(COMMAND brew --prefix vulkan-headers OUTPUT_VARIABLE VULKAN_SDK OUTPUT_STRIP_TRAILING_WHITESPACE)
set(Vulkan_INCLUDE_DIR "${VULKAN_SDK}/include")
find_package(Vulkan QUIET)

add_library(sdl3_deps INTERFACE)
target_link_libraries(sdl3_deps INTERFACE SDL3::SDL3)

add_library(vulkan_deps INTERFACE)
target_include_directories(vulkan_deps INTERFACE ${Vulkan_INCLUDE_DIRS})

target_compile_definitions(vulkan_deps INTERFACE 
    VK_USE_PLATFORM_METAL_EXT
    VK_ENABLE_BETA_EXTENSIONS
)
target_link_libraries(vulkan_deps INTERFACE
    "-framework CoreFoundation"
    "-framework Metal"
    "-framework IOSurface"
    "-framework CoreGraphics"
    "-framework Foundation"
    "-framework QuartzCore"
    "-framework UIKit"
)

add_subdirectory(Part_00_init_SDL3)
add_subdirectory(Part_01_window)
add_subdirectory(Part_02_vk_instance)

set_target_properties(00_init_SDL3 PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "init-sdl3"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.00-init-sdl3"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.example.00-init-sdl3"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_INFOPLIST_KEY_UIApplicationSupportsIndirectInputEvents YES
)
set_target_properties(01_window PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "window"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.01-window"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.example.01-window"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_INFOPLIST_KEY_UIApplicationSupportsIndirectInputEvents YES
)
set_target_properties(02_vk_instance PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "vk-instance"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.02-vk-instance"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.example.02-vk-instance"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_INFOPLIST_KEY_UIApplicationSupportsIndirectInputEvents YES
)