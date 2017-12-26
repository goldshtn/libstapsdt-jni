FROM openjdk:8

RUN apt update -y && apt install -y g++

WORKDIR /src

COPY . /src

ENTRYPOINT ["./build.sh"]
