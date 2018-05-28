cmake_minimum_required(VERSION 3.11)
set(CMAKE_C_FLAGS "-Wall -Wextra")

include_directories(include/)

set(SOURCES src/cplf.c src/util.c src/print.c src/repair.c src/options.c src/parser.c src/timeseries.c)
set(HEADERS include/cplf.h)

add_executable(cplf ${SOURCES} ${HEADERS})

