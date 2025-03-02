#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_LOCATIONS 10

// Min-Heap implementation using a Priority Queue
typedef struct {
    int vertex, distance;
} MinHeapNode;

typedef struct {
    MinHeapNode heap[MAX_LOCATIONS];
    int size;
} MinHeap;

// Function to swap two heap nodes
void swap(MinHeapNode *a, MinHeapNode *b) {
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify the node at index i
void minHeapify(MinHeap *minHeap, int i) {
    int smallest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < minHeap->size && minHeap->heap[left].distance < minHeap->heap[smallest].distance)
        smallest = left;
    if (right < minHeap->size && minHeap->heap[right].distance < minHeap->heap[smallest].distance)
        smallest = right;
    if (smallest != i) {
        swap(&minHeap->heap[i], &minHeap->heap[smallest]);
        minHeapify(minHeap, smallest);
    }
}

// Function to extract the minimum node from heap
MinHeapNode extractMin(MinHeap *minHeap) {
    MinHeapNode root = minHeap->heap[0];
    minHeap->heap[0] = minHeap->heap[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return root;
}

// Function to insert a new node in the heap
void insertMinHeap(MinHeap *minHeap, int v, int dist) {
    int i = minHeap->size++;
    minHeap->heap[i].vertex = v;
    minHeap->heap[i].distance = dist;
    while (i && minHeap->heap[i].distance < minHeap->heap[(i - 1) / 2].distance) {
        swap(&minHeap->heap[i], &minHeap->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to print the shortest path
void printPath(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%d ", j);
        return;
    }
    printPath(parent, parent[j]);
    printf("-> %d ", j);
}

// Optimized Dijkstra's Algorithm using Min-Heap
void dijkstra(int graph[MAX_LOCATIONS][MAX_LOCATIONS], int src, int n) {
    int dist[MAX_LOCATIONS]; // Shortest distances
    bool visited[MAX_LOCATIONS] = {0}; // Visited nodes
    int parent[MAX_LOCATIONS]; // Path tracking
    MinHeap minHeap = { .size = 0 };

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    dist[src] = 0;
    insertMinHeap(&minHeap, src, 0);

    while (minHeap.size) {
        MinHeapNode node = extractMin(&minHeap);
        int u = node.vertex;
        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] && !visited[v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
                insertMinHeap(&minHeap, v, dist[v]);
            }
        }
    }

    printf("Location\tDistance from Source\tPath\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i, dist[i]);
        printPath(parent, i);
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter number of locations: ");
    scanf("%d", &n);

    int graph[MAX_LOCATIONS][MAX_LOCATIONS];
    printf("Enter adjacency matrix (use 0 for no direct connection):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    int src;
    printf("Enter the source location index: ");
    scanf("%d", &src);

    dijkstra(graph, src, n);
    return 0;
}
