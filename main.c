#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include <unistd.h>     // for sleep()
#include <string.h>

#define INFINITY 200000000

struct Edge {
    int source;
    int destination;
    int weight;
};

struct Digraph {
    int numberOfNodes;
    int numberOfEdges;
    int** graph;
};

void printDigraph(struct Digraph* digraph) {
    printf("number of nodes: %d\n", digraph->numberOfNodes);
    printf("number of edges: %d\n", digraph->numberOfEdges);
    for (int i = 0; i < digraph->numberOfNodes; i++)
    {
        for (int j = 0; j < digraph->numberOfNodes; j++)
        {
            if (digraph->graph[i][j] == INFINITY) {
                printf("_ ");
            }
            else {
                printf("%d ", digraph->graph[i][j]);
            }
        }
        printf("\n");
    }
    
}

struct Digraph* digraphBuilder(int numberOfNodes, int numberOfEdges) {
    struct Digraph* digraph = (struct Digraph*)malloc(sizeof(struct Digraph));
    digraph->numberOfNodes = numberOfNodes;
    digraph->numberOfEdges = numberOfEdges;
    digraph->graph = (int**) malloc(numberOfNodes * sizeof(int*));
    for (int i = 0; i < numberOfNodes; i++) {
        digraph->graph[i] = (int*) malloc(sizeof(int) * numberOfNodes);
    }
    
    return digraph;
}

struct Digraph* digraphCopier(struct Digraph* digraph) {
    struct Digraph* newDigraph = (struct Digraph*)malloc(sizeof(struct Digraph));
    newDigraph->numberOfNodes = digraph->numberOfNodes;
    newDigraph->numberOfEdges = digraph->numberOfEdges;
    newDigraph->graph = (int**) malloc(digraph->numberOfNodes * sizeof(int*));
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        newDigraph->graph[i] = (int*) malloc(sizeof(int) * digraph->numberOfNodes);
    }
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        for (int j = 0; j < digraph->numberOfNodes; j++) {
            newDigraph->graph[i][j] = digraph->graph[i][j];
        }
    }
    return newDigraph; 
}

struct Digraph* readInputFile(char* fileName) {
    int numberOfNodes;
    int numberOfEdges;
    
    FILE *file;
    file = fopen(fileName, "r");
    fscanf(file, "(%d, %d)", &numberOfNodes, &numberOfEdges);
    struct Digraph* digraph = digraphBuilder(numberOfNodes, numberOfEdges);
    for (int i = 0; i < numberOfNodes; i++) {
        for (int  j = 0; j < numberOfNodes; j++) {
            int x;
            fscanf(file, "%d", &x);
            if (x == 0) {
                x = INFINITY;
            }
            digraph->graph[i][j] = x;    
      }
    }
    return digraph;
}

struct DijkstraReturn {
    int source;
    int numberOfNodes;
    int* distance;
    int* predecessor;
};

struct DijkstraReturn* dijkstraReturnBuilder(int source, int numberOfNodes, int* distance, int* predecessor) {
    struct DijkstraReturn* newData = (struct DijkstraReturn*)malloc(sizeof(struct DijkstraReturn));
    newData->source = source;
    newData->numberOfNodes = numberOfNodes;
    newData->distance = distance;
    newData->predecessor = predecessor;
    return newData;
}

void printDijkstra(struct DijkstraReturn* data) {
    for (int i = 0; i < data->numberOfNodes; i++) {
        if (i == data->source) {
            continue;
        }
        printf("(v%d, v%d): ", data->source+1, i+1);

        int parentArray[data->numberOfNodes];
        parentArray[0] = data->predecessor[i];
        int length = 1;
        while (parentArray[length-1] != data->source) {
            parentArray[length] = data->predecessor[parentArray[length-1]];
            length++;
        }
        for (int i = length - 1; i >= 0; i--) {
            printf("v%d->", parentArray[i]+1);
        }
        printf("v%d, %d\n", i+1, data->distance[i]+1);
    }
    printf("\n");
}

int getMinimumEdge(struct Digraph* digraph, int node) {
    int minimum = INFINITY;
    int minimumNode = -1;
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        int edgeValue = digraph->graph[node][i];
        if (edgeValue < minimum && edgeValue != 0) {
            minimum = edgeValue;
            minimumNode = i;
        } 
    }
    return minimum;
}

int getClosestNode(int* array, int n) {
    int minimumNode = -1;
    int shortestDistance = INFINITY;
    for (int i = 0; i < n; i++) {
        if (array[i] == -1) {
            continue;
        }
        if (array[i] < shortestDistance) {
            minimumNode = i;
            shortestDistance = array[i];
        }
    }
    return minimumNode;
}

struct DijkstraReturn* dijkstraArray(struct Digraph* inputDigraph, int source) {
    struct Digraph* digraph = digraphCopier(inputDigraph);
    int* distance = malloc(sizeof(int) * digraph->numberOfNodes);
    int* previous = malloc(sizeof(int) * digraph->numberOfNodes);
    int* nodesToVisit = malloc(sizeof(int) * digraph->numberOfNodes);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        distance[i] = INFINITY;
        previous[i] = -1;
        nodesToVisit[i] = INFINITY;
    } 
    distance[source] = 0;
    nodesToVisit[source] = 0;

    int closestNode = getClosestNode(nodesToVisit, digraph->numberOfNodes);
    nodesToVisit[source] = -1;
    while (closestNode != -1) {
        for (int i = 0; i < digraph->numberOfNodes; i++) {
            if (digraph->graph[closestNode][i] == INFINITY) {
                continue;
            }
            int newPathDistance = distance[closestNode] + digraph->graph[closestNode][i];
            if (newPathDistance < distance[i]) {
                distance[i] = newPathDistance;
                nodesToVisit[i] = newPathDistance;
                previous[i] = closestNode;
            }
        }
        closestNode = getClosestNode(nodesToVisit, digraph->numberOfNodes);
        if (closestNode != -1) {
            nodesToVisit[closestNode] = -1;
        }
    }
    struct DijkstraReturn* returnData = dijkstraReturnBuilder(source, digraph->numberOfNodes, distance, previous);
    return returnData;
}

struct BellmanFordReturn {
    int numberOfNodes;
    int* distances;
};

struct BellmanFordReturn* bellmanFordReturnBuilder(int numberOfNodes) {
    struct BellmanFordReturn * data = (struct BellmanFordReturn*) malloc(sizeof(struct BellmanFordReturn));
    data->numberOfNodes = numberOfNodes;
    data->distances = (int*) malloc(sizeof(int) * numberOfNodes);
    data->distances[0] = 0;
    for (int i = 1; i < numberOfNodes; i++) {
        data->distances[i] = INFINITY;
    }
    return data;
}

void printBellman(struct BellmanFordReturn* data) {
    for (int i = 0; i < data->numberOfNodes; i++) {
        printf("v%d: %d\n", i+1, data->distances[i]);
    }
}

struct BellmanFordReturn* bellmanFord(struct Digraph* digraph) {
    struct BellmanFordReturn* data = bellmanFordReturnBuilder(digraph->numberOfNodes);
    for (int count = 0; count < digraph->numberOfNodes; count++) {
        for (int i = 0; i < digraph->numberOfNodes; i++) {
            for (int j = 0; j < digraph->numberOfNodes; j++) {
                // digraph->graph[i][j];
                // data->distances[i];
                // data->distances[j];
                if (digraph->graph[i][j] + data->distances[i] < data->distances[j]) {
                    data->distances[j] = digraph->graph[i][j] + data->distances[i];
                }
            }
        }
    }
    return data;
}

struct JohnsonReturn {
    struct DijkstraReturn** dijkstra;
    int** realDistances;
};

struct Digraph* createStart(struct Digraph* digraph) {
    struct Digraph* newDigraph = digraphBuilder(digraph->numberOfNodes + 1, digraph->numberOfEdges + digraph->numberOfNodes);
    for (int i = 0; i < newDigraph->numberOfNodes; i++) {
        newDigraph->graph[0][i] = 0;
        newDigraph->graph[i][0] = INFINITY;
    }
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        for (int j = 0; j < digraph->numberOfNodes; j++) {
            newDigraph->graph[i+1][j+1] = digraph->graph[i][j];
        }
    }
    return newDigraph;

}

struct Digraph* reweighGraph(struct Digraph* digraph, struct BellmanFordReturn* data) {
    struct Digraph* newDigraph = digraphBuilder(digraph->numberOfNodes, digraph->numberOfEdges);
    for (int i = 0; i < newDigraph->numberOfNodes; i++) {
        for (int j = 0; j < newDigraph->numberOfNodes; j++) {
            if (digraph->graph[i][j] == INFINITY) {
                newDigraph->graph[i][j] = INFINITY;
            }
            else {
                newDigraph->graph[i][j] = digraph->graph[i][j] + data->distances[i+1] - data->distances[j+1];
            }
        }
    }
    return newDigraph;
}

void johnson(struct Digraph* digraph, char* outputFileName) {
    struct Digraph* newDigraph = createStart(digraph);
    struct BellmanFordReturn* bellmanFordReturn = bellmanFord(newDigraph);
    struct Digraph* noNegativeGraph = reweighGraph(digraph, bellmanFordReturn);

    struct DijkstraReturn** shortestPathData = (struct DijkstraReturn **) malloc(sizeof(struct DijkstraReturn) * digraph->numberOfNodes);
    for (int i = 0; i < noNegativeGraph->numberOfNodes; i++) {
        shortestPathData[i] = dijkstraArray(noNegativeGraph, i);
    }
    
    int numberOfIteratioins = digraph->numberOfNodes;
    if (numberOfIteratioins % 2 == 1) {
        numberOfIteratioins--;
    }

    FILE* fp;
    fp = fopen(outputFileName, "w+");
    fprintf(fp, "All pairs shortest paths:\n");
    for (int i = 0; i < numberOfIteratioins; i++) {
        fprintf(fp, "(v%d, v%d): ", i+1, numberOfIteratioins-i-1+1);

        int parentArray[shortestPathData[i]->numberOfNodes];
        parentArray[0] = numberOfIteratioins-i-1;

        int distance = 0;
        int length = 1;
        while (parentArray[length-1] != i) {
            parentArray[length] = shortestPathData[i]->predecessor[parentArray[length-1]];
            distance += digraph->graph[parentArray[length]][parentArray[length-1]];
            length++;
        }
        for (int i = length - 1; i > 0; i--) {
            fprintf(fp, "v%d->", parentArray[i]+1);
        }
        fprintf(fp, "v%d; %d\n", numberOfIteratioins-i-1+1, distance);
    }


}

int main(int argc, char* argv[]) {
    char* inputFileName = argv[1];
    char* outputFileName = argv[2];
    
    struct Digraph* digraph = readInputFile(inputFileName);

    johnson(digraph, outputFileName);

    // {
    // printf("Dijkstra's algorithm:\n");
    // printf("- Array implementation\n");
    // double time_spent = 0.0;
    // clock_t begin = clock();
    // struct DijkstraReturn* Ddata = dijkstraArray(digraph, 0);
    // clock_t end = clock();
    // time_spent += (double)(end - begin) / CLOCKS_PER_SEC; 
    // printf("Running time: %f\n", time_spent);
    // printDijkstra(Ddata);
    // printf("\n");
    // }

    return 0;
}