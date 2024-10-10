#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gly_type_render.h"

unsigned int size_by_line;
unsigned char size = 5;
static char* text = "a";
static char* screen = NULL;

void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (y1 < size && x1 < size_by_line) {
            screen[y1 * size_by_line + x1] = '@';
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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        text = argv[1];
    }
    if (argc > 2) {
        size = strtol(argv[2], NULL, 10);
    }

    unsigned int length = strlen(text);
    unsigned int skip = length > 1 ? 2 : 0;
    size_by_line = length * (skip + size); 
    unsigned int size_total = size_by_line * size;

    screen = malloc(size_total);
    if (!screen) {
        perror("Failed to allocate memory");
        return 1;
    }
    memset(screen, '.', size_total);

    gly_type_render(0, 0, size, text, draw_line);
    
    for (unsigned char i = 0; i < size; i++) {
        printf("%.*s\n", size_by_line - skip, &screen[i * size_by_line]);
    }

    free(screen);
    return 0;
}
