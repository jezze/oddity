ARCH=host
BIN=opk/ztore
PKG=ztore.opk
OBJ= \
    src/backend.o \
    src/box.o \
    src/db.o \
    src/download.o \
    src/file.o \
    src/list.o \
    src/main.o \
    src/selection.o \
    src/session.o \
    src/text.o \
    src/view.o \
    src/view_front.o \
    src/view_repolist.o \
    src/view_applist.o \
    src/view_app.o \
    src/view_settings.o \
    src/view_sync.o \
    src/widget.o \

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
