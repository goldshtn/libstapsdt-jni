#!/bin/bash

JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64

javac sdt/*.java
javah sdt.Native
cp sdt_Native.h /out

gcc -shared -fPIC -std=c++11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -I/out \
    -o libstapsdt-jni.so sdt_Native.cc
cp libstapsdt-jni.so /out

LD_PRELOAD=/out/libstapsdt.so.0 java -Djava.library.path=/out sdt/Demo
