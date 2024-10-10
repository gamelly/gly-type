#include <stdio.h>

int main(int argc, char *argv[]) {
    int i, j;
    char c;
    unsigned int segment = 0x00;

    i = 1;
    while(i < argc) {
        j = 0;
        while(argv[i][j]) {
            c = (argv[i][j] | 0x20) - 'a';
            if (c < 8) {
                segment |= 1 << c;
            }
            j++;
        }
        i++;
    }

    printf("%02x\n", segment);
    return 0;
}
