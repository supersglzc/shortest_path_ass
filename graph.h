#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

typedef struct node{
  	int name;
  	int score;
}Node;

typedef struct priority_heap{
	Node * array;
  	int * index;
  	int count;
}PriorityHeap;

typedef struct edge{
    	int vertex;
    	int value;
}Edge;

typedef struct vertex{
    	Edge ** edge;
    	int length;
	int visited;
	int next;
    	int prev;
	int cost;
}Vertex;

typedef struct graph{
    	Vertex ** vertex;
}Graph;


typedef struct edge2{
	int src;
	int dest;
	int weight;
}Edge2;

typedef struct graph2{
	int V, E;
	Edge2 * edge;
}Graph2;

PriorityHeap * create_heap(int size);
void insert(PriorityHeap * heap, int name, int score);
void move(PriorityHeap * heap, int index);
int remove_with_min(PriorityHeap * h);
Graph * create_graph(int num);
void add(Graph * g, int airport1, int airport2, int value);
void dijkstra (Graph * g, int airport1, int airport2, int length, Vertex * node, char ** name);
Graph2 * create_bellman(int v, int e);
int BellmanFord(Graph2 * g, int airport1, int airport2);


