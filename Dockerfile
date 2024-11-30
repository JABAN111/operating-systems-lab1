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
    git \
    linux-tools-common \
    linux-tools-generic \
    linux-tools-5.4.0-146-generic \
    time \
    htop

WORKDIR /app
COPY . .

# Компиляция без оптимизации
RUN g++ -std=c++17 -pthread -o combined combined.cpp
RUN g++ -o factorize factorize.cpp
RUN g++ -o ema-sort-int ema-sort-int.cpp

WORKDIR /agressive_version
COPY . .

# Компиляция с агрессивной оптимизацией
RUN g++ -std=c++17 -pthread -O3 -march=native -flto -o agr_combined combined.cpp
RUN g++ -O3 -march=native -flto -o agr_factorize factorize.cpp
RUN g++ -O3 -o agr_ema-sort-int ema-sort-int.cpp