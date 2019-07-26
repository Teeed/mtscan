#!/bin/bash

set -e

ARCH="${ARCH:-x86_64}"

cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make -j4
make install DESTDIR=AppDir

cp ../icons/48x48/apps/mtscan.png AppDir/
cat > AppDir/mtscan.desktop << ___KEK
[Desktop Entry]
Type=Application
Name=MTScan
Comment=Mikrotik Scanner
Icon=mtscan
Exec=mtscan
Terminal=false
Categories=Network;

___KEK

wget -O AppDir/AppRun https://github.com/AppImage/AppImageKit/releases/download/12/AppRun-${ARCH}
chmod +x AppDir/AppRun


wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${ARCH}.AppImage
chmod +x linuxdeploy-${ARCH}.AppImage

# Hack to avoid using FUSEFS in docker
./linuxdeploy-${ARCH}.AppImage --appimage-extract
./squashfs-root/AppRun --appdir AppDir/ --output appimage

