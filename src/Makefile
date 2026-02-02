CC = g++
CCFLAGS = -Wall -Wextra -std=c++17 -g
NCURSES_FLAGS = -lncurses
CHECK_FLAGS = -lcheck -lsubunit
LIBFLAGS = -L. -L/usr/local/lib -L/usr/include
RM = rm -rf

GCOVR = gcovr
# GCOVR = gcovr # pip install gcovr
# GCOVR = ~/.local/bin/gcovr


# имя исполняемого файла
GAME_NAME = tetris
# имя тестового файла
ТESTS_NAME = tetris_test
# имя тестового файла c тестами покрытия кода gcovr
ТESTS_GCOV_NAME = tetris_tests_gcov
# имя директории для цели dist
TAR_DIR = s21_tetris-1.0
# директория установки
INSTALL_DIR = ./install

# основные файлы и директории
MAIN_DIR = ./
MAIN_SOURCES = ./tetris.cc
MAIN_OBJECTS = $(MAIN_SOURCES:.cc=.o)


# файлы и директории функций библиотеки backend'a
BACK_LIB_NAME = backend.a
BACK_LIB_DIR = ./brick_game/tetris
BACK_LIB_SOURCES = $(shell find $(BACK_LIB_DIR) -name "*.cc")
BACK_OBJECTS = $(BACK_LIB_SOURCES:.cc=.o)


# файлы и директории функций frontend'a
FRONT_DIR = ./gui/cli
FRONT_SOURCES = $(shell find $(FRONT_DIR) -name "*.cc")
FRONT_OBJECTS = $(FRONT_SOURCES:.cc=.o)


# файлы и директории тестов
TEST_DIR = tests/tetris
TEST_HEADER_NAME = tetris_test.h
TEST_SRC = $(shell find $(TEST_DIR) -name "*.cc")
TEST_OBJ = $(TEST_SRC:.cc=.o)

.PHONY: all build clean install uninstall dvi dist

all: clean build

%.o: %.cc
	$(CC) $(CCFLAGS) -c $< -o $@

build: $(BACK_LIB_NAME) $(FRONT_OBJECTS) $(MAIN_OBJECTS)
	$(CC) $(CCFLAGS) $(MAIN_OBJECTS) $(LIBFLAGS) -l:$(BACK_LIB_NAME) $(FRONT_OBJECTS) $(NCURSES_FLAGS) -o $(GAME_NAME)

install: build
	mkdir $(INSTALL_DIR)
	cp -p $(GAME_NAME) $(INSTALL_DIR)

uninstall: 
	$(RM) $(INSTALL_DIR)

dvi:
	open ./doxygen/html/index.html

dist: build
	$(RM) $(TAR_DIR)
	mkdir $(TAR_DIR)
	cp -p $(GAME_NAME) $(TAR_DIR)
	tar czf $(TAR_DIR).tar.gz $(TAR_DIR)
	$(RM) $(TAR_DIR)
	
test: clean $(ТESTS_NAME)
	./$(ТESTS_NAME)

$(ТESTS_NAME): $(TEST_OBJ) $(BACK_LIB_NAME)
	$(CC) $(CCFLAGS) $^ $(LIBFLAGS) -l:$(BACK_LIB_NAME) $(CHECK_FLAGS) -o $@ $(NCURSES_FLAGS)

gcov_report: clean_gcov clean_report $(ТESTS_GCOV_NAME)
	./$(ТESTS_GCOV_NAME)
	mkdir report
	$(GCOVR) -r . --html --html-details -o report/coverage.html --exclude=$(TEST_DIR)
	make clean_gcov
	open report/coverage.html

$(ТESTS_GCOV_NAME):$(TEST_SRC) $(BACK_LIB_SOURCES)
	$(CC) $(CCFLAGS) -g -fprofile-arcs -ftest-coverage $^ -o $@ $(NCURSES_FLAGS) $(CHECK_FLAGS)

clang:
	clang-format -n $(shell find $(MAIN_DIR) -name "*.cc")
	clang-format -n $(shell find $(MAIN_DIR) -name "*.h")

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem --check-config tetris.cc ./brick_game/tetris/ ./gui/cli/

valgrind: test
	valgrind --tool=memcheck --leak-check=yes ./$(ТESTS_NAME)

clean: clean_gcov clean_report
	$(RM) $(GAME_NAME)
	$(RM) $(ТESTS_NAME)
	$(RM) $(shell find $(DIR) -name "*.a")
	$(RM) $(shell find $(DIR) -name "*.o")
	$(RM) $(shell find $(DIR) -name "*.gz")

clean_report:
	$(RM) report

clean_gcov:
	$(RM) $(ТESTS_GCOV_NAME)
	$(RM) *.gcda
	$(RM) *.gcno

$(BACK_LIB_NAME): $(BACK_OBJECTS)
	ar rc $@ $^
	ranlib $@
