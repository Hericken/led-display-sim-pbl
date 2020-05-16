#! /bin/bash

if [ $# -lt 1 ]; then
    printf "too few arguments  Usage:$0 [run file] [option]\n"
    exit 2
fi

TARGET=$1
shift
printf "Running virtual LED display (timeout:30s)... "
timeout 30 ./${TARGET} $@
printf "Done\n"

if [ ! -e log/rectmp/0001.bmp ]; then
    printf "No bmp file in 'log/rectmp' directory\n"
    exit 2
fi

if type ffmpeg > /dev/null 2>&1; then
    printf "Writing to video file... "
    ffmpeg -r 30 -i log/rectmp/%04d.bmp log/rectmp/tmp.mp4 -loglevel fatal
else
    if !(type obj/ffmpeg-git*/ffmpeg > /dev/null) 2>&1; then
        ARCH=`uname -e`
        if [ ARCH = "x86_64" ]; then
            cd obj
            curl -OL https://johnvansickle.com/ffmpeg/builds/ffmpeg-git-amd64-static.tar.xz
            tar -Jxf ffmpeg-git-amd64-static.tar.xz
            rm ffmpeg-git-amd64-static.tar.xz
            cd ../
        elif [ ARCH = "armv7l" ]; then
            cd obj
            curl -OL https://johnvansickle.com/ffmpeg/builds/ffmpeg-git-arm64-static.tar.xz
            tar -Jxf ffmpeg-git-arm64-static.tar.xz
            rm ffmpeg-git-arm64-static.tar.xz
            cd ../
        else
            printf "command 'ffmpeg' not found"
        fi
    fi
    printf "Writing to video file... "
    obj/ffmpeg-git*/ffmpeg -r 30 -i log/rectmp/%04d.bmp log/rectmp/tmp.gif -loglevel fatal
fi
if [ $? = 0 ]; then
    printf "Done\n"
else
    printf "Failure\n"
    exit 2
fi

rm log/rectmp/*.bmp
if [ ! -e log/rectmp/tmp.mp4 ]; then
    printf "'log/rectmp/tmp.mp4': No such file\n"
    exit 2
fi
TIME=$(date +%m%d_%H-%M-%S)
mv log/rectmp/tmp.mp4 log/led_matrix_${TIME}.mp4
printf "save to file 'log/led_matrix_${TIME}.mp4'\n"
