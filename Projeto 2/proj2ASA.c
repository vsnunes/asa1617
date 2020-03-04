/* Projeto 2 - Analise e Sintese de Algoritmos
 * Grupo 45
 * 83576 - Vitor Nunes
 * 83417 - Afonso Samora
 */

#include <stdio.h>
#include <stdlib.h>

/*Struct for storing edges */
typedef struct {
	int u;
	int v;
	int w;
	char type; /* (r) road or (a) airport */
}edge_t;

int n_cities, n_airports, n_roads;
edge_t *edges;
int sol_roads, sol_airp, sol_w; /* number of roads and airports */
int *p, *rank; 
int idx; /* the index of edges array */

void Make_Set   (int x);
int  Find_Set   (int x);
void Union      (int x, int y);
void Link       (int x, int y);
int  MST_Kruskal();
int  main       ();

void Make_Set(int x) {
	p[x]    = x;
	rank[x] = 0;
}

int Find_Set(int x) {
	if (x != p[x])
		p[x] = Find_Set(p[x]);
	return p[x];
}

void Union(int x, int y) {
	Link(Find_Set(x), Find_Set(y));
}

void Link(int x, int y) {
	if (rank[x] > rank[y])
		p[y] = x;
	else {
		p[x] = y;
		if (rank[x] == rank[y])
			rank[y] = rank[y] + 1;
	}
	
}

/* Compare function to be used by quicksort in Kruskal */
int compare (const void *a, const void *b) {
	int x = *((int *)a);
	int y = *((int *)b);
	
	return (edges[x].w - edges[y].w);
}

int MST_Kruskal() {
	int total_weight = 0;
	int *order;
	int i,v,e;
	for (v = 1; v <= n_cities; v++) {
		Make_Set(v);
	}

	/* To prevent copy the data of every edge we use an integer 
	array and we sort that array by comparing the weight of every edge
        on the edges array.*/	
	order = (int*)calloc(idx + 1,sizeof(int));
	for (i = 1; i < idx; i++)
		order[i] = i;

	qsort(order, idx, sizeof(int), compare);
	
	for (e = 1; e < idx; e++) {
		if (Find_Set(edges[order[e]].u) != Find_Set(edges[order[e]].v)) {
			total_weight += edges[order[e]].w;
			if (edges[order[e]].type == 'a') sol_airp++;
			else if (edges[order[e]].type == 'r') sol_roads++;
			Union(edges[order[e]].u, edges[order[e]].v);
		}
	}
	
	free(order);
	return total_weight;
}

int main() {
	int i, w, a, b, s;
	int sol_onlyRoads = 0;
	int sol_numberRoads = 0;
	idx = 1;
	edge_t* airp;
	sol_roads = 0;
	sol_airp  = 0;
	s = scanf("%d\n%d", &n_cities, &n_airports);
	if (s == 0) printf("Erro no scanf");
	
	p     = (int*)   calloc(n_cities + 1,sizeof(int));
	rank  = (int*)   calloc(n_cities + 1,sizeof(int));
	/* Just an auxiliar array to run Kruskal only with roads */
	airp  = (edge_t*)calloc(n_airports + 1,sizeof(edge_t));
	
	for (i = 1; i <= n_airports; i++){
		s = scanf("%d %d", &a, &w);
		airp[i].w = w;
		airp[i].u = 0;
		airp[i].v = a;
		airp[i].type = 'a';
		
	}
	
	s = scanf("%d", &n_roads);

	edges = (edge_t*)calloc(n_roads + n_airports + 1, sizeof(edge_t));

	for (i = 0; i < n_roads; i++){
		s = scanf("%d %d %d", &a, &b, &w);
		edges[idx].w = w;
		edges[idx].u = a;
		edges[idx].v = b;
		edges[idx].type = 'r';
		idx++;
	}
	sol_onlyRoads = MST_Kruskal();
	sol_numberRoads = sol_roads;
	

	sol_airp  = 0;
	sol_roads = 0;
	/* Now we copy the airports back to the edges array to run Kruskal to get the minimum cost between roads and airports */
	for (i = 1; i <= n_airports; i++) {
		edges[idx].w = airp[i].w;
		edges[idx].u = 0;
		edges[idx].v = airp[i].v;
		edges[idx].type = 'a';
		idx++;
	}

	sol_w = MST_Kruskal();
	/* If the solution with only roads connects all the cities and has equal cost to the construction of airports then no need for construct airports.*/
	if (sol_onlyRoads <= sol_w && sol_numberRoads >= n_cities - 1) {
		printf("%d\n0 %d\n", sol_onlyRoads, sol_numberRoads);	
	}
	else
	/* Caso insuficiente */
	if (sol_airp + sol_roads >= n_cities)
		printf("%d\n%d %d\n", sol_w, sol_airp, sol_roads);
	else printf("Insuficiente\n");

	free(p);
	free(rank);
	free(edges);
	free(airp);
	return 0;
}
