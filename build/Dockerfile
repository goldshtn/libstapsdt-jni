FROM ubuntu:xenial

RUN apt update -y && apt install -y libelf1 libelf-dev git make gcc

VOLUME /out

WORKDIR /build

COPY build.sh /build

ENTRYPOINT ["./build.sh"]
