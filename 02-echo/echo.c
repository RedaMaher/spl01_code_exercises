#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echo_main (int argc , char* argv []){

	int bytes_number=0;
	int total_bytes=0;
	
	if (argc == 1){
	printf("\n");
	return 0;
	}
	
	int maximum_arg =0 ;
	
	for (int i = 0 ; i < argc ; i++){
		if (strlen(argv [i])>maximum_arg ){
		maximum_arg =strlen(argv [i]);
	
		
		}
		
	
	}
	
	
	
	
	/*
	
	char buf[total_bytes];
	int j =0;
	
	
	for (int i = 0 ; i < argc ; i++){
		buf[j] = argv[i];
		j++;
	
	}
	
	
	
	if (argc > 1) 
	
	write (1 , argv[i] , 4);
	int x = sizeof(*argv[0]);
		printf ("%d\n" , x);*/
		

	char buf[argc+2 ] [maximum_arg +2]; 
	
		
	for (int i = 1 ; i < argc ; i++){
		int x = strlen(argv[i]);
		for (int j = 0 ; j < x ; j++){
			buf [i][j] = argv[i][j];
			
			
			
			if (j == x -1  && i != argc -1){
			
				buf [i] [j+1] = ' ';
				buf [i] [j+2] = '\0';
				break;
				
			
			}
			else if (j == x -1  && i == argc -1){
			

				buf [i] [j+1] = '\0';
				break;
				
			
			}
			
			
			

	
	}
	}
	
	
	for (int i = 1 ; i < argc ; i++){

		if (  write (1 , buf[i], ( strlen(buf[i]) ) )  <0   ) {
		
		
		exit (-5);
		}
		
	
	}
	printf ("\n");
	return 0;

	




}
