ARCH=host
BIN=opk/ztore
PKG=ztore.opk
OBJ= \
    src/area.o \
    src/ztore.o \
    src/file.o \
    src/db.o \
    src/box.o \
    src/text.o \
    src/list.o \
    src/menu.o \
    src/backend.o \
    src/session.o \
    src/view.o \
    src/view_front.o \
    src/view_repolist.o \
    src/view_applist.o \
    src/view_app.o \
    src/view_settings.o \
    src/view_sync.o \
    src/view_install.o \
    src/view_uninstall.o \

include ${ARCH}.mk

all: $(PKG)

%.o: %.c
	@echo CC $@
	@$(CC) -c -o $@ -Wall -Werror -pedantic -std=gnu99 `${SDLCONFIG} --cflags` $^

$(BIN): $(OBJ)
	@echo LD $@
	@$(LD) -o $@ $^ `${SDLCONFIG} --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lsqlite3

$(PKG): $(BIN)
	mksquashfs opk $@

clean:
	rm -f $(PKG) $(BIN) $(OBJ)

upload:
	scp $(PKG) od@10.1.1.2:/media/data/apps/$(PKG)
