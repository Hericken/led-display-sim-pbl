#include "led-matrix-c.h"
#include "mnb_bmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct RGBLedMatrix {};

struct RGBLedMatrix *led_matrix_create_from_options(
        struct RGBLedMatrixOptions *options,
        int *argc,
        char ***argv)
{
    int i;
    char s[4];

    for(i = 1; i < *argc; i++) {
        if(!strncmp((*argv)[i], "--led-rows=", strlen("--led-rows="))) {
            if(strlen((*argv)[i]) >= 16) {
                fprintf(stderr, "set default size 64 as too large rows\n");
            }
            strncpy(s, (*argv)[i]+11, strlen((*argv)[i])-11);
            options->rows = atoi(s);
        } else if(!strncmp((*argv)[i], "--led-cols=", strlen("--led-cols="))) {
            if(strlen((*argv)[i]) >= 16) {
                fprintf(stderr, "set default size 64 as too large cols\n");
            }
            strncpy(s, (*argv)[i]+11, strlen((*argv)[i])-11);
            options->cols = atoi(s);
        }
    }
    setPanelSize(options->rows, options->cols);
    initBmpImg();
    struct RGBLedMatrix *matrix = (struct RGBLedMatrix*)malloc(sizeof(struct RGBLedMatrix));
    return matrix;
}


struct RGBLedMatrix *led_matrix_create_from_options_const_argv(
        struct RGBLedMatrixOptions *options,
        int argc,
        char ***argv)
{
    return led_matrix_create_from_options(options, &argc, argv);
}


struct RGBLedMatrix *led_matrix_create(int rows, int chained, int parallel)
{
    setPanelSize(rows, 64);
    initBmpImg();
    struct RGBLedMatrix *matrix = (struct RGBLedMatrix*)malloc(sizeof(struct RGBLedMatrix));
    return matrix;
}


void led_matrix_delete(struct RGBLedMatrix *matrix)
{
    return;
}


struct LedCanvas *led_matrix_get_canvas(struct RGBLedMatrix *matrix)
{
    struct LedCanvas *c;
    return c;
}


void led_canvas_get_size(const struct LedCanvas *canvas, int *width, int *height)
{
    *width = _panel_cols;
    *height = _panel_rows;
}

void led_canvas_set_pixel(struct LedCanvas *c, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= _panel_cols || y < 0 || y >= _panel_rows) return;

    img_canvas[y][x].r = r;
    img_canvas[y][x].g = g;
    img_canvas[y][x].b = b;
}


void led_canvas_clear(struct LedCanvas *c)
{
    led_canvas_fill(c, 0, 0, 0);
}


void led_canvas_fill(struct LedCanvas *c, uint8_t r, uint8_t g, uint8_t b)
{
    int i, j;

    for (i = 0; i < _panel_rows; i++) {
        for (j = 0; j < _panel_cols; j++) {
            led_canvas_set_pixel(c, j, i, r, g, b);
        }
    }
}


struct LedCanvas *led_matrix_create_offscreen_canvas(struct RGBLedMatrix *matrix)
{
    struct LedCanvas *c;
    return c;
}


struct LedCanvas* led_matrix_swap_on_vsync(struct RGBLedMatrix *matrix, struct LedCanvas *canvas)
{
    int i, j, k, l, xx, yy;

    for(i = 0; i < _panel_rows; i++) {
        for(j = 0; j < _panel_cols; j++) {
            for (k = 0; k < PIXEL_WIDTH; k++) {
                for (l = 0; l < PIXEL_WIDTH; l++) {
                    //hard coding PIXEL_WIDTH is 5
                    if((!k && !l)||(!k && l==4)||(k==4 && !l)||(k==4 && l==4)) continue;

                    xx = k + (LINE_WIDTH + PIXEL_WIDTH) * j + LINE_WIDTH + SCALE_LENGTH;
                    yy = l + (LINE_WIDTH + PIXEL_WIDTH) * i + LINE_WIDTH + SCALE_LENGTH;
                    bmp_img->data[yy][xx].r = img_canvas[i][j].r;
                    bmp_img->data[yy][xx].g = img_canvas[i][j].g;
                    bmp_img->data[yy][xx].b = img_canvas[i][j].b;
                }
            }
        }
    }
    return canvas;
}
