#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    printf("SDL Version: %d\n", SDL_GetVersion());
    printf("Platform: %s\n", SDL_GetPlatform());

    // Query video driver
    const char* video_driver = SDL_GetCurrentVideoDriver();
    printf("Video Driver: %s\n", video_driver ? video_driver : "None");

    // Query displays
    int num_displays = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&num_displays);
    printf("Displays: %d\n", num_displays);

    if (displays) {
        for (int i = 0; i < num_displays; i++) {
            const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displays[i]);
            if (mode) {
                printf("  Display %d: %dx%d @ %.0f Hz\n", 
                    i, mode->w, mode->h, mode->refresh_rate);
            }
        }
        SDL_free(displays);
    }

    SDL_Quit();
    printf("\n✅ SDL3 initialized successfully!\n");
    return 0;
}