SDLCONFIG=sdl-config

include rules.cross

all: ztore.opk

opk/ztore: src/ztore.c src/file.c src/db.c src/box.c src/text.c src/menu.c src/backend.c src/session.c src/view.c src/view_front.c src/view_repolist.c src/view_applist.c src/view_app.c src/view_sync.c src/view_install.c src/view_uninstall.c
	$(CC) -o $@ -Wall -Werror -pedantic -std=c89 $^ `$(SDLCONFIG) --cflags --libs` -lSDL_image -lSDL_ttf -lsqlite3

ztore.opk: opk/ztore
	mksquashfs opk $@

clean:
	rm -f ztore.opk opk/ztore src/*.o
