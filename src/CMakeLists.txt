set(SOURCES list.c parser.c scanner.c)
include_directories("${PROJECT_BINARY_DIR}/src")

add_executable(crisp crisp.c ${SOURCES})
install_targets(/bin crisp)