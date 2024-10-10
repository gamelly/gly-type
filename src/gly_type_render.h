/**
 * @file src/gly_type_render.h
 * @short GlyType Renderer
 * @brief a ultra lightweight font renderer and font
 * @date 2024
 * @version 0.0.7
 * @author RodrigoDornelles
 * @copyright
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 *
 * @par Style
 * The font style is inspired by digital displays, but not outdated,
 * with its straight lines and some diagonals, a modernized look,
 * and also a nice differentiation between numbers and characters.
 *
 * @par 7 Segments
 * The renderer stores the glyphs as if it were a 7-segment display,
 * thus spending only 1 bit for each line,
 * which is the trick of being so compact.
 *
 * The @b 8 bit/segment it is a special line, its size varies,
 * from bottom to top it stops when
 * it hits the first fixed segment @c G or @c A
 * .
 *
 * There are special characters, which would be @b x , @b v and @b z
 * to differentiate them from all other characters,
 * they have their own internal rendering function,
 * are flagged as segment @c H without segment @c G or @c A
 * .
 *
 * @code
 * FAAAAAAAAAAAAB
 * FFAAAAAAAAABBB
 * FFF........BBB
 * FFF........BBB
 * FFGGGGGGGGGGBB
 * EEGGGGGGGGGGCC
 * EEE........CCC
 * EEE........CCC
 * EEDDDDDDDDDDCC
 * EDDDDDDDDDDDDC
 * @endcode
 */

#ifndef H_GLY_TYPE_RENDER
#define H_GLY_TYPE_RENDER

/**
 * @param [in] x in pixels
 * @param [in] y in pixels
 * @param [in] size in pixels
 * @param [in] text with null char terminator
 * @param [in] fptr clojure to drawn line with interface:
 * @n `function(x1, y1, x2, y2)`
 *
 * @par Total Size
 * @li @b width `length(text) * (size+1)`
 * @li @b height `min(count(text, 0x0A), 1) * (size+1)`
 *
 * @par Example
 * @code
 * gly_type_render(x, y, size, "hello world", draw_line_func);
 * @endcode
 */
void
gly_type_render(unsigned char x,
                unsigned char y,
                unsigned char s,
                const char *t,
                const void *const f) {
    const void (*const draw_line)(
      unsigned char, unsigned char, unsigned char, unsigned char) = f;

    static const unsigned char alpha_segments[] = {
        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x7d, 0x76, 0x89,
        0x1e, 0xf2, 0x38, 0xb7, 0x37, 0x3f, 0x73, 0x67, 0xf3,
        0x6d, 0x81, 0x3e, 0xa2, 0xbe, 0x80, 0xe2, 0x88
    };
    char c = '\0';

    while (*t) {
        c = (*t | 0x20) - 'a';
        if (c <= ('z' - 'a')) {
            unsigned char m = alpha_segments[c];
            unsigned char segment = 0;
            while (segment < 8) {
                switch (m & (1 << segment) ? segment : 9) {
                    case 0: draw_line(0, 0, 4, 0); break;
                    case 1: draw_line(4, 0, 4, 2); break;
                    case 2: draw_line(4, 2, 4, 4); break;
                    case 3: draw_line(0, 4, 4, 4); break;
                    case 4: draw_line(0, 2, 0, 4); break;
                    case 5: draw_line(0, 0, 0, 2); break;
                    case 6: draw_line(0, 2, 4, 2); break;
                    case 7: {
                        if (m & (1 << 6)) {
                            draw_line(2, 2, 2, 4);
                        } else if (m & 1) {
                            draw_line(2, 0, 2, 4);
                        } else {
                            switch ((*t - 'v' + 1) >> 1) {
                                case 0:
                                    draw_line(0, 2, 2, 4);
                                    draw_line(2, 4, 4, 2);
                                    break;
                                case 1:
                                    draw_line(0, 0, 4, 4);
                                    draw_line(0, 4, 4, 0);
                                    break;
                                case 2:
                                    draw_line(0, 0, 4, 0);
                                    draw_line(0, 4, 4, 0);
                                    break;
                            }
                        }
                    }
                }
                segment++;
            }
        }
        t++;
    }
}

#endif
