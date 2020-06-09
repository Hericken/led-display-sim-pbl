#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "led-matrix-c.h"
#include "mnb_bmp.h"

int main(int argc, char const **argv)
{
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    char rgb_sequence[] = "GBR";

    memset(&options, 0, sizeof(options));
    options.rows = options.cols = 64;
    options.led_rgb_sequence = rgb_sequence;
    matrix = led_matrix_create_from_options(&options, &argc, (char***)&argv);
    if (matrix == NULL) return 1;
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);

    pthread_t recthread;
    if(pthread_create(&recthread, NULL, (void*)takeTimelapse, NULL)) {
        return -1;
    }

    int i, j, k, width, height;
    led_canvas_get_size(offscreen_canvas, &width, &height);
    for(k = 0; k < 100; ++k) {
        led_canvas_clear(offscreen_canvas);
        for(i = 0; i < height; ++i) {
            for(j = 0; j < width; ++j) {
                // led_canvas_set_pixel(offscreen_canvas, j, i, j*4, (((i&15)^(j&15))>>3&1)*255, 0);
                led_canvas_set_pixel(offscreen_canvas, j, i, 0, ((k%2)^(((i&15)^(j&15))>>3&1))*255, 0);
            }
        }
        led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        usleep(100000);
    }
    return 0;
}
