cmake_minimum_required(VERSION 3.10)
project(crisp)

include_directories("${PROJECT_BINARY_DIR}")

set(SOURCES list.c parser.c scanner.c)
add_executable(crisp crisp.c ${SOURCES})