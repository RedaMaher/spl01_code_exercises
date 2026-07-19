tests: pwd.c tests.cpp
	gcc -c pwd.c
	g++ -std=c++17 -o tests tests.cpp -lgtest -lgtest_main -pthread  pwd.o -g
clean: 
	rm -rf pwd.o tests.o tests
