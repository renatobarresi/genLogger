#!/bin/bash

clang-format -style=file -i -fallback-style=none \
	src/**/*.c src/**/*.h test/**/*.c test/**/*.h