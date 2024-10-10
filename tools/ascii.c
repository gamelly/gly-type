#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gly_type_render.h"

#define SIZE 5
char screen[SIZE*SIZE];

void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        screen[(y1 * SIZE) + x1] = '@';

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


void preview(char l)
{
    static char buf[] = "@";
    
    buf[0] = l;
    
    memset(screen, '.', sizeof(screen));
    
    gly_type_render(0, 0, SIZE, buf, draw_line);
    
    for(unsigned char i = 0; i < SIZE; i++) {
        printf("%.*s\n", SIZE, &screen[i * SIZE]);
    }
}

int main(int argc, char *argv[])
{
    preview(argc <= 1? 'a': argv[1][0]);
    return 0;
}
