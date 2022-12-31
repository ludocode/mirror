#!/bin/sh
cd "$(dirname "$0")"/..
make -f test/Makefile "$@"
