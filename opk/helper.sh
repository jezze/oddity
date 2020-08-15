#!/bin/sh

run_volume_get() {
    amixer sget PCM | awk -F"[][]" '/dB/ { print $2 }' | head -n 1
    exit 0
}

run_volume_increment() {
    amixer -q set PCM 10%+
    run_volume_get
    exit 0
}

run_volume_decrement() {
    amixer -q set PCM 10%-
    run_volume_get
    exit 0
}

run_$1
