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
 * thus spending only 1 bit for each line, which is the trick of being so compact.
 *
 * The @b 8 bit/segment it is a special line, its size varies,
 * from bottom to top it stops when it hits the first fixed segment @c G or @c A .
 *
 * There are special characters, which would be @b x and @b v
 * to differentiate them from all other characters,
 * they have their own internal rendering function,
 * are flagged as segment only @c H (special).
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
 * @param [in] fptr clojure to drawn line with interface: @n `function(x1, y1, x2, y2)`
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
void gly_type_render(unsigned char x, unsigned char y, unsigned char s, const char *const t, const void *const f)
{
    const void (*const gly_tr_draw_line)(unsigned char, unsigned char, unsigned char, unsigned char) = f;
    
    gly_tr_draw_line(0, 0, 4, 0);
    gly_tr_draw_line(0, 0, 0, 4);
    gly_tr_draw_line(4, 0, 4, 4);
    gly_tr_draw_line(0, 2, 4, 2);
    gly_tr_draw_line(4, 0, 4, 4);
}

#endif
