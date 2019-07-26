#!/bin/bash

set -e


apt update
apt install -y cmake build-essential pkg-config libgtk2.0-dev libssh-dev libyajl-dev libcurl4-openssl-dev libpcap-dev wget libfuse2

cd /app/build
./build-appimage.sh