#!/bin/sh

docker run --rm -i -t  -v `pwd`/../:/app --device /dev/fuse --cap-add SYS_ADMIN ubuntu:bionic /bin/bash -c "cd /app/build && ./build-appimage-onubuntu.sh"