/* matrices.h */

//function that prints the matrix
void pmatrix(int sz, int matrix[sz][sz]);

//function that parses the cost matrix 
void init_matrix(int N, int matrix[N][N], char *argv, int *flag);

//dijkstra's algorithm implementation
void dijkstra(int graph[V][V], int src);

