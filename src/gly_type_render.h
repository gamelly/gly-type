#ifndef H_GLY_TYPE_RENDER
#define H_GLY_TYPE_RENDER

void gly_type_render(unsigned char x, unsigned char y, unsigned char s, const char *const t, void* f)
{
    const void (*const gly_tr_draw_line)(unsigned char, unsigned char, unsigned char, unsigned char) = f;
    
    gly_tr_draw_line(0, 0, 4, 0);
    gly_tr_draw_line(0, 0, 0, 4);
    gly_tr_draw_line(4, 0, 4, 4);
    gly_tr_draw_line(0, 2, 4, 2);
    gly_tr_draw_line(4, 0, 4, 4);
}

#endif
