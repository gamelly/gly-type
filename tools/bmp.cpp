#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gly_type_render.h"

#define WIDTH 64
#define HEIGHT 96
#define ROW_SIZE ((WIDTH + 7) / 8)
#define PADDED_ROW_SIZE ((ROW_SIZE + 3) & ~3)
#define DATA_SIZE (PADDED_ROW_SIZE * HEIGHT)

typedef struct __attribute__((packed)) {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct __attribute__((packed)) {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;

const BMPFileHeader fileHeader = {
    .bfType = 0x4D42,
    .bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 8 + DATA_SIZE,
    .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 8
};

const BMPInfoHeader infoHeader = {
    .biSize = sizeof(BMPInfoHeader),
    .biWidth = WIDTH,
    .biHeight = -HEIGHT,
    .biPlanes = 1,
    .biBitCount = 1,
    .biCompression = 0,
    .biSizeImage = DATA_SIZE,
    .biXPelsPerMeter = 2835,
    .biYPelsPerMeter = 2835,
    .biClrUsed = 2,
    .biClrImportant = 2
};

const uint8_t padding[3] = {
    0x00, 0x00, 0x00
};

const uint8_t palette[8] = {
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00
};

static uint8_t bitmap[HEIGHT * WIDTH / 8];

void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (y1 < HEIGHT && x1 < WIDTH) {
            int byte_index = (y1 * WIDTH + x1) / 8;
            int bit_index = (y1 * WIDTH + x1) % 8;
            bitmap[byte_index] |= (1 << (7 - bit_index));
        }

        if (x1 == x2 && y1 == y2) break;

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

int main() {
    FILE *file = fopen("font.bmp", "wb");

    if (!file) {
        return 1;
    }

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, file);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, file);
    fwrite(palette, sizeof(uint8_t), sizeof(palette), file);

    for (char i = 0x20; i < 0x7e; i++) {
        uint8_t x = (i - 0x20) % (WIDTH/8) * 8;
        uint8_t y = (i - 0x20) / (WIDTH/8) * 8;
        gly_type_render(x, y, 7, &i, 1, draw_line);
    }
   
    for (int y = 0; y < HEIGHT; ++y) {
        fwrite(&bitmap[y * ROW_SIZE], ROW_SIZE, 1, file);
        fwrite(padding, PADDED_ROW_SIZE - ROW_SIZE, 1, file);
    }

    fclose(file);
    return 0;
}
