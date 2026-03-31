CC = g++
CFLAGS = -Wall -std=c++17 -I. -Ilib

GTEST_DIR = googletest/googletest
GTEST_INC = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
GTEST_OBJ = gtest-all.o

program: main.cpp
	$(CC) $(CFLAGS) main.cpp -o program

$(GTEST_OBJ):
	$(CC) $(CFLAGS) $(GTEST_INC) -c $(GTEST_DIR)/src/gtest-all.cc -o $(GTEST_OBJ)

tests: tests.cpp $(GTEST_OBJ)
	$(CC) $(CFLAGS) $(GTEST_INC) tests.cpp $(GTEST_OBJ) -o tests -pthread

run_tests: tests
	./tests --gtest_color=yes

clean:
	rm -f program tests $(GTEST_OBJ)
