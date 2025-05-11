# Base image with build essentials and modern C++
FROM ubuntu:25.04

# Avoids timezone prompting during build
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools, debugging tools, and libraries
RUN apt update && \
  apt upgrade -y && \
  apt install -y \
  build-essential \
  procps \
  curl \
  file \
  git \
  valgrind \
  gdb \
  python3 \
  clangd \
  cmake \
  ninja-build \
  liburing-dev \
  sudo \
  && apt-get clean && \
  rm -rf /var/lib/apt/lists/* /var/cache/apt/archives/*


RUN git clone https://github.com/ada-url/ada.git /workspace/ada
WORKDIR /workspace/ada
RUN cmake -S . -B build -G Ninja
RUN cmake --build build --target install


# https://github.com/nodejs/llhttp
