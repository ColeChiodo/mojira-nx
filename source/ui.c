#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ui.h"
#include "app.h"

SDL_Texture *ui_render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface)
        return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        rect->w = surface->w;
        rect->h = surface->h;
    }

    SDL_FreeSurface(surface);
    return texture;
}

static void draw_filled_circle(SDL_Renderer *renderer, int cx, int cy, int r, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dy = -r; dy <= r; dy++)
        for (int dx = -r; dx <= r; dx++)
            if (dx * dx + dy * dy <= r * r)
                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
}

void ui_draw_rounded_rect(SDL_Renderer *renderer, int x, int y, int w, int h, int r, SDL_Color color)
{
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_Rect body = { x + r, y, w - 2 * r, h };
    SDL_RenderFillRect(renderer, &body);

    SDL_Rect side_left  = { x, y + r, r, h - 2 * r };
    SDL_Rect side_right = { x + w - r, y + r, r, h - 2 * r };
    SDL_RenderFillRect(renderer, &side_left);
    SDL_RenderFillRect(renderer, &side_right);

    draw_filled_circle(renderer, x + r,     y + r,     r, color);
    draw_filled_circle(renderer, x + w - r, y + r,     r, color);
    draw_filled_circle(renderer, x + r,     y + h - r, r, color);
    draw_filled_circle(renderer, x + w - r, y + h - r, r, color);
}

void ui_draw_card(SDL_Renderer *renderer, int x, int y, int w, int h, int selected, float scale, SDL_Texture *image)
{
    int sw = (int)(w * scale);
    int sh = (int)(h * scale);
    int sx = x + (w - sw) / 2;
    int sy = y + (h - sh) / 2;

    SDL_Color shadow = { 0, 0, 0, 80 };
    if (selected) {
        ui_draw_rounded_rect(renderer, sx + 4, sy + 6, sw, sh, 12, shadow);
    }

    SDL_Color card_color = { 0x1C, 0x1C, 0x24, 0xFF };
    ui_draw_rounded_rect(renderer, sx, sy, sw, sh, 12, card_color);

    if (image) {
        SDL_Rect dst = { sx + 8, sy + 8, sw - 16, sh - 16 };
        SDL_RenderCopy(renderer, image, NULL, &dst);
    }

    if (selected) {
        SDL_Color border = { 0xE6, 0x00, 0x12, 0xFF };
        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        for (int i = 0; i < 3; i++)
            SDL_RenderDrawRect(renderer, &(SDL_Rect){ sx - i, sy - i, sw + 2 * i, sh + 2 * i });
    }
}

void ui_draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, const char *key, int x, int y, int selected)
{
    SDL_Color bg = selected ? (SDL_Color){ 0xE6, 0x00, 0x12, 0xFF }
                            : (SDL_Color){ 0x1C, 0x1C, 0x24, 0xFF };
    SDL_Color fg = { 0xFF, 0xFF, 0xFF, 0xFF };

    char buf[64];
    snprintf(buf, sizeof(buf), "%s  %s", key, label);
    SDL_Rect tr = { 0, 0, 0, 0 };
    SDL_Texture *tex = ui_render_text(renderer, font, buf, fg, &tr);
    if (!tex) return;

    int pad = 16, rx = x, ry = y, rw = tr.w + pad * 2, rh = tr.h + 12;
    ui_draw_rounded_rect(renderer, rx, ry, rw, rh, 8, bg);

    tr.x = rx + pad;
    tr.y = ry + 6;
    SDL_RenderCopy(renderer, tex, NULL, &tr);
    SDL_DestroyTexture(tex);
}

void ui_draw_title(SDL_Renderer *renderer, TTF_Font *font, const char *title, int y)
{
    SDL_Color color = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Rect rect = { 0, 0, 0, 0 };
    SDL_Texture *tex = ui_render_text(renderer, font, title, color, &rect);
    if (tex) {
        rect.x = 40;
        rect.y = y;
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        SDL_DestroyTexture(tex);
    }
}

void ui_draw_panel(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color)
{
    ui_draw_rounded_rect(renderer, x, y, w, h, 12, color);
}

void ui_draw_status_bar(SDL_Renderer *renderer, TTF_Font *font, const char *message, int y)
{
    if (!message || !message[0])
        return;

    SDL_Color color = { 0xAA, 0xAA, 0xAA, 0xFF };
    SDL_Rect rect = { 0, 0, 0, 0 };
    SDL_Texture *tex = ui_render_text(renderer, font, message, color, &rect);
    if (tex) {
        rect.x = 40;
        rect.y = y;
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        SDL_DestroyTexture(tex);
    }
}