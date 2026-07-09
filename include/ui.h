#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>

#define CARD_W 240
#define CARD_H 160
#define CARD_PADDING 24
#define CARD_GAP 28
#define GRID_COLS 4
#define GRID_ROWS 3

#define COLOR_BG       0x10, 0x10, 0x14
#define COLOR_PANEL    0x1C, 0x1C, 0x24
#define COLOR_ACCENT   0xE6, 0x00, 0x12
#define COLOR_WHITE    0xFF, 0xFF, 0xFF
#define COLOR_GRAY     0xAA, 0xAA, 0xAA
#define COLOR_SHADOW   0x00, 0x00, 0x00
#define COLOR_SELECTED 0xFF, 0xFF, 0xFF

SDL_Texture *ui_render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect);
void ui_draw_rounded_rect(SDL_Renderer *renderer, int x, int y, int w, int h, int r, SDL_Color color);
void ui_draw_card(SDL_Renderer *renderer, int x, int y, int w, int h, int selected, float scale, SDL_Texture *image);
void ui_draw_panel(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color);
void ui_draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *label, const char *key, int x, int y, int selected);
void ui_draw_title(SDL_Renderer *renderer, TTF_Font *font, const char *title, int y);
void ui_draw_status_bar(SDL_Renderer *renderer, TTF_Font *font, const char *message, int y);

#endif