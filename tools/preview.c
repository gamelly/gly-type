#include <stdio.h>
#include <string.h>
#include "../src/gly_type_render.h"

#define SIZE 5
char screen[SIZE*SIZE];

void draw_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    while(x1 < x2 || y1 < y2) {
        screen[(y1*SIZE) + x1] = '@';
        if (y1 < y2) {
            y1++;
        }
        if (x1 < x2) {
            x1++;
        }
        screen[(y1*SIZE) + x1] = '@';
    }
}

void preview(char l)
{
    static char buf[] = "@";
    
    buf[0] = l;
    
    memset(screen, '.', sizeof(screen));
    
    gly_type_render(0, 0, SIZE, "A", draw_line);
    
    for(unsigned char i = 0; i < SIZE; i++) {
        printf("%.*s\n", SIZE, &screen[i * SIZE]);
    }
}

int main(int argc, char *argv[])
{
    preview(argc <= 1? 'a': argv[1][0]);
    return 0;
}
