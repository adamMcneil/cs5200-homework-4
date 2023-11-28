#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include <unistd.h>     // for sleep()
#include <string.h>

int WHITE = 0;
int GRAY = 1;
int BLACK = 2;
int TIME = 0;
int NO_PREDECESSOR = -1;
#define MAX_HEAP_SIZE 100
struct Edge {
    int source;
    int destination;
    int weight;
};

struct ConnectedComponentsData {
    int numberOfComponents;
    int numberOfNodes;
    int* componentArray;
};

struct ConnectedComponentsData* connectedBuilder(int numberOfNodes, struct DepthData* depthData) {
    struct ConnectedComponentsData* data = (struct ConnectedComponentsData*)malloc(sizeof(struct ConnectedComponentsData));
    data->numberOfNodes = numberOfNodes;
    data->componentArray = (int*) malloc(numberOfNodes * sizeof(int));
    for (int i = 0; i < numberOfNodes; i++) {
        data->componentArray[i] = -1;
    }
    for (int i = 0; i < numberOfNodes; i++) {

    }
}

void printConnectComponents(struct ConnectedComponentsData* data) {
    printf("Strongly connected components:");
    printf("# of strongly connect components: %d", data->numberOfComponents);
    for (int i = 0; i < data->numberOfComponents; i++) {
        printf("C%d: ", i);
        for (int n = 0; n < data->numberOfNodes; n++) {
            if (data->componentArray[n] == i) {
                printf("v%d, ", n);
            }
        }
        printf("\n");
    }
}

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
            printf("%d ", digraph->graph[i][j]);
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

struct Digraph* transpose(struct Digraph* digraph) {
    struct Digraph* transposeDigraph = digraphBuilder(digraph->numberOfNodes, digraph->numberOfEdges);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        for (int j = 0; j < digraph->numberOfNodes; j++) {
            transposeDigraph->graph[i][j] = digraph->graph[j][i];
        }
    }
    return transposeDigraph;
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

struct DepthData {
    int numberOfNodes;
    int* discoveryTime;
    int* finishTime;
    int* predecessors;
};

void print(struct DepthData* data) {
    for (int i = 0; i < data->numberOfNodes; i++) {
        printf("%d / %d / %d\n", data->discoveryTime[i], data->finishTime[i], data->predecessors[i]);
    }
}

struct DepthData* depthDataBuilder(int numberOfNodes) {
    struct DepthData* depthData = (struct DepthData*)malloc(sizeof(struct DepthData));
    depthData->discoveryTime = malloc(sizeof(int) * numberOfNodes);
    depthData->finishTime = malloc(sizeof(int) * numberOfNodes);
    depthData->predecessors = malloc(sizeof(int) * numberOfNodes);
    depthData->numberOfNodes = numberOfNodes;
}

void depthSearchVisit(struct Digraph* digraph, int i, struct DepthData* depthData, int* colorArray) {
    TIME++;
    depthData->discoveryTime[i] = TIME;
    colorArray[i] = GRAY;
    for (int j = 0; j < digraph->numberOfNodes; j++) {
        if (digraph->graph[i][j] != 0 && colorArray[j] == WHITE) {
            depthData->predecessors[j] = i;
            depthSearchVisit(digraph, j, depthData, colorArray);
        } 
    }
    colorArray[i] = BLACK;
    TIME++;
    depthData->finishTime[i] = TIME;
}

struct DepthData* depthSearch(struct Digraph* digraph) {
    int* colorArray = malloc(sizeof(int) * digraph->numberOfNodes);
    struct DepthData* depthData = depthDataBuilder(digraph->numberOfNodes);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        colorArray[i] = WHITE;
        depthData->predecessors[i] = NO_PREDECESSOR;
    }

    TIME = 0;
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        if (colorArray[i] == WHITE) {
            depthSearchVisit(digraph, i, depthData, colorArray);    
        }
    }
    return depthData;
}

void depthSearch2Visit(struct Digraph* digraph, int i, struct DepthData* depthData, int* colorArray) {
    TIME++;
    depthData->discoveryTime[i] = TIME;
    colorArray[i] = GRAY;
    for (int j = 0; j < digraph->numberOfNodes; j++) {
        if (digraph->graph[i][j] != 0 && colorArray[j] == WHITE) {
            depthData->predecessors[j] = i;
            depthSearch2Visit(digraph, j, depthData, colorArray);
        } 
    }
    colorArray[i] = BLACK;
    TIME++;
    depthData->finishTime[i] = TIME;
}

struct DepthData* depthSearch2(struct Digraph* digraph) {
    int* colorArray = malloc(sizeof(int) * digraph->numberOfNodes);
    struct DepthData* depthData = depthDataBuilder(digraph->numberOfNodes);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        colorArray[i] = WHITE;
        depthData->predecessors[i] = NO_PREDECESSOR;
    }

    TIME = 0;
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        if (colorArray[i] == WHITE) {
            depthSearch2Visit(digraph, i, depthData, colorArray);    
        }
    }
    return depthData;
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
            digraph->graph[i][j] = x;    
      }
    }
    return digraph;
}

struct ConnectedComponentsData findConnectedComponents(struct Digraph* digraph) {
    struct ConnectedComponentsData* newData;

    struct DepthData* depthDataOne = depthSearch(digraph);
    struct Digraph* digraphTranspose = transpose(digraph);
    struct DepthData* depthDataTwo = depthSearch2(digraphTranspose);
 
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

struct DijkstraReturn* dijkstraArray(struct Digraph* inputDigraph) {
    struct Digraph* digraph = digraphCopier(inputDigraph);
    int* distance = malloc(sizeof(int) * digraph->numberOfNodes);
    int* previous = malloc(sizeof(int) * digraph->numberOfNodes);
    int* nodesToVisit = malloc(sizeof(int) * digraph->numberOfNodes);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        distance[i] = INFINITY;
        previous[i] = -1;
        nodesToVisit[i] = INFINITY;
    } 
    distance[0] = 0;
    nodesToVisit[0] = 0;

    int closestNode = getClosestNode(nodesToVisit, digraph->numberOfNodes);
    nodesToVisit[0] = -1;
    while (closestNode != -1) {
        for (int i = 0; i < digraph->numberOfNodes; i++) {
            if (digraph->graph[closestNode][i] == 0) {
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
    struct DijkstraReturn* returnData = dijkstraReturnBuilder(0, digraph->numberOfNodes, distance, previous);
    return returnData;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int size, int index) {
    int largest = index;  
    int left = 2 * index + 1; 
    int right = 2 * index + 2; 

    if (left < size && arr[left] > arr[largest])
        largest = left;

    if (right < size && arr[right] > arr[largest])
        largest = right;

    if (largest != index) {
        swap(&arr[index], &arr[largest]);
        heapify(arr, size, largest);
    }
}

void insert(int arr[], int *size, int value) {
    if (*size >= MAX_HEAP_SIZE) {
        printf("Heap is full, can't insert more elements.\n");
        return;
    }

    *size = *size + 1;
    int i = *size - 1;
    arr[i] = value;

    while (i != 0 && arr[(i - 1) / 2] < arr[i]) {
        swap(&arr[i], &arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void printHeap(int arr[], int size) {
    printf("Heap: ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

struct DijkstraReturn* dijkstraHeap(struct Digraph* inputDigraph) {
    struct Digraph* digraph = digraphCopier(inputDigraph);
    int* distance = malloc(sizeof(int) * digraph->numberOfNodes);
    int* previous = malloc(sizeof(int) * digraph->numberOfNodes);
    int* nodesToVisit = malloc(sizeof(int) * digraph->numberOfNodes);
    for (int i = 0; i < digraph->numberOfNodes; i++) {
        distance[i] = INFINITY;
        previous[i] = -1;
        nodesToVisit[i] = INFINITY;
    } 
    distance[0] = 0;
    nodesToVisit[0] = 0;

    int closestNode = getClosestNode(nodesToVisit, digraph->numberOfNodes);
    nodesToVisit[0] = -1;
    while (closestNode != -1) {
        for (int i = 0; i < digraph->numberOfNodes; i++) {
            if (digraph->graph[closestNode][i] == 0) {
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
    struct DijkstraReturn* returnData = dijkstraReturnBuilder(0, digraph->numberOfNodes, distance, previous);
    return returnData;
}

int main(int argc, char* argv[]) {
    char* file_name = argv[1];
    
    struct Digraph* digraph = readInputFile(file_name);
    printDigraph(digraph);
    struct DepthData* data = depthSearch(digraph);
    print(data);

    {
    printf("Dijkstra's algorithm:\n");
    printf("- Array implementation\n");
    double time_spent = 0.0;
    clock_t begin = clock();
    struct DijkstraReturn* Ddata = dijkstraArray(digraph);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC; 
    printf("Running time: %f\n", time_spent);
    printDijkstra(Ddata);
    printf("\n");
    }

    {
    printf("- Heap implementation\n");
    double time_spent = 0.0;
    clock_t begin = clock();
    struct DijkstraReturn* Ddata = dijkstraArray(digraph);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC; 
    printf("Running time: %f\n", time_spent);
    printDijkstra(Ddata);
    }

    return 0;
}