#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
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
	char * five = "test";
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
	while((strcmp(command, one) != 0) && (strcmp(command, two) != 0) && (strcmp(command, three) != 0) && (strcmp(command, four) != 0) && (strcmp(command, five) != 0) ){
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
	if (!strcmp(command, five))
                return 5;
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
			printf("How to use the program:\n1. Enter 'quit' to end the program\n2. Enter 'airport' to see all airports' names and their abbreviation\n3. Enter distance <airport1> <airport2> (both names are abbreviations) to find the shortest distance between two airports!\n5. Enter 'test' to see the running time\n\n");
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
					Vertex * node = malloc(sizeof(Vertex));
					dijkstra(new, airport11, airport22, length_of_airports, node, names2);
					printf("Dijkstra Total Distance: %d\n", new->vertex[airport22]->cost);
					free(node);
					for (int i = 0; i < length_of_airports; i++){
                				if(new->vertex[i]) {
                					for(int j = 0; j < (new->vertex[i])->length; j++){
                        					free((new->vertex[i])->edge[j]);
                					}
                					free((new->vertex[i])->edge);
                				}
                				free(new->vertex[i]);
        				}
        				free(new->vertex);
				        free(new);
					
				}else{
					perror(argv[2]);
				}
				
				FILE * file3 = fopen(argv[2], "r");
                                if (file3 != NULL){
					char airports1 [5000][4];
					char airports2 [5000][4];
					int distance [5000];
					for (int i = 0; i < 5000; i ++){
						airports1[i][4] = '\0';
						airports1[i][4] = '\0';
						distance[i] = -1;
					}	
					int count = 0;
					while(fscanf(file3, "%s %s %d", airports1[count], airports2[count], &distance[count]) != EOF){
						count ++;
					}
					fclose(file3);	
					Graph2 * new2 = create_bellman(length_of_airports, 2 * count);
					int j = 0;
					for (int i = 0; i < count * 2; i ++){
						new2->edge[i].src = name_number[hash(airports1[j])];
						new2->edge[i].dest = name_number[hash(airports2[j])];
						new2->edge[i].weight = distance[j];
						i ++;
						new2->edge[i].src = name_number[hash(airports2[j])];
                                                new2->edge[i].dest = name_number[hash(airports1[j])];
                                                new2->edge[i].weight = distance[j];
						j ++;
					}
//					printf("%d, %d\n", airport11, airport22);
					int dis = BellmanFord(new2, airport11, airport22);
					printf("BellmanFord Total Distance: %d\n", dis);
					free(new2->edge);
					free(new2);	
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
			printf("\n");
		}else if (command == 5){
			printf("Entering the test...\n");
			char air1 [200][4];
			char air2 [200][4];
			srand(time(0));
			for(int i = 0; i < 400; i ++){
				int num = rand() % (length_of_airports);
				if (i < 200){
					strcpy(air1[i], names[num]);
					air1[i][4] = '\0';
				}else{
					strcpy(air2[i - 200], names[num]);
					air2[i - 200][4] = '\0';
				}
			}
			int result1[200];
			int result2[200];
			struct timeval tv1, tv2, tv3, tv4;
			double time1, time2;
			gettimeofday(&tv1, NULL);
			for (int i = 0; i < 200; i ++){
				FILE * file2 = fopen(argv[2], "r");
				if (file2 != NULL){
					Graph * new = create_graph(length_of_airports);
					int value; 
					char airport111[4]; 
					char airport222[4];
					while(fscanf(file2, "%s %s %d", airport111, airport222, &value) != EOF) {
						int buffer1 = name_number[hash(airport111)];
						int buffer2 = name_number[hash(airport222)];
        					add(new, buffer1, buffer2, value);
        					add(new, buffer2, buffer1, value);
					}
					fclose(file2);
					Vertex * node = malloc(sizeof(node));
					dijkstra(new, name_number[hash(air1[i])], name_number[hash(air2[i])], length_of_airports, node, names2);
					result1[i] = new->vertex[name_number[hash(air2[i])]]->cost;
					free(node);
					for (int i = 0; i < length_of_airports; i++){
                				if(new->vertex[i]) {
                					for(int j = 0; j < (new->vertex[i])->length; j++){
                        					free((new->vertex[i])->edge[j]);
                					}
                					free((new->vertex[i])->edge);
                				}
                				free(new->vertex[i]);
        				}
        				free(new->vertex);
				        free(new);
				}
			}
			gettimeofday(&tv2, NULL);
			time1 = ((double)(tv2.tv_usec - tv1.tv_usec)) / 1000000 + ((double)(tv2.tv_sec - tv1.tv_sec));
			
			gettimeofday(&tv3, NULL);
                        for (int i = 0; i < 200; i ++){
				FILE * file2 = fopen(argv[2], "r");
                                if (file2 != NULL){
					char airports1 [5000][4];
					char airports2 [5000][4];
					int distance [5000];
					for (int i = 0; i < 5000; i ++){
						airports1[i][4] = '\0';
						airports1[i][4] = '\0';
						distance[i] = -1;
					}	
					int count = 0;
					while(fscanf(file2, "%s %s %d", airports1[count], airports2[count], &distance[count]) != EOF){
						count ++;
					}
					fclose(file2);	
					Graph2 * new2 = create_bellman(length_of_airports, 2 * count);
					int j = 0;
					for (int i = 0; i < count * 2; i ++){
						new2->edge[i].src = name_number[hash(airports1[j])];
						new2->edge[i].dest = name_number[hash(airports2[j])];
						new2->edge[i].weight = distance[j];
						i ++;
						new2->edge[i].src = name_number[hash(airports2[j])];
                                                new2->edge[i].dest = name_number[hash(airports1[j])];
                                                new2->edge[i].weight = distance[j];
						j ++;
					}
					result2[i] = BellmanFord(new2, name_number[hash(air1[i])], name_number[hash(air2[i])]);
					free(new2->edge);
					free(new2);
				}
			}
			gettimeofday(&tv4, NULL);
			time2 = ((double)(tv4.tv_usec - tv3.tv_usec)) / 1000000 + ((double)(tv4.tv_sec - tv3.tv_sec));
			printf("The dijkstra runs %f\n", time1);
			printf("The bellman ford runs %f\n", time2);
			int tr = 0;		
			for(int i = 0; i < 200; i ++){
				if (result1[i] == result2[i])
					tr = 1;
			}
			if (tr)
				printf("correct!\n\n");
		}else
			printf("Never here\n");
		command = ask_command(airport1, airport2);
	}
	printf("Good Bye!\n");
	free(names2);
	return 0;
}
