#!/bin/sh
#

find **/*.{h,cpp} | xargs clang-format -i
