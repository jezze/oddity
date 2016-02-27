SDLCONFIG=sdl-config

include rules.cross

all: ztore.opk

opk/ztore: ztore.c db.c menu.c render.c view_front.c view_apps.c view_browse.c
	$(CC) -o $@ `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_gfx -lsqlite3 $^

ztore.opk: opk/ztore
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore *.o
