SDLCONFIG=sdl-config

include rules.cross

all: ztore.opk

opk/ztore: src/ztore.c src/view.c src/file.c src/db.c src/box.c src/text.c src/menu.c src/backend.c src/view_front.c src/view_browse.c src/view_showapp.c src/view_category.c
	$(CC) -o $@ -Wall -Werror -pedantic -std=c89 `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lsqlite3 $^

ztore.opk: opk/ztore
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore src/*.o
