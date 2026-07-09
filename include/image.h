#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>

#define MAX_GALLERY_IMAGES 64
#define IMAGE_PATH "data/"

typedef struct {
    SDL_Texture *texture;
    char filename[256];
    int w;
    int h;
    int valid;
} GalleryImage;

bool image_init(void);
void image_cleanup(void);
int  image_scan(SDL_Renderer *renderer, GalleryImage *images, int max);
void image_destroy(GalleryImage *img);

#endif