CC = gcc
FLAG_C = -c
FLAG_ER = -Wall -Werror -Wextra -std=c11
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
TET_LIB_C = ./brick_game/tetris/fsm.c ./brick_game/tetris/tetris.c ./brick_game/tetris/additional.c
TET_LIB_O = fsm.o additional.o tetris.o
TET_LIB_H = ./brick_game/tetris/tetris.h
SUITE_CASES_C = suite_*.c
SUITE_CASES_O = suite_*.o
FLAG_TESTS = -lcheck
TESTS_C=test.c
TESTS_O=test.c
LINUX=-lsubunit -lrt
LIBS=-lcheck -lm -lpthread

ifeq ($(shell uname), Linux)
	FLAG_TEST +=-lsubunit -lm -lcheck_pic
endif


all: clean uninstall install 

install:tetris.a
	gcc -Wall -Wextra -Werror -Wpedantic -o tetris main.c  gui/cli/frontend.c tetris.a -lncurses
	make clean

uninstall:
	-rm -f tetris_game 

tetris.a:
	$(CC) $(FLAG_C) $(FLAG_ER) $(TET_LIB_C)  
	ar rc tetris.a $(TET_LIB_O) -lm
	ranlib tetris.a

test: clean tetris.a
	gcc -g --coverage $(FLAG_ER) $(TESTS_C) -o test tetris.a -lcheck_pic -lm -lsubunit
	./test

gcov_report:
ifeq ($(OS), Darwin)
	gcc $(WWW) -fprofile-arcs -ftest-coverage -pedantic -g tetris.a -o unit_test $(LIBS) 
else
	gcc  $(WWW) -fprofile-arcs -ftest-coverage -pedantic  -g $(TET_LIB_C) test.c tetris.a -o unit_test $(LIBS) $(LINUX)
endif
	./unit_test
	lcov -t "test" -o test.info -c -d .  
	genhtml test.info --output-directory report
	open ./report/index.html


dvi:
	open ./README.md

dist:
	tar -czf tetris.install.tar.gz ./* 

clean:
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt
	-rm -f test
	-rm -f tests
	-rm -f greport
	-rm -rf report
	-rm -f tetris.install.tar.gz 
	find . -type d -name 'tests.dSYM' -exec rm -r {} +
