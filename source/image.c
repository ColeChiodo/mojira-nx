#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <SDL.h>
#include <SDL_image.h>

#include "image.h"

bool image_init(void)
{
    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(flags) & flags) != flags)
        return false;
    return true;
}

void image_cleanup(void)
{
    IMG_Quit();
}

static int load_texture(SDL_Renderer *renderer, GalleryImage *img, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
        return 0;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (!tex) {
        SDL_FreeSurface(surface);
        return 0;
    }

    img->texture = tex;
    img->w = surface->w;
    img->h = surface->h;
    img->valid = 1;

    SDL_FreeSurface(surface);
    return 1;
}

int image_scan(SDL_Renderer *renderer, GalleryImage *images, int max)
{
    int count = 0;

    const char *exts[] = { ".png", ".jpg", ".jpeg", ".webp" };
    int num_exts = 4;

    DIR *dir = opendir(IMAGE_PATH);
    if (!dir)
        return 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && count < max)
    {
        if (entry->d_type == DT_DIR)
            continue;

        const char *name = entry->d_name;
        int len = strlen(name);
        int matched = 0;

        for (int e = 0; e < num_exts; e++)
        {
            int elen = strlen(exts[e]);
            if (len >= elen && strcasecmp(name + len - elen, exts[e]) == 0)
            {
                matched = 1;
                break;
            }
        }

        if (!matched)
            continue;

        char fullpath[512];
        snprintf(fullpath, sizeof(fullpath), "%s%s", IMAGE_PATH, name);

        memset(&images[count], 0, sizeof(GalleryImage));
        snprintf(images[count].filename, sizeof(images[count].filename), "%s", name);

        if (!load_texture(renderer, &images[count], fullpath))
            images[count].valid = 0;

        count++;
    }

    closedir(dir);
    return count;
}

void image_destroy(GalleryImage *img)
{
    if (img->texture) {
        SDL_DestroyTexture(img->texture);
        img->texture = NULL;
    }
    img->valid = 0;
}