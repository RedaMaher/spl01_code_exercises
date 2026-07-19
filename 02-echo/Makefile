tests: echo.c tests.cpp
	gcc -c echo.c
	g++ -std=c++17 -o tests tests.cpp -lgtest -lgtest_main -pthread  echo.o -g
clean: 
	rm -rf echo.o tests.o tests
