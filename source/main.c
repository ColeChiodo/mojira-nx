#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <switch.h>

#include "app.h"
#include "ui.h"

#define GRID_ORIGIN_X 60
#define GRID_ORIGIN_Y 100
#define TOTAL_ITEMS (GRID_COLS * GRID_ROWS)

int main(int argc, char **argv)
{
    App app;
    if (!app_init(&app))
        return 1;

    TTF_Font *font_title = app.font;
    TTF_Font *font_body  = app.font;
    TTF_Font *font_small = app.font;

    int focus = 0;
    int focus_timer = 0;

    char image_count_text[64];
    snprintf(image_count_text, sizeof(image_count_text),
             "%d image%s", app.image_count, app.image_count == 1 ? "" : "s");

    SDL_Event event;

    while (!app.exit_requested && appletMainLoop())
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                app.exit_requested = 1;

            if (event.type == SDL_JOYBUTTONDOWN)
            {
                int col = focus % GRID_COLS;
                int row = focus / GRID_COLS;

                switch (event.jbutton.button)
                {
                case JOY_PLUS:
                    app.exit_requested = 1;
                    break;

                case JOY_UP:
                    if (row > 0) focus -= GRID_COLS;
                    break;

                case JOY_DOWN:
                    if (row < GRID_ROWS - 1) focus += GRID_COLS;
                    break;

                case JOY_LEFT:
                    if (col > 0) focus--;
                    break;

                case JOY_RIGHT:
                    if (col < GRID_COLS - 1) focus++;
                    break;

                case JOY_A:
                    if (focus < app.image_count && app.images[focus].valid)
                        app_set_status(&app, app.images[focus].filename);
                    break;

                case JOY_B:
                    focus = 0;
                    break;

                default:
                    break;
                }

                focus_timer = 0;
            }
        }

        focus_timer++;

        if (app.status_ticks > 0)
            app.status_ticks--;
        if (app.status_ticks == 0)
            app.status_message[0] = '\0';

        SDL_SetRenderDrawColor(app.renderer, 0x10, 0x10, 0x14, 0xFF);
        SDL_RenderClear(app.renderer);

        ui_draw_title(app.renderer, font_title, "Mojira Gallery", 30);

        {
            SDL_Color gray = { 0xAA, 0xAA, 0xAA, 0xFF };
            SDL_Rect tr = { 0, 0, 0, 0 };
            SDL_Texture *tex = ui_render_text(app.renderer, font_small, image_count_text, gray, &tr);
            if (tex) {
                tr.x = 40;
                tr.y = 65;
                SDL_RenderCopy(app.renderer, tex, NULL, &tr);
                SDL_DestroyTexture(tex);
            }
        }

        int draw_count = app.image_count < TOTAL_ITEMS ? app.image_count : TOTAL_ITEMS;

        for (int i = 0; i < draw_count; i++)
        {
            int col = i % GRID_COLS;
            int row = i / GRID_COLS;
            int x = GRID_ORIGIN_X + col * (CARD_W + CARD_GAP);
            int y = GRID_ORIGIN_Y + row * (CARD_H + CARD_GAP);
            int selected = (i == focus);

            float scale = selected ? 1.0f + 0.06f * sinf(focus_timer * 0.08f) : 1.0f;

            SDL_Texture *tex = app.images[i].valid ? app.images[i].texture : NULL;
            ui_draw_card(app.renderer, x, y, CARD_W, CARD_H, selected, scale, tex);
        }

        ui_draw_button(app.renderer, font_body, "Upload", "A", 40, 630, 0);
        ui_draw_button(app.renderer, font_body, "Refresh", "X", 340, 630, 0);
        ui_draw_button(app.renderer, font_body, "Exit", "+", 640, 630, 0);

        ui_draw_status_bar(app.renderer, font_small, app.status_message, 680);

        SDL_RenderPresent(app.renderer);
        SDL_Delay(16);
    }

    app_cleanup(&app);
    return 0;
}