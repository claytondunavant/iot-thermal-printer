# You can blame Fedora having poor cross-compiling support for this

# version of ubuntu with glibc 2.31
FROM ubuntu:20.04

WORKDIR /app

# Update the package list and install required packages
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    g++-aarch64-linux-gnu 

# Clean up after installation to reduce the image size
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

RUN cd /app
CMD make CC=aarch64-linux-gnu-g++