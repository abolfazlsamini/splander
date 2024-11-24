#!/bin/sh

set -xe

mkdir -p build
make -C ./src PROJECT_BUILD_PATH=../build
