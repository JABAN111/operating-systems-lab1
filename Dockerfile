FROM ubuntu:20.04

LABEL authors="jaba"

ENV TZ=Europe/Moscow
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    tzdata \
    g++ \
    make \
    clang-tidy \
    cmake \
    git

WORKDIR /app
COPY . .

RUN clang-tidy *.cpp -- -I/usr/include/c++/9 -I/usr/include/x86_64-linux-gnu/c++/9

RUN g++ -std=c++17 -pthread -o combined combined.cpp
RUN g++ -o factorize factorize.cpp
RUN g++ -o ema-sort-int ema-sort-int.cpp