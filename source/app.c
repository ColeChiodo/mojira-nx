#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <switch.h>

#include <SDL_image.h>

#include "app.h"
#include "image.h"

bool app_init(App *app)
{
    memset(app, 0, sizeof(App));
    app->state = APP_STATE_GALLERY;

    romfsInit();
    chdir("romfs:/");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0)
        return false;

    if (TTF_Init() < 0)
        return false;

    app->window = SDL_CreateWindow(
        "Mojira NX",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);
    if (!app->window)
        return false;

    app->renderer = SDL_CreateRenderer(
        app->window, -1, SDL_RENDERER_ACCELERATED);
    if (!app->renderer)
        return false;

    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    app->font = TTF_OpenFont("data/LeroyLetteringLightBeta01.ttf", 28);
    if (!app->font)
        return false;

    if (!image_init())
        return false;

    app->image_count = image_scan(app->renderer, app->images, MAX_GALLERY_IMAGES);

    return true;
}

void app_cleanup(App *app)
{
    for (int i = 0; i < app->image_count; i++)
        image_destroy(&app->images[i]);

    image_cleanup();

    if (app->font)
        TTF_CloseFont(app->font);

    if (app->renderer)
        SDL_DestroyRenderer(app->renderer);

    if (app->window)
        SDL_DestroyWindow(app->window);

    TTF_Quit();
    SDL_Quit();
    romfsExit();
}

void app_set_status(App *app, const char *message)
{
    strncpy(app->status_message, message, sizeof(app->status_message) - 1);
    app->status_message[sizeof(app->status_message) - 1] = '\0';
    app->status_ticks = 180;
}