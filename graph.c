#include "graph.h"

PriorityHeap * create_heap(int size){
    	PriorityHeap * heap = malloc(sizeof(PriorityHeap));
    	heap->array= malloc(size * sizeof(Node));
	heap->index = malloc(size * sizeof(int));
	heap->count = 0;

    	for (int i = 0; i < size; i++){
       		heap->index[i] = -1;
	}
	return heap;
}


void insert(PriorityHeap * h, int name, int score){
	Node buffer;
        int temp, time, time2;
    	if (h->index[name] == -1){
		Node data = {name, score};
        	h->array[h->count] = data;
        	h->index[name]=h->count;
		time = h->count;
		time2 = (h->count - 1) / 2;
		while((h->array[time]).score < (h->array[time2]).score){
        		temp = h->index[(h->array[time2]).name];
        		h->index[(h->array[time2]).name] = h->index[(h->array[time]).name];
        		h->index[(h->array[time]).name] = temp;
        		buffer = h->array[time2];
        		h->array[time2] = h->array[time];
        		h->array[time] = buffer;
        		time = time2;
        		time2 = (time2 - 1)/2;
		}
        	h->count++;
    	}else{
		if((h->array[h->index[name]]).score > score){
        		(h->array[h->index[name]]).score = score;
			time = h->index[name];
        		time2 = (h->index[name] - 1) / 2;
        		while((h->array[time]).score < (h->array[time2]).score){
        			temp = h->index[(h->array[time2]).name];
     		   		h->index[(h->array[time2]).name] = h->index[(h->array[time]).name];
        			h->index[(h->array[time]).name] = temp;
        				buffer = h->array[time2];
        			h->array[time2] = h->array[time];
        			h->array[time] = buffer;
        			time = time2;
        			time2 = (time2 - 1)/2;
        		}
        	}
    	}
}

int remove_with_min(PriorityHeap * h){
    	Node min = h->array[0];
    	h->index[min.name] = -1;
    	h->array[0] = h->array[h->count-1];
    	h->count--;
    	move(h, 0);
    	return min.name;
}
                                                            
Graph * create_graph(int num){
	Graph * new = malloc(sizeof(Graph));
	new->vertex = calloc(num, sizeof(Vertex));
	return new;
}

void add(Graph * g, int airport1, int airport2, int value){
	if (g->vertex[airport1] == NULL){
		g->vertex[airport1] = malloc(sizeof(Vertex));
		(g->vertex[airport1])->length = 0;
		(g->vertex[airport1])->cost = INT_MAX;
		(g->vertex[airport1])->visited = 0;
		(g->vertex[airport1])->prev = 0;
		(g->vertex[airport1])->next = -1;
		(g->vertex[airport1])->edge = malloc(1000 * sizeof(Edge *));
	}
	if (g->vertex[airport2] == NULL){
                g->vertex[airport2] = malloc(sizeof(Vertex));
                (g->vertex[airport2])->length = 0;
		(g->vertex[airport2])->cost = INT_MAX;
                (g->vertex[airport2])->visited = 0;
                (g->vertex[airport2])->prev = 0;
                (g->vertex[airport2])->next = -1;
		(g->vertex[airport2])->edge = malloc(1000 * sizeof(Edge *));
        }
//	printf("here\n");
	Vertex * start = g->vertex[airport1];
	for (int i = 0; i < start->length; i++){
        	if ((start->edge[i])->vertex == airport2){
			return;
		}
	}
//	printf("here\n");
    	Edge * edge = malloc(sizeof(Edge));
    	edge->vertex = airport2;
    	edge->value = value;
    	start->edge[start->length ++] = edge;
}

void dijkstra(Graph * g, int airport1, int airport2, int length, Vertex * node, char ** name){
//	printf("here\n");
	Vertex * current = g->vertex[airport1];
//	if(current == NULL)
//		printf("here\n");
//	printf("%d\n", current->cost);
	current->cost = 0;
//	printf("here\n");
	PriorityHeap * heap = create_heap(length);
//	printf("here\n");
	insert(heap, airport1, 0);
//	printf("here\n");
	while (heap->count != 0){
		int v = remove_with_min(heap);
		current = g->vertex[v];
		if (v == airport2){
			break;
		}else if(current->visited == 1){
			continue;
		}else{
			current->visited = 1;
			for(int i = 0; i < current->length; i ++){
				Edge * edge = current->edge[i];
				Vertex * next = g->vertex[edge->vertex];
				if (next->visited != 1){
					if ((current->cost + edge->value) <= next->cost){
						next->cost = current->cost + edge->value;
						next->prev = v;
						insert(heap, edge->vertex, next->cost);
					}
				}
			}

		}
	}
   	node = g->vertex[airport2];
    	while(node->cost != 0){
		node = g->vertex[node->prev];
        	for(int i = 0; i < node->length; i++){
            	if (node->edge[i]->vertex == airport2){
                	node->next = i;
                	airport2 = g->vertex[node->edge[i]->vertex]->prev;
            		}
        	}
    	}
	Vertex * next;
	while(node->next != -1){
        	next = g->vertex[node->edge[node->next]->vertex];
        	printf("%s->%s:%d\n", name[next->prev], name[node->edge[node->next]->vertex], node->edge[node->next]->value);
		node = next;
    	}
	free(heap->index);
	free(heap->array);
	free(heap);
}

void move(PriorityHeap * heap, int count){
    	int left = count * 2 + 1;
    	int right = left + 1;
    	int min = count;
	int tmp;
    	Node buffer;
    	if (left < heap->count && (heap->array[left]).score < (heap->array[count]).score)
        	min = left;
    	if (right < heap->count && (heap->array[right]).score < (heap->array[min]).score)
        	min = right;
    	if(min != count){
        	tmp = heap->index[(heap->array[min]).name];
        	heap->index[(heap->array[min]).name] = heap->index[(heap->array[count]).name];
        	heap->index[(heap->array[count]).name] = tmp;
        	buffer = heap->array[min];
        	heap->array[min] = heap->array[count];
        	heap->array[count] = buffer;
        	move(heap, min);
    	}
}

Graph2 * create_bellman(int v, int e){
	Graph2 * g = malloc(sizeof(Graph2));
	g->V = v;
	g->E = e;
	g->edge = calloc(e, sizeof(Edge2));

	return g; 	
}

void printArr(int dist[], int n) 
{ 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < n; ++i) 
        printf("%d \t\t %d\n", i, dist[i]); 
}

int BellmanFord(Graph2 * g, int airport1, int airport2){
	int V = g->V; 
    	int E = g->E; 
    	int dist[V]; 
	for (int i = 0; i < V; i++){ 
        	dist[i] = INT_MAX; 
	}
    	dist[airport1] = 0;
	for (int i = 1; i <= V - 1; i++){ 
        	for (int j = 0; j < E; j++){ 
            		int u = g->edge[j].src; 
            		int v = g->edge[j].dest; 
            		int weight = g->edge[j].weight; 
            		if(dist[u] != INT_MAX && dist[u] + weight < dist[v]) 
                		dist[v] = dist[u] + weight; 
        	}	 
    	}

	return dist[airport2];
}






