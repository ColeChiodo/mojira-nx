#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "image.h"

#define SCREEN_W 1280
#define SCREEN_H 720

#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  10
#define JOY_MINUS 11
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

typedef enum {
    APP_STATE_GALLERY,
    APP_STATE_UPLOAD,
    APP_STATE_SETTINGS,
    APP_STATE_STATUS,
    APP_STATE_EXIT
} AppState;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    AppState state;
    int exit_requested;
    char status_message[256];
    int status_ticks;
    GalleryImage images[MAX_GALLERY_IMAGES];
    int image_count;
} App;

bool app_init(App *app);
void app_cleanup(App *app);
void app_set_status(App *app, const char *message);

#endif