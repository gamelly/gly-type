all: segments ascii
	@

ascii: tools/ascii.c gly_type_render.h
	$(CC) -std=c89 -I. -o $@ tools/ascii.c

segments: tools/segments.c gly_type_render.h
	$(CC) -std=c89 -I. -o $@ tools/segments.c

doxfilter: tools/doxfilter.cpp
	$(CXX) -std=c++98 -I. -o $@ $< -static

bmp: tools/bmp.cpp gly_type_render.h
	$(CXX) -std=c++98 -I. -o $@ tools/bmp.cpp

font.bmp: bmp
	./bmp

clear:
	rm -f ascii segments font.bmp bmp
