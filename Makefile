all: segments ascii
	@

doxfilter: tools/doxfilter.cpp
	$(CXX) -I. -o $@ $< -static

ascii: tools/ascii.c gly_type_render.h
	$(CC) -I. -o $@ tools/ascii.c

bmp: tools/bmp.c gly_type_render.h
	$(CC) -I. -o $@ tools/bmp.c

font.bmp: bmp
	./bmp

segments: tools/segments.c gly_type_render.h
	$(CC) -I. -o $@ tools/segments.c

clear:
	rm ascii segments
