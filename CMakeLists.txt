cmake_minimum_required(VERSION 3.23)
project(SetsDistance C)

set(CMAKE_C_STANDARD 11)

add_executable(SetsDistance main.c src/utils.c include/utils.h include/defines.h src/fileParser.c include/fileParser.h src/data.c include/data.h)
