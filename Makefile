SDLCONFIG=sdl-config

include rules.cross

all: ztore.opk

opk/ztore: ztore.c view.c db.c box.c text.c menu.c render.c view_front.c view_apps.c view_browse.c view_showapp.c
	$(CC) -o $@ -Wall -Werror -pedantic -std=c89 `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lsqlite3 $^

ztore.opk: opk/ztore
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore *.o
