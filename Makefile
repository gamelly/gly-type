all: segments ascii
	@

ascii: tools/ascii.c gly_type_render.h
	$(CC) -I. -o $@ tools/ascii.c

segments: tools/segments.c gly_type_render.h
	$(CC) -I. -o $@ tools/segments.c

doxfilter: tools/doxfilter.cpp
	$(CXX) -I. -o $@ $< -static

bmp: tools/bmp.cpp gly_type_render.h
	$(CXX) -std=c++20 -I. -o $@ tools/bmp.cpp

svg: tools/svg.cpp gly_type_render.h
	$(CXX) -std=c++20 -I. -o $@ tools/svg.cpp

font.svg: svg
	./svg

font.bmp: bmp
	./bmp

font.png: font.bmp
	convert font.bmp -scale 400% font.png 

test_unit_cpp: tests/unit.cpp gly_type_render.h
	$(CXX) -std=c++98 -Wall -Werror -I. -o $@ tests/unit.cpp -static

test_unit_c: tests/unit.c gly_type_render.h
	$(CC) -std=c89 -Wall -Werror -I. -o $@ tests/unit.c -static

tests: test_unit_c test_unit_cpp
	./test_unit_c && ./test_unit_cpp

ocrtest: bmp
	./bmp 40x8 test_legible.bmp "A B C"
	convert test_legible.bmp -bordercolor white -border 8x8 -scale 400% test_legible.png
	tesseract test_legible.png stdout --psm 7 | grep ABC

clear:
	rm -f ascii segments bmp a.out doxfilter test_* font.*
