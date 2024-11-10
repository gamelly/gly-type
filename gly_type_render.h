/**
 * @file gly_type_render.h
 * @short gly_type_render.h
 * @brief a ultra lightweight font renderer and font
 * @date 2024
 * @version 0.1
 * @author RodrigoDornelles
 * @copyright
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. @n See http://www.wtfpl.net/ for more details.
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
 *
 * @par Source Code
 *
 * @sourcecode
 */

#ifndef H_GLY_TYPE_RENDER
#define H_GLY_TYPE_RENDER

#ifndef GLY_TYPE_INT
/**
 * The @c GLY_TYPE_INT defines the integer type used for coordinates and sizes
 * in @ref gly_type_render.
 *
 * @li @c int but size varies by compiler/architecture. Common in rendering
 * libraries.
 * @li @c uint8_t suitable for small screens or limited coordinate ranges.
 * @li @c uint16_t often ideal for most cases, balancing range and
 * memory efficiency.
 *
 * @par
 * @code
 * #define GLY_TYPE_INT uint16_t  // Recommended for most applications
 * @endcode
 */
#define GLY_TYPE_INT unsigned char
#endif

#ifdef DOXYGEN
/**
 * @details
 * The @c GLY_TYPE_SAFE macro enables a length parameter to ensure safe string
 * handling in the @ref gly_type_render function. When defined, it activates an
 * additional check to limit the number of processed characters, preventing
 * access beyond the string's end.
 */
#define GLY_TYPE_SAFE
#endif

/**
 * gly_type_render
 *
 * The `gly_type_render` function is used to render text at specified
 * coordinates using a line-drawing function. It employs segments to display
 * characters and allows control over the position and size of the rendered
 * characters.
 *
 * @pre If the optional @c len parameter is used,
 * @ref GLY_TYPE_SAFE must be defined with `#define GLY_TYPE_SAFE` prior to
 * including this function.
 *
 * @param [in] x      Horizontal coordinate (in pixels).
 * @param [in] y      Vertical coordinate (in pixels).
 * @param [in] size   Character size in pixels.
 * @param [in] text   Text string to be rendered, terminated with a null
 * character (`\0`).
 * @param [in] fptr   Pointer to a line-drawing function, with the interface:
 *                    `function(x1, y1, x2, y2)`
 * @n draws a line from `(x1, y1)` to `(x2, y2)`.
 *
 * Optional Parameter:
 * @param [in] len    Maximum length of characters to process, applicable only
 * if `GLY_TYPE_SAFE` is defined. If `len` is -1, all characters in the string
 * are processed until the null terminator.
 *
 * Total Size:
 * @li Width:  `length(text) * (size + 1)`
 * @li Height: `min(count(text, 0x0A), 1) * (size + 1)`
 *
 * Example Usage:
 * @code
 * gly_type_render(x, y, size, "hello world", draw_line_func);
 * @endcode
 *
 * This function will render the text "hello world" at the coordinates `(x, y)`,
 * with the specified character size, using `draw_line_func` to draw the lines.
 */
void
gly_type_render(GLY_TYPE_INT x,
                GLY_TYPE_INT y,
                GLY_TYPE_INT s,
                const char *t,
#ifdef GLY_TYPE_SAFE
                signed int len,
#endif
                const void *const f) {
    const void (*const draw_line)(
      GLY_TYPE_INT, GLY_TYPE_INT, GLY_TYPE_INT, GLY_TYPE_INT) = f;

    static const unsigned char number_segments[] = { 0xbf, 0x07, 0x5b, 0x4f,
                                                     0x66, 0xec, 0xfc, 0x87,
                                                     0xff, 0xef };

    static const unsigned char alpha_segments[] = {
        0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x7d, 0x76, 0x89,
        0x1e, 0x30, 0x38, 0xb7, 0x37, 0x3f, 0x73, 0x67, 0xf3,
        0x6d, 0x81, 0x3e, 0xa2, 0xbe, 0x80, 0xe2, 0x88
    };

    unsigned char c;
    GLY_TYPE_INT sp2, sm1, x1, x2, x3, y1, y2, y3;

    sp2 = s + 2;
    sm1 = s - 1;
    x1 = x;
    y1 = y;
    y2 = y1 + (sm1 / 2);
    y3 = y1 + sm1;

    while (*t) {
#ifdef GLY_TYPE_SAFE
        if (len != -1 && len-- <= 0) {
            break;
        }
#endif
        c = (*t | 0x20) - 'a';
        x2 = x1 + (sm1 / 2);
        x3 = x1 + sm1;
        if ('0' <= *t && *t <= '9') {
            unsigned char m = number_segments[*t - '0'];
            unsigned char segment = 0;
            while (segment < 8) {
                switch (m & (1 << segment) ? segment : 8) {
                    case 0: draw_line(x2, y1, x3, y1); break;
                    case 1: draw_line(x3, y1, x3, y2); break;
                    case 2: draw_line(x3, y2, x3, y3); break;
                    case 3: draw_line(x1, y3, x3, y3); break;
                    case 4: draw_line(x1, y2, x1, y3); break;
                    case 5: draw_line(x1, y1, x1, y2); break;
                    case 6: draw_line(x1, y2, x3, y2); break;
                    case 7: draw_line(x1, y1, x2, y1); break;
                }
                segment++;
            }
        }
        if (c == ('k' - 'a')) {
            draw_line(x2, y2, x3, y1);
            draw_line(x2, y2, x3, y3);
        }
        if (c <= ('z' - 'a')) {
            unsigned char m = alpha_segments[c];
            unsigned char segment = 0;
            while (segment < 8) {
                switch (m & (1 << segment) ? segment : 8) {
                    case 0: draw_line(x1, y1, x3, y1); break;
                    case 1: draw_line(x3, y1, x3, y2); break;
                    case 2: draw_line(x3, y2, x3, y3); break;
                    case 3: draw_line(x1, y3, x3, y3); break;
                    case 4: draw_line(x1, y2, x1, y3); break;
                    case 5: draw_line(x1, y1, x1, y2); break;
                    case 6: draw_line(x1, y2, x3, y2); break;
                    case 7: {
                        if (m & (1 << 6)) {
                            draw_line(x2, y2, x2, y3);
                        } else if (m & 1 || m & (1 << 4)) {
                            draw_line(x2, y1, x2, y3);
                        } else {
                            switch ((c + 'a' - 'v' + 1) >> 1) {
                                case 0:
                                    draw_line(x1, y2, x2, y3);
                                    draw_line(x2, y3, x3, y2);
                                    break;
                                case 1:
                                    draw_line(x1, y1, x3, y3);
                                    draw_line(x1, y3, x3, y1);
                                    break;
                                case 2:
                                    draw_line(x1, y1, x3, y1);
                                    draw_line(x1, y3, x3, y1);
                                    break;
                            }
                        }
                    }
                    case 8: break;
                }
                segment++;
            }
        }
        x1 += sp2;
        t++;
    }
}

#endif
