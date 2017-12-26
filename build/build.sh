#!/bin/bash

git clone --depth=1 https://github.com/sthima/libstapsdt
cd libstapsdt
make
cp out/libstapsdt.so.0 src/libstapsdt.h /out
