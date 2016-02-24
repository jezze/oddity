include rules.cross

all: ztore.opk

opk/ztore: ztore.c tpl.c
	$(CC) -o $@ `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_gfx $^

opk/zrepo: zrepo.c tpl.c
	$(CC) -o $@ `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_gfx $^

ztore.opk: opk/ztore opk/zrepo
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore opk/zrepo *.o
