#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"

void substring(char s[], char sub[], int p, int l) {
   int c = 0;   
   while (c < l) {
      sub[c] = s[p + c - 1];
      c ++;
   }
   sub[c] = '\0';
}

int ask_command(char airport1 [], char airport2 []){
	char command[128];
	char * one = "help";
	char * two = "quit";
	char * three = "airport";
	char * four = "distance";
	printf("enter command> ");
	scanf(" %[^\n]", command);
	if (strlen(command) > 7){
		char sub[128];
		substring(command, sub, 1, 8);
//		printf("%s\n", sub);
		strcpy(command, sub);
		substring(command, airport1, 11, 3);
		substring(command, airport2, 17, 3);
		airport1[3] = '\0';
		airport2[3] = '\0';
//		printf("%s, %s\n", airport1, airport2);
	}
	while((strcmp(command, one) != 0) && (strcmp(command, two) != 0) && (strcmp(command, three) != 0) && (strcmp(command, four) != 0) ){
//		printf("%s", command);
		printf("Invalid command, please enter 'help' to know how to use the program!\n\n");
		printf("enter command> ");
		scanf(" %[^\n]", command);
		if (strlen(command) > 7){
                char sub[128];
		substring(command, sub, 1, 8);
		strcpy(command, sub);
                substring(command, airport1, 11, 3);
                substring(command, airport2, 17, 3);
                airport1[3] = '\0';
                airport2[3] = '\0';
		}
	}
	if (!strcmp(command, one))
		return 1;
	if (!strcmp(command, two))
                return 2;
	if (!strcmp(command, three))
                return 3;
	if (!strcmp(command, four))
                return 4;
	return 0;
} 

int hash(char * name){
	return ((name[0] - 'A') * 676 + (name[1] - 'A') * 26 + (name[2] - 'A'));

}

int main(int argc, char **argv ){
	if (argc != 3){
		printf("To run: ./test <airport names> <airport distances>\n");
		return 0;
	}
//	printf("here");
	int length = 17576;
	int length_of_airports = 0;
	char airports[length][128];
	int name_number[length];
	for (int i = 0; i < length; i ++){
                name_number[i] = -1;
        }

//	printf("length: %d", length);
	FILE * file = fopen(argv[1], "r");
	if (file != NULL){
		char buffer[128];
		while (fgets(buffer, 128, file) != NULL){
			strcpy(airports[length_of_airports], buffer);
			airports[length_of_airports][strcspn(airports[length_of_airports], "\r\n")] = '\0';
			length_of_airports += 1;
		}
			
	}else{
		perror(argv[1]);
	}
	char names[length_of_airports][4];
	for(int i = 0; i < length_of_airports; i ++){
//		printf("%s\n", airports[i]);
		substring(airports[i], names[i], 1, 3);
//		printf("%s\n", names[i]);
		names[i][3] = '\0';
		name_number[hash(names[i])] = i;
//		printf("%d\n", name_number[hash(names[i])]);
	}
	
//	for (int i = 0; i < length; i ++){
//		printf("%d\n", name_number[i]);
//	}
	char airport1[4];
        char airport2[4];
	char ** names2 = malloc(length_of_airports * sizeof(char *));
	for(int i = 0; i < length_of_airports; i ++){
		names2[i] = names[i];
	}
	int command = ask_command(airport1, airport2);
	while(command != 2){
		if (command == 1){
			printf("How to use the program:\n1. Enter 'quit' to end the program\n2. Enter 'airport' to see all airports' names and their abbreviation\n3. Enter distance <airport1> <airport2> (both names are abbreviations) to find the shortest distance between two airports!\n\n");
		}else if (command == 3){
			for (int i = 0; i < length_of_airports; i ++){
				printf("%s\n", airports[i]);
			}
			printf("\n");
		}else if (command == 4){
			printf("airport 1: %s, airport 2: %s\n", airport1, airport2);
			bool a = false;
			bool b = false;
			for(int i = 0; i < length_of_airports; i ++){
				if (!strcmp(airport1, names[i]))
					a = true;
				if (!strcmp(airport2, names[i]))
                                        b = true;
			}
			if (a && b){
//				printf("Both correct\n");
				int airport11 = name_number[hash(airport1)];
				int airport22 = name_number[hash(airport2)];
//				printf("%d, %d\n", airport11, airport22);
				FILE * file2 = fopen(argv[2], "r");
				if (file2 != NULL){
					Graph * new = create_graph(length_of_airports);
					int value; 
					char airport111[4]; 
					char airport222[4];
//					printf("here\n");
					while(fscanf(file2, "%s %s %d", airport111, airport222, &value) != EOF) {
//						printf("%d, %d\n", hash(airport111), hash(airport222));
						int buffer1 = name_number[hash(airport111)];
						int buffer2 = name_number[hash(airport222)];
        					add(new, buffer1, buffer2, value);
        					add(new, buffer2, buffer1, value);
    					}
    					fclose(file2);
//					printf("here\n);
					Vertex * node = malloc(sizeof(node));
					dijkstra(new, airport11, airport22, length_of_airports, node, names2);
					printf("Dijkstra Total Distance: %d\n", new->vertex[airport22]->cost);
					printf("\n");
				}else{
					perror(argv[2]);
				}
				
			}else{
				printf("Invalid airport name(s), please enter 'help' to know how to use the program\n\n");
			}
			for (int i = 0; i < 3; i ++){
				airport1[i] = '\0';
				airport2[i] = '\0';
			}
		}else
			printf("Never here\n");
		command = ask_command(airport1, airport2);
	}
	printf("Good Bye!\n");
	return 0;
}
