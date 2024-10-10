all: segments ascii
	@

ascii: tools/ascii.c gly_type_render.h
	$(CC) -I. -o $@ tools/ascii.c

segments: tools/segments.c gly_type_render.h
	$(CC) -I. -o $@ tools/segments.c

clear:
	rm ascii segments
