#include <stdio.h>
#include <stdlib.h>

#define INFINITY -1

typedef struct node {
	int vertex;
	struct node *next;
	
}node_t;

/**
 * Struct for storage adjacent vertexs.
 * */
typedef struct adjList {
	node_t *head;
	
}list_t;

int* d;         /*discover times*/ 
int* f;         /*finish time*/
int* result;    /* final result to be displayed */

list_t* edges;  /*adjacent list of vertexs*/
int vertexs;    /*number of vertexs. */
int time;       /* dfs time */
int indexs;     /* index of result vector */
unsigned int flag, flagDFS;

/** List of functions **/
node_t* newNode(int vertex);
void insertEdge(int v1, int v2);
void DFS();
void DFS_Visit(int v);
int main();

node_t* newNode(int vertex) {
	node_t* newn = (node_t*) malloc(sizeof(node_t));
	newn->vertex = vertex;
	newn->next   = NULL;
	return newn;
}

void insertEdge(int v1, int v2) {
	node_t* newnode    = newNode(v2);
	newnode->next      = edges[v1 - 1].head;
	edges[v1 - 1].head = newnode;
}

void DFS_Visit(int v) {
	d[v] = time;
	time++;
	node_t* n = edges[v].head;
	while(n != NULL) {
		if (d[n->vertex - 1] == INFINITY)
			DFS_Visit(n->vertex - 1);
		else if (f[n->vertex - 1] == INFINITY){
			if (!flagDFS)
				printf("Incoerente\n");
			flagDFS = 1;
			return;
		}
		n = n->next;
	}
	f[v] = time;
	result[--indexs] = v + 1;
	time++;
	
}

void DFS() {
	int i;
	time = 1;
	for (i = 0; i < vertexs; i++) {
		if (d[i] == INFINITY)
			DFS_Visit(i);
	}
	
}


int main() {
	vertexs = 0;
	int i;
	int v, e, edge1, edge2;
	int last;
	node_t* t;
	
	/*Input for vertexs and edges*/
	if (scanf("%d %d", &v, &e) == EOF)
		printf("Erro na leitura");
	
	d      =  (int*)    malloc(v * sizeof(int));
	f      =  (int*)    malloc(v * sizeof(int));
	edges  =  (list_t*) malloc(v * sizeof(list_t));
	result =  (int*)    malloc(v * sizeof(int));
	
	indexs = v;
	flagDFS = 0;

	for (i = 0; i < v; i++){
		edges[i].head = NULL;
		d[vertexs]   = INFINITY;
		f[vertexs++] = INFINITY;
	}
	
	/*Input for edges*/
	for (i = 0; i < e; i++){
		if (scanf("%d %d", &edge1, &edge2) == EOF)
			printf("Erro na leitura");
		insertEdge(edge1, edge2);
	}
	
	DFS();
		
	/* Caso insuficiente */
	if (!flagDFS) {
		for (i = 0; i < vertexs - 1; i++) {
			last = result[i+1];
			t = edges[result[i] - 1].head;
			flag = 0;
			/* Check if exists an edge between two consecutive vertexs */
			while(t != NULL) {
				if (t->vertex == last) {
					flag = 1;
					break;
				}
				t = t->next;
			}
			if (!flag){
				printf("Insuficiente\n");
				break;
			}		
		}
		
		if (flag) {
			for (i = 0; i < vertexs; i++){
				if (i < vertexs - 1)
					printf("%d ", result[i]);
				else printf("%d", result[i]);
			}
			printf("\n");
		}	
	}
	
	
	free(result);
	free(d);
	free(f);
	
	node_t *n;
	node_t *nxt;
	for (i = 0; i < vertexs; i++)
	{
		n = edges[i].head;
		while (n != NULL) {
			nxt = n->next;
			free(n);
			n = nxt;
		}
	}
	
	free(edges);
	
	return 0;
}
