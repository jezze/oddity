include rules.cross

all: ztore.opk

opk/ztore: main.c
	$(CC) -o $@ `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf $^

ztore.opk: opk/ztore
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore *.o
