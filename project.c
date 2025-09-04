#include <stdio.h>
#include <stdlib.h>

// Color constants for DFS
#define WHITE 0  // Unvisited
#define GRAY  1  // Currently being processed
#define BLACK 2  // Finished processing

// Global variables
int **adjacencyMatrix;
int *color;
int *inDegree;
int *currentOrder;
int n;
int hasCycle;
int solutionCount;

// Performs DFS traversal for cycle detection
void dfs(int vertex) {
    color[vertex] = GRAY;
    
    for (int i = 0; i < n; i++) {
        if (adjacencyMatrix[vertex][i] == 1) {
            if (color[i] == GRAY) {
                hasCycle = 1;
                return;
            }
            
            if (color[i] == WHITE) {
                dfs(i);
                if (hasCycle) {
                    return;
                }
            }
        }
    }
    
    color[vertex] = BLACK;
}

// Check if the graph has any cycles
int detectCycle() {
    for (int i = 0; i < n; i++) {
        color[i] = WHITE;
    }
    
    hasCycle = 0;
    
    for (int i = 0; i < n; i++) {
        if (color[i] == WHITE) {
            dfs(i);
            if (hasCycle) {
                return 1;
            }
        }
    }
    
    return 0;
}

// Calculate in-degrees for all vertices
void calculateInDegrees() {
    for (int i = 0; i < n; i++) {
        inDegree[i] = 0;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                inDegree[j]++;
            }
        }
    }
}

// Print current topological order
void printSolution() {
    printf("Solution %d: ", ++solutionCount);
    for (int i = 0; i < n; i++) {
        printf("%d", currentOrder[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

// Backtracking function to find all topological orderings
void findAllTopologicalOrders(int position, int *visited) {
    // If we've placed all vertices, we have a complete ordering
    if (position == n) {
        printSolution();
        return;
    }
    
    // Try placing each unvisited vertex at current position
    for (int vertex = 0; vertex < n; vertex++) {
        // Skip if vertex is already visited
        if (visited[vertex]) {
            continue;
        }
        
        // Check if vertex can be placed at current position
        // (all its predecessors must already be placed)
        int canPlace = 1;
        for (int pred = 0; pred < n; pred++) {
            if (adjacencyMatrix[pred][vertex] == 1 && !visited[pred]) {
                canPlace = 0;
                break;
            }
        }
        
        // If vertex can be placed, place it and recurse
        if (canPlace) {
            currentOrder[position] = vertex;
            visited[vertex] = 1;
            
            // Recursive call for next position
            findAllTopologicalOrders(position + 1, visited);
            
            // Backtrack
            visited[vertex] = 0;
        }
    }
}

// Main function to find all topological orderings
void findAllTopologicalSorts() {
    int *visited = (int*)calloc(n, sizeof(int));
    solutionCount = 0;
    
    printf("\nAll possible topological orderings:\n");
    findAllTopologicalOrders(0, visited);
    
    if (solutionCount == 0) {
        printf("No valid topological ordering found.\n");
    } else {
        printf("\nTotal number of topological orderings: %d\n", solutionCount);
    }
    
    free(visited);
}

// Reads the graph from input file
int readGraph(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    // Read number of vertices
    if (fscanf(file, "%d", &n) != 1) {
        printf("Error: Cannot read number of vertices\n");
        fclose(file);
        return 0;
    }
    
    // Allocate memory for adjacency matrix
    adjacencyMatrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        adjacencyMatrix[i] = (int*)malloc(n * sizeof(int));
    }
    
    // Read adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(file, "%d", &adjacencyMatrix[i][j]) != 1) {
                printf("Error: Cannot read adjacency matrix\n");
                fclose(file);
                return 0;
            }
        }
    }
    
    fclose(file);
    return 1;
}

// Frees allocated memory
void cleanup() {
    // Free adjacency matrix
    for (int i = 0; i < n; i++) {
        free(adjacencyMatrix[i]);
    }
    free(adjacencyMatrix);
    
    // Free other allocated memory
    if (color != NULL) {
        free(color);
    }
    if (inDegree != NULL) {
        free(inDegree);
    }
    if (currentOrder != NULL) {
        free(currentOrder);
    }
}

int main() {
    // Read graph from file
    if (!readGraph("graph.txt")) {
        return 1;
    }
    
    printf("Graph loaded successfully with %d vertices.\n", n);
    
    // Allocate memory for arrays
    color = (int*)malloc(n * sizeof(int));
    inDegree = (int*)malloc(n * sizeof(int));
    currentOrder = (int*)malloc(n * sizeof(int));
    
    // Check for cycles
    if (detectCycle()) {
        printf("Error: The graph contains a cycle. Topological sorting is not possible.\n");
        cleanup();
        return 1;
    }
    
    // Calculate in-degrees
    calculateInDegrees();
    
    // Find all topological orderings
    findAllTopologicalSorts();
    
    // Clean up allocated memory
    cleanup();
    
    return 0;
}

/*
 ALGORITHM EXPLANATION:
 
 This implementation finds ALL possible topological orderings using backtracking:
 
 1. CYCLE DETECTION:
    - First checks if the graph has cycles using DFS
    - Uses three-color system: WHITE, GRAY, BLACK
    - If a back edge is found (GRAY vertex encountered), cycle exists
 
 2. BACKTRACKING ALGORITHM:
    - Tries to place vertices one by one in topological order
    - At each position, considers all vertices that can be placed
    - A vertex can be placed if all its predecessors are already placed
    - Uses recursion to explore all possibilities
    - Backtracks when a complete ordering is found or no valid placement exists
 
 3. CONSTRAINT CHECKING:
    - For each vertex, checks if all incoming edges come from already placed vertices
    - This ensures the topological ordering constraint is maintained
 
 4. SOLUTION GENERATION:
    - When all vertices are placed, prints the current ordering
    - Continues backtracking to find all other possible orderings
 
 TIME COMPLEXITY: O(n! × n²) in worst case (all permutations checked)
 SPACE COMPLEXITY: O(n) for recursion stack and auxiliary arrays
 
 Note: For large graphs, the number of topological orderings can be exponential.
*/