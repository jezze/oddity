#!/bin/sh

cmd_datetime_get() {
    date --rfc-3339=second
}

cmd_volume_get() {
    amixer sget $1 | awk -F"[][]" '/dB/ { print $2 }' | head -n 1

    exit 0
}

cmd_volume_increment() {
    amixer -q set $1 1+
    cmd_volume_get $1
}

cmd_volume_decrement() {
    amixer -q set $1 1-
    cmd_volume_get $1
}

cmd_download() {
    wget -q --show-progress --progress=dot -o /dev/stdout $1 -O $2 2>/dev/null
}

cmd_autostart_get() {
    test -e ~/.autostart && echo "on" || echo "off"
}

cmd_autostart_install() {
    local opk="/media/data/apps/ztore.opk"

    test -e $opk && echo -e "#!/bin/sh\n\nopkrun $opk" > ~/.autostart && chmod +x ~/.autostart

    cmd_autostart_get
}

cmd_autostart_uninstall() {
    rm -f ~/.autostart

    cmd_autostart_get
}

if [ $# -gt 0 ]
then
    cmd_$@
fi
