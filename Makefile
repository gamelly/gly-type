all: segments ascii
	@

ascii: tools/ascii.c gly_type_render.h
	$(CC) -I. -o $@ tools/ascii.c

segments: tools/segments.c gly_type_render.h
	$(CC) -I. -o $@ tools/segments.c

doxfilter: tools/doxfilter.cpp
	$(CXX) -I. -o $@ $< -static

bmp: tools/bmp.cpp gly_type_render.h
	$(CXX) -I. -o $@ tools/bmp.cpp

font.bmp: bmp
	./bmp

font.png: font.bmp
	convert font.bmp -scale 800% font.png 

test_interface: tests/interface.cpp gly_type_render.h
	$(CXX) -std=c++98 -Wall -Werror -I. -o $@ tests/interface.cpp -static

test_legible: tests/legible.cpp gly_type_render.h
	$(CXX) -std=c++98 -Wall -Werror -I. -o $@ tests/legible.cpp -static

test_unit: tests/unit.c gly_type_render.h
	$(CC) -std=c89 -Wall -Werror -I. -o $@ tests/unit.c -static

tests: test_interface test_legible test_unit
	./test_unit && ./test_interface && ./test_legible

clear:
	rm -f ascii segments bmp a.out doxfilter test_* font.*
