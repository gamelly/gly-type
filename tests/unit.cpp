#include <stdint.h>
#include <assert.h>

#include "gly_type_render.h"

#define def_draw_line(T,I) T arr_##T##I[4]; \
    void drawline_##T##I(T a, T b, T c, T d) \
    {arr_##T##I[0]=a;arr_##T##I[1]=b;arr_##T##I[2]=c;arr_##T##I[3]=d;}

def_draw_line(int,_test1);
def_draw_line(int,_test2);
def_draw_line(int,_test3);
def_draw_line(uint8_t,_test1);
def_draw_line(uint8_t,_test2);
def_draw_line(uint8_t,_test3);

int main()
{
    gly_type_render(10, 20, 31, "`", drawline_int_test1);
    assert(arr_int_test1[0] == 10);
    assert(arr_int_test1[1] == 20);
    assert(arr_int_test1[2] == 25);
    assert(arr_int_test1[3] == 35);

    gly_type_render(7, 20, 31, "``", drawline_int_test2);
    assert(arr_int_test2[0] == 40);
    assert(arr_int_test2[1] == 20);
    assert(arr_int_test2[2] == 55);
    assert(arr_int_test2[3] == 35);

    gly_type_render(17, 30, 31, "```", 2, drawline_int_test3);
    assert(arr_int_test3[0] == 50);
    assert(arr_int_test3[1] == 30);
    assert(arr_int_test3[2] == 65);
    assert(arr_int_test3[3] == 45);

    gly_type_render<uint8_t>(40, 40, 31, "`", drawline_uint8_t_test1);
    assert(arr_uint8_t_test1[0] == 40);
    assert(arr_uint8_t_test1[1] == 40);
    assert(arr_uint8_t_test1[2] == 55);
    assert(arr_uint8_t_test1[3] == 55);

    gly_type_render<uint8_t>(27, 50, 31, "``", drawline_uint8_t_test2);
    assert(arr_uint8_t_test2[0] == 60);
    assert(arr_uint8_t_test2[1] == 50);
    assert(arr_uint8_t_test2[2] == 75);
    assert(arr_uint8_t_test2[3] == 65);

    gly_type_render<uint8_t>(37, 60, 31, "```", 2, drawline_uint8_t_test3);
    assert(arr_uint8_t_test3[0] == 70);
    assert(arr_uint8_t_test3[1] == 60);
    assert(arr_uint8_t_test3[2] == 85);
    assert(arr_uint8_t_test3[3] == 75);

    return 0;
}
