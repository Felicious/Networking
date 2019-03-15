#include <stdlib.h>
#include <limits.h>
#include "map.h"
#include "nier.h"

extern pthread_mutex_t lock;

int find_shortest(int *paths, int *visited)
{
  int min = INT_MAX;
  int index_min;

  //find min distance between each node
  for (int i = 0; i < 4; ++i) {
        if (visited[i] == 0 && paths[i] <= min) {
            min = paths[i];
            index_min = i;
        }
  }

  return index_min;
}

// Dijkstra's shortest path algorithm using adjacency Map 
//used to get the least costs Map from neighbors
//returns the least cost array 
int* nier(int access_pt) 
{ 
    //lock Map involved 
    pthread_mutex_lock(&lock);

    //make a new cost table that contains the least dist
    int *paths = (int*) malloc(4 * sizeof(int));
    // initialize to max value
    for (int i = 0; i < 4; i++){
      paths[i] = INT_MAX;
    }

    //mark all as unvisited initially
    int visited[4] = {0};

    // distance to itself is always 0
    paths[access_pt] = 0;

    for (int i = 0; i < 4; ++i) {
        int index = find_shortest(paths, visited);
        visited[index] = 1;

        for (int j = 0; j < 4; ++j) {
            if (!visited[j]            // have not visited
                && map_database[index][j] && paths[index] < 10000 // is a neighbor
                // distance to closest + curr node is less than the curr node
                && paths[index] + map_database[index][j] < paths[j]) {
                    paths[j] = paths[index] + map_database[index][j];
            }
        }
    }

    pthread_mutex_unlock(&lock);

    return paths;
}

//update 