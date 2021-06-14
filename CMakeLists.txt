cmake_minimum_required(VERSION 3.10)

project(libvcol)

set(LIBVCOL_C_FILES src/bstrglue.c src/bstrlib/bstrlib.c src/goki.c src/hashmap.c src/mem.c src/vector.c)

add_library(libvcol ${LIBVCOL_C_FILES})

