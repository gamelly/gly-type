/**
 * @file gly_type_render.h
 * @short gly_type_render.h
 * @brief a ultra lightweight font renderer and font
 * @image html font.png
 * @date 2024
 * @author RodrigoDornelles
 *
 * @version @b 0.1 0-9A-Z 8 bits / 9 segments @b 2024-11-10
 * @version @b 0.2 full ascii 16 bits / 20 segments @b 2025-01-19
 *
 * @todo publish as .ttf font in npm registry
 *
 * @todo add new segments rules for special characters
 *
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
 * @par 16-bit 20 segments
 * The renderer stores glyphs like a 7-segment display, using 1 bit per line,
 * making storage highly compact. Additional tricks allow more lines than bits
 * and can be visually explained in the documentation.
 *
 * @code
 * | segments_1     | segments_2_r1  | segments_2_r2  | segments_2_r3  |
 * | AAAAAAABBBBBBB | .............. | DDD........EEE | ....DDDEEE.... |
 * | AAAAAAABBBBBBB | .............. | .DDD......EEE. | ...DDD..EEE... |
 * | HHH........CCC | .............. | ..DDD....EEE.. | ..DDD....EEE.. |
 * | HHH........CCC | .............. | ...DDD..EEE... | .DDD......EEE. |
 * | HHH........CCC | AAAAAAABBBBBBB | ....DDDEEE.... | DDD........EEE |
 * | GGG........DDD | AAAAAAABBBBBBB | ....GGGFFF.... | GGG........FFF |
 * | GGG........DDD | .............. | ...GGG..FFF... | .GGG......FFF. |
 * | GGG........DDD | .............. | ..GGG....FFF.. | ..GGG....FFF.. |
 * | FFFFFFFEEEEEEE | .............. | .GGG......FFF. | ...GGG..FFF... |
 * | FFFFFFFEEEEEEE | .............. | GGG........FFF | ....GGGFFF.... |
 *
 * | segments_2_r4  | segments_2_r5  | segments_2_r6  |
 * | ......CC...... | .............. | ......CC...... |
 * | ......CC...... | .............. | ......CC...... |
 * | ......CC...... | .............. | ......CC...... |
 * | ......CC...... | .............. | ......CC...... |
 * | ......CC...... | ......CC...... | ......CC...... |
 * | ......CC...... | ......CC...... | .............. |
 * | ......CC...... | ......CC...... | .............. |
 * | ......CC...... | ......CC...... | .............. |
 * | ......CC...... | ......CC...... | .............. |
 * @endcode
 *
 * @li @b segements_1 no-tricks!
 * @li @b segements_2
 * - @b rule_1 no-tricks!
 * - @b rule_2 when segment_2 H is false
 * - @b rule_3 when segment_2 H is true
 * - @b rule_4 when segment_2 A or B is false
 * - @b rule_5 when segment_2 A or B is true and segment_1 A or B is false
 * - @b rule_6 when segment_2 A or B is true and segment_1 A or B is true
 */

#ifndef H_GLY_TYPE_RENDER
#define H_GLY_TYPE_RENDER

#if defined(__cplusplus) && (defined(GLY_TYPE_INT) || defined(GLY_TYPE_SAFE))
#error Do not use GLY_TYPE_INT or GLY_TYPE_SAFE in C++
#endif

#if !defined(__cplusplus) && !defined(GLY_TYPE_INT)
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

#if defined(__cplusplus) || defined(DOXYGEN)
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
 *
 * @par Source Code
 *
 * @sourcecode
 */
#if defined(__cplusplus)
template<typename GLY_TYPE_INT>
#endif
void
gly_type_render(GLY_TYPE_INT x,
                GLY_TYPE_INT y,
                unsigned int s,
                const char *t,
#if defined(GLY_TYPE_SAFE)
                signed int len,
#endif
#if !defined(__cplusplus)
                void *f
#else
                void (*const draw_line)(GLY_TYPE_INT,
                                        GLY_TYPE_INT,
                                        GLY_TYPE_INT,
                                        GLY_TYPE_INT)
#endif
) {

#if !defined(__cplusplus)
    void (*const draw_line)(
      GLY_TYPE_INT, GLY_TYPE_INT, GLY_TYPE_INT, GLY_TYPE_INT) = f;
#endif

    static const unsigned char segments_1[] = {
        0x00, 0x00, 0x81, 0xff, 0xbb, 0x11, 0x33, 0x80, 0x12, 0x21, 0x00, 0x00,
        0x00, 0x00, 0x08, 0x00, 0xff, 0x30, 0x76, 0x3e, 0x8c, 0xb9, 0xf9, 0x0f,
        0xff, 0xbf, 0x02, 0x02, 0x00, 0x03, 0x00, 0x83, 0xf7, 0xcf, 0xf9, 0xf3,
        0xe1, 0xf3, 0xc3, 0xfb, 0xcc, 0x33, 0x7c, 0xc0, 0xf0, 0xcf, 0xcc, 0xf9,
        0xc7, 0x8f, 0xc7, 0xbb, 0x03, 0xfc, 0x84, 0xfc, 0x00, 0x84, 0x33, 0xe1,
        0x00, 0x1e, 0x00, 0x30, 0x00, 0x70, 0xf8, 0xf1, 0x7c, 0xe1, 0xc1, 0x00,
        0xc8, 0xc0, 0x60, 0xc0, 0xe0, 0xc9, 0xc1, 0xe1, 0xc1, 0x81, 0xc0, 0xa1,
        0x03, 0xe0, 0x80, 0x84, 0x00, 0x00, 0x33, 0x33, 0x00, 0x33, 0x99
    };

    static const unsigned char segments_2[] = {
        0x00, 0x9f, 0x00, 0xfb, 0x07, 0x50, 0x4a, 0x00, 0xc8, 0xb0, 0x7b, 0x54,
        0xa2, 0x03, 0x02, 0x50, 0x00, 0x8c, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00,
        0x03, 0x03, 0x02, 0xa2, 0xc8, 0x03, 0x48, 0x50, 0x06, 0x03, 0x07, 0x00,
        0xb0, 0x03, 0x03, 0x02, 0x03, 0x04, 0x00, 0x31, 0x00, 0x04, 0x28, 0x90,
        0x03, 0x03, 0x23, 0x03, 0x04, 0x00, 0xe0, 0x04, 0x78, 0x07, 0x50, 0x00,
        0x28, 0x00, 0x98, 0x00, 0x08, 0x0d, 0x03, 0x00, 0x03, 0x01, 0x01, 0xea,
        0x03, 0x00, 0x04, 0x8d, 0x00, 0x94, 0x04, 0x04, 0x05, 0x45, 0x88, 0xc0,
        0x04, 0x04, 0xc4, 0xe4, 0x78, 0x58, 0x50, 0x49, 0x04, 0x32, 0x04
    };

    unsigned char c, m1, m2, segment;
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
        x2 = x1 + (sm1 / 2);
        x3 = x1 + sm1;

        c = *t - 0x20;
        m1 = segments_1[c];
        m2 = segments_2[c];

        segment = 0;
        while (segment < 8) {
            switch (m1 & (1 << segment) ? segment : 8) {
                case 0: draw_line(x1, y1, x2, y1); break;
                case 1: draw_line(x2, y1, x3, y1); break;
                case 2: draw_line(x3, y1, x3, y2); break;
                case 3: draw_line(x3, y2, x3, y3); break;
                case 4: draw_line(x2, y3, x3, y3); break;
                case 5: draw_line(x1, y3, x2, y3); break;
                case 6: draw_line(x1, y2, x1, y3); break;
                case 7: draw_line(x1, y1, x1, y2); break;
            }
            segment++;
        }

        segment = 0;
        while (segment < 7) {
            switch (m2 & (1 << segment) ? segment : 7) {
                case 0: draw_line(x1, y2, x2, y2); break;
                case 1: draw_line(x2, y2, x3, y2); break;
                case 2:
                    m2 & 0x3 ? draw_line(x2, y2, x2, m1 & 0x03 ? y1 : y3)
                             : draw_line(x2, y1, x2, y3);
                    break;
                case 3:
                    m2 & 0x80 ? draw_line(x1, y2, x2, y1)
                              : draw_line(x1, y1, x2, y2);
                    break;
                case 4:
                    m2 & 0x80 ? draw_line(x2, y1, x3, y2)
                              : draw_line(x2, y2, x3, y1);
                    break;
                case 5:
                    m2 & 0x80 ? draw_line(x2, y3, x3, y2)
                              : draw_line(x2, y2, x3, y3);
                    break;
                case 6:
                    m2 & 0x80 ? draw_line(x1, y2, x2, y3)
                              : draw_line(x1, y3, x2, y2);
                    break;
            }
            segment++;
        }

        x1 += sp2;
        t++;
    }
}

#if defined(__cplusplus)
template<typename GLY_TYPE_INT>
void
gly_type_render(GLY_TYPE_INT x,
                GLY_TYPE_INT y,
                unsigned int s,
                const char *t,
                void (*const draw_line)(GLY_TYPE_INT,
                                        GLY_TYPE_INT,
                                        GLY_TYPE_INT,
                                        GLY_TYPE_INT)) {
    gly_type_render(x, y, s, t, -1, draw_line);
}
#endif

#endif
