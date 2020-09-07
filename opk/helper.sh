#!/bin/sh

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

    exit 0
}

cmd_autorun_get() {
    test -e ~/.autorun && echo "on" || echo "off"

    exit 0
}

cmd_autorun_install() {
    local opk="/media/data/apps/ztore.opk"

    test -e $opk && echo -e "#!/bin/sh\n\nopkrun $opk" > ~/.autorun

    cmd_autorun_get
}

cmd_autorun_uninstall() {
    rm -f ~/.autorun

    cmd_autorun_get
}

if [ $# -gt 0 ]
then
    cmd_$@
fi
