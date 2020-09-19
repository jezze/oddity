#!/bin/sh

cmd_sysinfo_get() {
    . /etc/os-release

    PROCESSOR=`cat /proc/cpuinfo | head -1 | sed 's/^.*: //' `
    MEMORY=`cat /proc/meminfo | head -1 | sed 's/^MemTotal: \+//' `
    ROOTFS_VERSION=`echo $VERSION | sed -e 's/.*-g/g/'`
    ROOTFS_DATE=`date -r /etc/os-release '+%a %b %d %Y' `
    KERNEL_VERSION=`uname -r`
    KERNEL_DATE=`uname -v | cut -d ' ' -f 3-5,8`

    echo "Processor: $PROCESSOR"
    echo "Memory: $MEMORY"
    echo "Kernel version: $KERNEL_VERSION"
    echo "Kernel date: $KERNEL_DATE"
    echo "Root FS version: $ROOTFS_VERSION"
    echo "Root FS date: $ROOTFS_DATE"
}

cmd_date_get() {
    date +"%Y-%m-%d"
}

cmd_date_get_localized() {
    date +"%x"
}

cmd_date_set() {
    y=$1
    m=$2
    d=$3
    time=`cmd_time_get`

    date -s "$y-$m-$d $time"
}

cmd_time_get() {
    date +"%H:%M:%S"
}

cmd_time_get_localized() {
    date +"%X"
}

cmd_time_set() {
    h=$1
    m=$2
    s=$3
    date=`cmd_date_get`

    date -s "$date $h:$m:$s"
}

cmd_timezone_get() {
    date +"%Z"
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
    local opk="/media/data/apps/oddity.opk"

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
