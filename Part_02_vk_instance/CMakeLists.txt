add_executable(02_vk_instance
    main.cpp
)

target_link_libraries(02_vk_instance PRIVATE
    sdl3_deps
    vulkan_deps
)