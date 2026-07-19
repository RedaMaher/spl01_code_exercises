tests: mv.c tests.cpp
	gcc -c mv.c
	g++ -std=c++17 -o tests tests.cpp -lgtest -lgtest_main -pthread  mv.o -g
clean: 
	rm -rf mv.o tests.o tests
