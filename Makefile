ARCH=host
BIN=opk/oddity
PKG=oddity.opk
OBJ= \
    src/backend.o \
    src/box.o \
    src/db.o \
    src/helper.o \
    src/file.o \
    src/list.o \
    src/main.o \
    src/pool.o \
    src/session.o \
    src/text.o \
    src/view.o \
    src/widget.o \
    src/views/app.o \
    src/views/applist.o \
    src/views/exit.o \
    src/views/front.o \
    src/views/repolist.o \
    src/views/settings.o \
    src/views/settings_audio.o \
    src/views/settings_date.o \
    src/views/settings_datetime.o \
    src/views/settings_system.o \
    src/views/settings_time.o \
    src/views/settings_timezone.o \
    src/views/sync.o \
    src/views/sysinfo.o \

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

run: $(PKG)
	(cd opk && ./oddity)

upload:
	scp $(PKG) od@10.1.1.2:/media/data/apps/$(PKG)
