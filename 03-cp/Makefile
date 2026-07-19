tests: cp.c tests.cpp
	gcc -c cp.c
	g++ -std=c++17 -o tests tests.cpp -lgtest -lgtest_main -pthread  cp.o -g
clean: 
	rm -rf cp.o tests.o tests
