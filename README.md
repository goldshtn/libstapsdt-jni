## Java wrapper for libstapsdt

This is a proof-of-concept Java wrapper for the [libstapsdt](https://github.com/sthima/libstapsdt) library, which enables JVM applications to create USDT probes and fire them dynamically, without defining anything ahead of time.

### Build

The included [Makefile](Makefile) uses two Docker containers to build libstapsdt and the demo application. The first [Dockerfile](build/Dockerfile) grabs libstapsdt and produces libstapsdt.so; the second [Dockerfile](java/Dockerfile) builds the JNI wrapper and the demo application, and runs the demo application in a container.

Simply run `make` to go through the whole process. If you want to use the resulting artifacts in another application, you'll need the wrapper's Java files in [java/sdt](java/sdt) and the [build steps](java/build.sh) that generate the JNI header and build the JNI library libstapsdt-jni.so.

> NOTE: Because I'm very far from being a Java packaging expert, contributions which will clean this up and turn this wrapper into a consumable package would be very much appreciated :-)

### Demo

To observe the demo, run `make` and then in another shell, use any tool that supports SystemTap probes. For example, if you have the [BCC](https://github.com/iovisor/bcc) tools installed, you should be able to see the probes as follows:

```
# tplist -p $(pidof java) -vv jprovider:*
/tmp/jprovider-FhrqlC.so jprovider:jprobe [sema 0x0]
  location #1 0x250
    argument #1 4 signed   bytes @ di
    argument #2 8 signed   bytes @ si
    argument #3 8 unsigned bytes @ dx

# trace -p $(pidof java) 'u::jprobe "i=%d i*i=%lld msg=%s", arg1, arg2, arg3'
PID    TID    COMM         FUNC             -
27845  27846  java         jprobe           i=269 i*i=72361 msg=iteration #269 done
27845  27846  java         jprobe           i=270 i*i=72900 msg=iteration #270 done
27845  27846  java         jprobe           i=271 i*i=73441 msg=iteration #271 done
27845  27846  java         jprobe           i=272 i*i=73984 msg=iteration #272 done
^C
```
