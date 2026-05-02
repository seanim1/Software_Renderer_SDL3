/*
 * Part 02 - Vulkan Instance Creation
 *
 * This tutorial demonstrates the most basic Vulkan setup:
 * Creating a VkInstance, the entry point to Vulkan.
 *
 * A VkInstance is required before you can:
 * - Enumerate physical devices (GPUs)
 * - Create surfaces (window connections)
 * - Load device functions
 *
 * Topics covered:
 * - VkApplicationInfo
 * - VkInstanceCreateInfo
 * - Platform-specific extensions (Win32, Xlib, Metal)
 * - VkInstance lifecycle (create/destroy)
 * - Basic error handling
 */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vulkan/vulkan.h>
#ifdef __APPLE__
    #define VK_ENABLE_BETA_EXTENSIONS
    #include <vulkan/vulkan_metal.h>
    #include <vulkan/vulkan_beta.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * Helper function to check Vulkan result codes
 * ============================================================================ */

void check_vk_result(VkResult result, const char* operation) {
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Vulkan error during %s: %d\n", operation, result);
        exit(1);
    }
}

/* ============================================================================
 * Vulkan Instance Creation
 * ============================================================================
 *
 * The instance is the connection between your application and the Vulkan
 * loader. It allows you to:
 * 1. Enable global extensions (extensions needed at instance level)
 * 2. Enable global validation layers (for debugging)
 * 3. Query physical devices
 *
 * ============================================================================ */

VkInstance create_vulkan_instance() {
    printf("Creating Vulkan instance...\n");

    /* ───────────────────────────────────────────────────────────────────
     * Step 1: Define your application
     *
     * VkApplicationInfo tells the driver about your app.
     * This is metadata that the driver can use for optimization or debugging.
     * ─────────────────────────────────────────────────────────────────── */

    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Part 02 - Vulkan Instance",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 2),  /* My app version */
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3,  /* Target Vulkan 1.3 */
    };

    /* ───────────────────────────────────────────────────────────────────
     * Step 2: Specify extensions
     *
     * Extensions are optional features we want to use. At the instance level,
     * we typically need:
     * - VK_KHR_SURFACE: Required for window surfaces
     * - Platform-specific surface extension (Win32, Xlib, Metal, etc.)
     *
     * Extensions enable features like:
     * - Display access (VK_KHR_DISPLAY)
     * - Debug output (VK_EXT_DEBUG_UTILS)
     * - Validation layers (VK_LAYER_KHRONOS_VALIDATION)
     * ─────────────────────────────────────────────────────────────────── */

    /* We need VK_KHR_SURFACE for window rendering on all platforms */
    const char* required_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        /* Platform-specific surface extension */
#ifdef VK_USE_PLATFORM_WIN32_KHR
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
        VK_EXT_METAL_SURFACE_EXTENSION_NAME,
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#endif
    };

    uint32_t required_extension_count =
        sizeof(required_extensions) / sizeof(required_extensions[0]);

    printf("Required extensions:\n");
    for (uint32_t i = 0; i < required_extension_count; i++) {
        printf("  - %s\n", required_extensions[i]);
    }

    /* ───────────────────────────────────────────────────────────────────
     * Step 3: Check extension availability
     *
     * It's good practice to query available extensions before creating
     * the instance, to ensure they exist on this system.
     * ─────────────────────────────────────────────────────────────────── */

    uint32_t available_extension_count = 0;
    vkEnumerateInstanceExtensionProperties(
        NULL,  /* No layer filter */
        &available_extension_count,
        NULL   /* Query count only */
    );

    printf("\nAvailable instance extensions: %u\n", available_extension_count);

    VkExtensionProperties* available_extensions =
        (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * available_extension_count);

    vkEnumerateInstanceExtensionProperties(
        NULL,
        &available_extension_count,
        available_extensions
    );

    for (uint32_t i = 0; i < available_extension_count; i++) {
        printf("  - %s (version %u)\n",
               available_extensions[i].extensionName,
               available_extensions[i].specVersion);
    }

    free(available_extensions);

    /* Query available validation layers (informational only) */
    uint32_t available_layer_count = 0;
    vkEnumerateInstanceLayerProperties(&available_layer_count, NULL);

    VkLayerProperties* available_layers =
        (VkLayerProperties*)malloc(sizeof(VkLayerProperties) * available_layer_count);

    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers);

    printf("\nAvailable instance layers: %u\n", available_layer_count);
    for (uint32_t i = 0; i < available_layer_count; i++) {
        printf("  - %s (version %u, spec %u)\n",
               available_layers[i].layerName,
               available_layers[i].implementationVersion,
               available_layers[i].specVersion);
    }

    free(available_layers);

    /* ───────────────────────────────────────────────────────────────────
     * Step 4: Validation layers (optional)
     *
     * Layers are optional features for debugging and validation.
     * We'll skip them for now since they're not critical for learning.
     * ─────────────────────────────────────────────────────────────────── */

    uint32_t layer_count = 0;
    const char** enabled_layers = NULL;

    /* ───────────────────────────────────────────────────────────────────
     * Step 5: Create the instance
     *
     * VkInstanceCreateInfo bundles all the configuration.
     * This is where we specify:
     * - The app info
     * - Required extensions
     * - Validation layers to enable
     * ─────────────────────────────────────────────────────────────────── */

    VkInstanceCreateInfo instance_create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
#ifdef VK_USE_PLATFORM_METAL_EXT
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
#else
        .flags = 0,
#endif
        .pApplicationInfo = &app_info,
        .enabledLayerCount = layer_count,
        .ppEnabledLayerNames = enabled_layers,
        .enabledExtensionCount = required_extension_count,
        .ppEnabledExtensionNames = required_extensions,
    };

    VkInstance instance;
    VkResult result = vkCreateInstance(&instance_create_info, NULL, &instance);
    check_vk_result(result, "vkCreateInstance");

    printf("✓ Vulkan instance created successfully\n");

    return instance;
}

/* ============================================================================
 * Query and display device information
 * ============================================================================
 *
 * Once we have an instance, we can enumerate physical devices (GPUs)
 * and query their properties.
 * ============================================================================ */

void enumerate_physical_devices(VkInstance instance) {
    printf("\n=== Physical Devices (GPUs) ===\n");

    /* Get device count */
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);

    if (device_count == 0) {
        printf("No Vulkan-compatible devices found!\n");
        return;
    }

    printf("Found %u device(s)\n\n", device_count);

    /* Get device handles */
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    /* Query properties of each device */
    for (uint32_t i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(devices[i], &properties);

        printf("Device %u: %s\n", i, properties.deviceName);
        printf("  Type: ");
        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                printf("Discrete GPU\n");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                printf("Integrated GPU\n");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                printf("Virtual GPU\n");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                printf("Software Renderer\n");
                break;
            default:
                printf("Unknown\n");
        }

        printf("  Vulkan Version: %u.%u.%u\n",
               VK_API_VERSION_MAJOR(properties.apiVersion),
               VK_API_VERSION_MINOR(properties.apiVersion),
               VK_API_VERSION_PATCH(properties.apiVersion));

        printf("  Driver Version: %u\n", properties.driverVersion);
        printf("  Vendor ID: 0x%x\n", properties.vendorID);

        /* Query memory properties */
        VkPhysicalDeviceMemoryProperties memory_properties;
        vkGetPhysicalDeviceMemoryProperties(devices[i], &memory_properties);

        printf("  Memory Heaps: %u\n", memory_properties.memoryHeapCount);
        for (uint32_t j = 0; j < memory_properties.memoryHeapCount; j++) {
            VkMemoryHeap heap = memory_properties.memoryHeaps[j];
            float size_gb = (float)heap.size / (1024.0f * 1024.0f * 1024.0f);
            printf("    Heap %u: %.2f GB ", j, size_gb);

            if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                printf("(DEVICE_LOCAL)");
            }
            printf("\n");
        }

        printf("\n");
    }

    free(devices);
}

/* ============================================================================
 * Main program
 * ============================================================================ */

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    printf("=== Part 02: Vulkan Instance Creation ===\n\n");

    /* Initialize SDL3 */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize SDL3: %s\n", SDL_GetError());
        return 1;
    }

    /* Create a window (we won't use it yet, but it's good to have) */
    SDL_Window* window = SDL_CreateWindow(
        "Part 02 - Vulkan Instance",
        800, 600,
        SDL_WINDOW_VULKAN
    );

    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("SDL3 window created: 800x600\n\n");

    /* Create Vulkan instance */
    printf("\n=== Creating Vulkan Instance ===\n");
    VkInstance instance = create_vulkan_instance();

    /* Enumerate and display physical devices */
    printf("\n=== Querying Physical Devices ===\n");
    enumerate_physical_devices(instance);

    /* Game loop */
    printf("\n=== Running (close window to exit) ===\n\n");
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        /* Present frame (no rendering yet, just show black window) */
        SDL_Delay(16);  /* ~60 FPS */
    }

    /* Cleanup */
    printf("Destroying Vulkan instance...\n");
    vkDestroyInstance(instance, NULL);
    printf("✓ Instance destroyed\n");

    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Program completed successfully!\n");
    return 0;
}
