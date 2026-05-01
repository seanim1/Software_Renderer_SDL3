# Part 02 - Vulkan Instance Creation

## What You'll Learn

In this tutorial, you'll create your first **VkInstance**, which is:
- The **entry point** to the Vulkan API
- The **global context** for your Vulkan application
- Required before you can enumerate devices, create surfaces, or load functions

## Why VkInstance?

Before Vulkan can do anything, it needs to know:
1. What your application is (for optimization/debugging)
2. What extensions you want to use globally
3. What validation layers you want enabled

All of this goes into a VkInstance.

## Key Concepts

### 1. VkApplicationInfo
Metadata about your application:
```cpp
VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "My App",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_3,  // Target Vulkan 1.3
};
```

### 2. Extensions
Optional features you need. For rendering to a window:
```cpp
const char* extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,        // Required on all platforms
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,  // Windows only
    // OR
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME,   // Linux only
    // OR
    VK_EXT_METAL_SURFACE_EXTENSION_NAME,  // macOS/iOS only
};
```

### 3. Validation Layers
Optional debugging helpers. Standard layer is:
```cpp
VK_LAYER_KHRONOS_validation  // Catches API misuse
```

Validation layers add overhead, so only enable during development.

### 4. VkInstanceCreateInfo
Bundles everything together:
```cpp
VkInstanceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &app_info,
    .enabledExtensionCount = 2,
    .ppEnabledExtensionNames = extensions,
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = validation_layers,
};
```

### 5. Create and Destroy
```cpp
VkInstance instance;
vkCreateInstance(&info, NULL, &instance);
// ... use instance ...
vkDestroyInstance(instance, NULL);
```

## What This Tutorial Does

1. **Creates an SDL3 window** (needed for surfaces later)
2. **Queries available extensions** (shows what your driver supports)
3. **Queries available validation layers** (shows debugging options)
4. **Creates a VkInstance** with required extensions
5. **Enumerates physical devices** (GPUs)
6. **Displays GPU properties**:
   - Name and type (discrete, integrated, software)
   - Vulkan version support
   - Memory heaps and sizes
7. **Cleans up** (destroys instance, window, SDL)

## Running the Tutorial

### Linux/macOS (after running setup.sh)
```bash
cmake --build build --target 02_vk_instance
./build/Part_02_VkInstance/02_vk_instance
```

### Windows (after running setup.ps1)
```powershell
cmake --build build --target 02_vk_instance
.\build\Part_02_VkInstance\Release\02_vk_instance.exe
```

### iOS (after running setup-ios.sh)
```bash
open build_ios/hello_triangle.xcodeproj
# Select 02_vk_instance target
# Build and run
```

## Expected Output

You should see something like:

```
=== Part 02: Vulkan Instance Creation ===

SDL3 window created: 800x600

Creating Vulkan instance...
Required extensions:
  - VK_KHR_SURFACE
  - VK_KHR_XLIB_SURFACE

Available instance extensions: 15
  - VK_KHR_SURFACE (version 25)
  - VK_KHR_XLIB_SURFACE (version 6)
  - ... more extensions ...

Available instance layers: 1
  - VK_LAYER_KHRONOS_validation (version ...)

✓ Vulkan instance created successfully

=== Physical Devices (GPUs) ===
Found 1 device(s)

Device 0: NVIDIA GeForce RTX 4090
  Type: Discrete GPU
  Vulkan Version: 1.3.280
  Driver Version: 560000000
  Vendor ID: 0x10de
  Memory Heaps: 2
    Heap 0: 24.00 GB (DEVICE_LOCAL)
    Heap 1: 62.78 GB

Destroying Vulkan instance...
✓ Instance destroyed

Program completed successfully!
```

## Common Issues

### "No Vulkan-compatible devices found"
- Vulkan drivers not installed
- GPU doesn't support Vulkan (very old hardware)
- Solution: Install GPU drivers, or use software renderer

### Extension not available
- Wrong extension name
- Driver doesn't support that platform extension
- Solution: Only request extensions from the available list

### Validation layer not found
- VK_LAYER_KHRONOS_validation not installed
- Solution: Install Vulkan SDK from lunarg.com
- Or: Remove validation layers (not critical for first steps)

## What's Next (Part 03)

Once you have a VkInstance, you can:
- Create a VkSurfaceKHR (window surface)
- Pick a VkPhysicalDevice (GPU)
- Create a VkDevice (logical device for command submission)
- Create swap chains and render to the window

## Vulkan API Calls Used

| Function | Purpose |
| --- | --- |
| `vkCreateInstance()` | Create global Vulkan context |
| `vkDestroyInstance()` | Clean up instance |
| `vkEnumerateInstanceExtensionProperties()` | Query available extensions |
| `vkEnumerateInstanceLayerProperties()` | Query available validation layers |
| `vkEnumeratePhysicalDevices()` | Get list of GPUs |
| `vkGetPhysicalDeviceProperties()` | Get GPU name, type, version |
| `vkGetPhysicalDeviceMemoryProperties()` | Get GPU memory info |

## Code Organization

The tutorial code is structured in phases:

1. **Helper function** (`check_vk_result`) - Error checking
2. **Instance creation** (`create_vulkan_instance`) - 5 steps:
   - Define app info
   - Specify extensions
   - Check extension availability
   - Specify validation layers
   - Create instance
3. **Device enumeration** (`enumerate_physical_devices`) - Query GPUs
4. **Main program** - Initialize SDL, create instance, cleanup

Each section has detailed comments explaining the "why" not just the "how".

## Deep Dive: Why Extensions?

Extensions in Vulkan are how the API stays stable. Instead of changing core Vulkan with each release:
- New features are added as extensions
- Your code explicitly requests them
- You can gracefully fall back if unavailable
- Drivers can optimize based on what you use

Example: Rendering to a window requires platform-specific code:
- **Windows**: `VK_KHR_WIN32_SURFACE` talks to HWND
- **Linux**: `VK_KHR_XLIB_SURFACE` talks to X11 windows
- **macOS/iOS**: `VK_EXT_METAL_SURFACE` talks to Metal

But they all present the same abstract `VkSurfaceKHR` interface.

## Further Reading

- [Vulkan Spec: VkInstance](https://www.khronos.org/registry/vulkan/specs/1.3/html/vkspec.html#VkInstance)
- [Vulkan Spec: Extensions](https://www.khronos.org/registry/vulkan/specs/1.3/html/vkspec.html#extendingvulkan-extensions)
- [Vulkan Layers](https://vulkan.lunarg.com/doc/view/latest/linux/layer_configuration.html)