#!/bin/sh

run_volume_get() {
    amixer sget $1 | awk -F"[][]" '/dB/ { print $2 }' | head -n 1
    exit 0
}

run_volume_increment() {
    amixer -q set $1 1+
    run_volume_get $1
    exit 0
}

run_volume_decrement() {
    amixer -q set $1 1-
    run_volume_get $1
    exit 0
}

run_download() {
    wget -q --show-progress --progress=dot -o /dev/stdout $1 -O $2 2>/dev/null
    exit 0
}

if [ $# -gt 0 ]
then
    run_$@
fi
