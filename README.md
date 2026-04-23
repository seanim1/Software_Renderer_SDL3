# SDL3 Hello Triangle - Multi-Platform

Cross-platform triangle rendering with software rasterization.

**Tested:** Linux✅, macOS(M1+), Windows, iOS

## Linux / macOS (M1+)

### Setup (run once)
```bash
chmod +x setup.sh
./setup.sh
```

### Build (every time you modify code)
```bash
cmake --build build
cmake --build build --target 00_init_SDL3
cmake --build build --target 01_window
```

### Run
```bash
./build/Part_00_init_SDL3/00_init_SDL3
./build/Part_01_window/01_window
```

### Clean
```bash
rm -rf build
```

---

