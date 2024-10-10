ascii:
	${CC} tools/ascii.c -Isrc -o build/ascii

segments:
	${CC} tools/segments.c -Isrc -o build/segments
