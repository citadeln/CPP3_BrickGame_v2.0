CC   = g++
CCFLAGS    = -Wall -Wextra -std=c++17 -g
NCURSES    = -lncurses
CHECK_FLAGS= -lcheck
LIBFLAGS   = -L. -L/usr/local/lib
RM   = rm -rf

GCOVR = gcovr

# ── Имена исполняемых файлов ──────────────────────────────────────────────────
GAME_NAME    = brickgame
QT_TARGET    = brickgame-qt
SNAKE_TEST_NAME   = snake_test
TETRIS_TEST_NAME  = tetris_test

# ── Основные исходники ────────────────────────────────────────────────────────
MAIN_SRC = ./main.cc

# ── Tetris backend ────────────────────────────────────────────────────────────
TETRIS_LIB   = tetris_backend.a
TETRIS_DIR   = ./brick_game/tetris
TETRIS_SRC   = $(shell find $(TETRIS_DIR) -name "*.cc")
TETRIS_OBJ   = $(TETRIS_SRC:.cc=.o)

# ── Snake backend (Model + ViewModel) ────────────────────────────────────────
SNAKE_LIB    = snake_backend.a
SNAKE_DIR    = ./brick_game/snake
SNAKE_SRC    = $(shell find $(SNAKE_DIR) -name "*.cc")
SNAKE_OBJ    = $(SNAKE_SRC:.cc=.o)

# ── CLI GUI (ncurses frontend) ────────────────────────────────────────────────
GUI_DIR  = ./gui/cli
GUI_SRC  = $(shell find $(GUI_DIR) -name "*.cc")
GUI_OBJ  = $(GUI_SRC:.cc=.o)

# ── Qt GUI (desktop frontend) ─────────────────────────────────────────────────
DESKTOP_DIR  = ./gui/desktop
QT_CFLAGS    = -fPIC $(shell pkg-config --cflags Qt5Widgets 2>/dev/null)
QT_LIBS      = $(shell pkg-config --libs   Qt5Widgets 2>/dev/null)
MOC   = $(shell pkg-config --variable=host_bins Qt5Core 2>/dev/null)/moc

# Исходники Qt View (без moc-файлов)
DESKTOP_SRC  = $(DESKTOP_DIR)/snake_widget.cc \
	       $(DESKTOP_DIR)/snake_window.cc \
	       $(DESKTOP_DIR)/tetris_widget.cc

# MOC-файлы генерируются из заголовков с Q_OBJECT
MOC_SRCS     = $(DESKTOP_DIR)/moc_snake_widget.cc \
	       $(DESKTOP_DIR)/moc_snake_window.cc \
	       $(DESKTOP_DIR)/moc_tetris_widget.cc

DESKTOP_OBJ  = $(DESKTOP_SRC:.cc=.o) $(MOC_SRCS:.cc=.o)

# main_qt.cc компилируется с Qt-флагами
MAIN_QT_OBJ  = main_qt.o

# ── Тесты Змейки ─────────────────────────────────────────────────────────────
SNAKE_TEST_DIR = ./tests/snake
SNAKE_TEST_SRC = $(shell find $(SNAKE_TEST_DIR) -name "*.cc")
SNAKE_TEST_OBJ = $(SNAKE_TEST_SRC:.cc=.o)

# ── Тесты Тетриса ─────────────────────────────────────────────────────────────
TETRIS_TEST_DIR = ./tests/tetris
TETRIS_TEST_SRC = $(shell find $(TETRIS_TEST_DIR) -name "*.cc")
TETRIS_TEST_OBJ = $(TETRIS_TEST_SRC:.cc=.o)

# ── Установка ─────────────────────────────────────────────────────────────────
INSTALL_DIR = ./install
TAR_DIR     = s21_brickgame-1.0

# ─────────────────────────────────────────────────────────────────────────────
.PHONY: all build build-qt clean install uninstall dvi dist \
	test test_snake test_tetris gcov_report style cpp valgrind

all: clean build

# ── Шаблонное правило: обычная сборка (без Qt) ────────────────────────────────
%.o: %.cc
	$(CC) $(CCFLAGS) -c $< -o $@

# ── Библиотеки ────────────────────────────────────────────────────────────────
$(TETRIS_LIB): $(TETRIS_OBJ)
	ar rc $@ $^
	ranlib $@

$(SNAKE_LIB): $(SNAKE_OBJ)
	ar rc $@ $^
	ranlib $@

# ── Основная сборка (ncurses) ─────────────────────────────────────────────────
build: $(TETRIS_LIB) $(SNAKE_LIB) $(GUI_OBJ) $(MAIN_SRC:.cc=.o)
	$(CC) $(CCFLAGS) $(MAIN_SRC:.cc=.o) $(LIBFLAGS) \
	    -l:$(TETRIS_LIB) -l:$(SNAKE_LIB) $(GUI_OBJ) $(NCURSES) -o $(GAME_NAME)

# ─────────────────────────────────────────────────────────────────────────────
# Qt сборка: make build-qt → ./brickgame-qt
#
# Зависимости: Qt5Widgets (pkg-config), moc (Qt5Core)
# Модель и ViewModel не меняются — только View-слой Qt
# ─────────────────────────────────────────────────────────────────────────────

# MOC: генерируем moc_*.cc из заголовков с Q_OBJECT
$(DESKTOP_DIR)/moc_snake_widget.cc: $(DESKTOP_DIR)/snake_widget.h
	$(MOC) $< -o $@

$(DESKTOP_DIR)/moc_snake_window.cc: $(DESKTOP_DIR)/snake_window.h
	$(MOC) $< -o $@

$(DESKTOP_DIR)/moc_tetris_widget.cc: $(DESKTOP_DIR)/tetris_widget.h
	$(MOC) $< -o $@

# Qt-файлы компилируются с Qt-флагами (через отдельные правила)
$(DESKTOP_DIR)/%.o: $(DESKTOP_DIR)/%.cc
	$(CC) $(CCFLAGS) $(QT_CFLAGS) -c $< -o $@

$(MAIN_QT_OBJ): main_qt.cc
	$(CC) $(CCFLAGS) $(QT_CFLAGS) -c $< -o $@

# Финальная сборка Qt-версии (Model + ViewModel — из SNAKE_LIB, View — из DESKTOP_OBJ)
build-qt: $(TETRIS_LIB) $(SNAKE_LIB) $(DESKTOP_OBJ) $(MAIN_QT_OBJ)
	$(CC) $(CCFLAGS) $(QT_CFLAGS) \
	    $(MAIN_QT_OBJ) $(DESKTOP_OBJ) \
	    $(LIBFLAGS) -l:$(TETRIS_LIB) -l:$(SNAKE_LIB) $(QT_LIBS) \
	    -o $(QT_TARGET)

# ─────────────────────────────────────────────────────────────────────────────

install: build
	mkdir -p $(INSTALL_DIR)
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

# ── Тесты ─────────────────────────────────────────────────────────────────────
test: test_snake test_tetris

test_snake: clean $(SNAKE_TEST_NAME)
	./$(SNAKE_TEST_NAME)

$(SNAKE_TEST_NAME): $(SNAKE_TEST_OBJ) $(SNAKE_LIB)
	$(CC) $(CCFLAGS) $^ $(LIBFLAGS) -l:$(SNAKE_LIB) $(CHECK_FLAGS) -o $@

test_tetris: clean $(TETRIS_TEST_NAME)
	./$(TETRIS_TEST_NAME)

$(TETRIS_TEST_NAME): $(TETRIS_TEST_OBJ) $(TETRIS_LIB)
	$(CC) $(CCFLAGS) $^ $(LIBFLAGS) -l:$(TETRIS_LIB) $(CHECK_FLAGS) -o $@ $(NCURSES)

# ── Покрытие кода ─────────────────────────────────────────────────────────────
SNAKE_GCOV_NAME = snake_tests_gcov

gcov_report: clean_gcov clean_report $(SNAKE_GCOV_NAME)
	./$(SNAKE_GCOV_NAME)
	mkdir report
	$(GCOVR) -r . --html --html-details -o report/coverage.html \
	    --exclude=$(SNAKE_TEST_DIR)
	make clean_gcov
	open report/coverage.html

$(SNAKE_GCOV_NAME): $(SNAKE_TEST_SRC) $(SNAKE_SRC)
	$(CC) $(CCFLAGS) -g -fprofile-arcs -ftest-coverage $^ \
	    -o $@ $(CHECK_FLAGS)

# ── Стиль кода ────────────────────────────────────────────────────────────────
style:
	clang-format -n $(shell find . -name "*.cc" -o -name "*.h")
	clang-format -i $(shell find . -name "*.cc" -o -name "*.h")

# ── Статический анализ ────────────────────────────────────────────────────────
cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	    main.cc ./brick_game/snake/ ./brick_game/tetris/ ./gui/cli/

# ── Valgrind ──────────────────────────────────────────────────────────────────
valgrind: test_snake
	valgrind --tool=memcheck --leak-check=yes ./$(SNAKE_TEST_NAME)

# ── Очистка ───────────────────────────────────────────────────────────────────
clean: clean_gcov clean_report
	$(RM) $(GAME_NAME) $(QT_TARGET) $(SNAKE_TEST_NAME) $(TETRIS_TEST_NAME)
	$(RM) $(shell find . -name "*.a"  2>/dev/null)
	$(RM) $(shell find . -name "*.o"  2>/dev/null)
	$(RM) $(shell find . -name "*.gz" 2>/dev/null)
	$(RM) $(DESKTOP_DIR)/moc_*.cc

clean_report:
	$(RM) report

clean_gcov:
	$(RM) $(SNAKE_GCOV_NAME)
	$(RM) *.gcda *.gcno
