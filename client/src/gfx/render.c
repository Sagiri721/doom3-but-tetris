/**
 * @file        render.h
 * @brief       Game rendering implementation
 */

#include "../sokol_gp/thirdparty/sokol_gfx.h"
#include "../sokol_gp/sokol_gp.h"
#include "../sokol_gp/thirdparty/sokol_app.h"
#include "../sokol_gp/thirdparty/sokol_glue.h"
#include "../core/tetris.h"

#define CLEAR_COLOUR_R 0.1f
#define CLEAR_COLOUR_G 0.1f
#define CLEAR_COLOUR_B 0.1f

#define CELL_SIZE 32

int width, height;
float ratio; // Aspect ratio

#define NORMALIZE_X(x) ((x) * (width / 2.0f))
#define NORMALIZE_Y(y) ((y) * (height / 2.0f))

void render_begin() {
    // Get current window size.
    width = sapp_width(), height = sapp_height();

    // Begin recording draw commands for a frame buffer of size (width, height).
    sgp_begin(width, height);
    // Set frame buffer drawing region to (0,0,width,height).
    sgp_viewport(0, 0, width, height);
    // Set drawing coordinate space to top-left origin with pixel units.
    sgp_project(0, width, 0, height); // origin at top-left

    // Clear the frame buffer.
    sgp_set_color(CLEAR_COLOUR_R, CLEAR_COLOUR_G, CLEAR_COLOUR_B, 1.0f);
    sgp_clear();
}

void render_end() {
    // Finish a draw command queue, clearing it.
    sgp_end();
    // End render pass.
    sg_end_pass();
    // Commit Sokol render.
    sg_commit();
}

void render_game(tetris_board *game) {
    
    // Draw field background
    float cell_px = CELL_SIZE;
    float board_width = game->rows * cell_px;
    float board_height = game->cols * cell_px;

    float board_x = (width - board_width) / 2.0f;
    float board_y = (height - board_height) / 2.0f;

    sgp_set_color(0.6f, 0.6f, 0.6f, 1.0f);
    sgp_draw_filled_rect(board_x, board_y, board_width, board_height);

    for (int x = 0; x < game->rows; x++) {
        for (int y = 0; y < game->cols; y++) {
            int cell;
            if ((cell = index_cell(game, x, y)) != 0) {
                // Draw filled cell
                float cell_x = board_x + x * cell_px;
                float cell_y = board_y + y * cell_px;

                // Set color based on cell value
                float r = (cell * 37 % 255) / 255.0f;
                float g = (cell * 91 % 255) / 255.0f;
                float b = (cell * 53 % 255) / 255.0f;
                sgp_set_color(r, g, b, 1.0f);
                sgp_draw_filled_rect(cell_x, cell_y, cell_px - 1, cell_px - 1);
            }
        }
    }

    // Begin a render pass.
    sg_pass pass = {.swapchain = sglue_swapchain()};
    sg_begin_pass(&pass);
    // Dispatch all draw commands to Sokol GFX.
    sgp_flush();
}