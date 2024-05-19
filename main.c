// QOSSAY RIDA 1211553      section: 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZEDATA 30

// Definition of a Graph data structure
typedef struct graph* Graph;
typedef struct graph* Vertex;
typedef struct edge* Edge;

// Definition of a Queue data structure
typedef struct queue* Queue;
typedef struct nodeQueue* NodeQueue;

// Definition of a NodeList data structure
typedef struct nodeList* List;
typedef struct nodeList* NodeList;

// Definition of a NodeList data structure
typedef struct nodeList2* List2;
typedef struct nodeList2* NodeList2;

// Definition of a MinHeap data structure
typedef struct heap* MinHeap;

//  Functions for Read & Write in file
Graph ReadFile();
void WriteFile(List Res1,List Res2 ,int Dis1,int Dis2,int index);

//  Functions for Graph
Graph MakeGraph();
Vertex InsertVertex (Graph graph,char City[]);
Vertex Find (Graph graph,char City[]);
int SetDefault (Graph graph);
void PrintGraph(Graph graph);
void AddEdge(Graph graph,char City1[],char City2[], int weight);

//  Functions for Queue
Queue CreateQueue();
void Enqueue(Queue queue,Vertex Data);
Vertex Dequeue(Queue queue);

//  Functions for Linked List
List CreateList();
void InsertToList(List list, char City[], int Distance);
void PrintList(List list);
void fPrintList(FILE* fp,List list);
int NumberOfElement(List list);

//  Functions for Linked List2
List2 CreateList2();
void InsertToList2 (List2 list, List L1,List L2, int num1,int num2);

// Functions for Heap
MinHeap CreateMinHeap(int capacity);
void InsertToHeap(MinHeap heap,Vertex Value);
Vertex ExtractMin(MinHeap heap);
void HeapUp(MinHeap heap,int index);
void HeapDown(MinHeap heap,int index);
int FindIndex(MinHeap heap,Vertex Value);
void EditElement(MinHeap heap,int index,int newValue);

// Functions for BFS  &&  Dijkstra
void BFS (Graph graph,char City1[],char City2[]);
void Dijkstra (Graph graph,char City1[]);
List FindRoad (Graph graph,char Destination[]);

struct graph {
    char City[SIZEDATA];        // Name of the city represented by this graph node
    Edge Edge;                  // Pointer to the first edge connected to this vertex
    Vertex Next;                // Pointer to the next vertex in the graph
    // This field for apply BFS and Dijkstra algorithm
    char HowToArrive[SIZEDATA]; // Information on how to arrive at this city
    int Visited;                // Flag to mark if the vertex has been visited during a traversal
    int Distance;               // Distance from the source vertex during traversal
};

struct edge {
    char City[SIZEDATA];  // Name of the city represented by this edge
    int Weight;           // Weight/cost of traveling to the destination city
    Edge Next;            // Pointer to the next edge connected to the same vertex
};

struct nodeList {
    char City[SIZEDATA];  // Name of the city represented by this node
    int Distance;         // Distance from the source node to this node
    NodeList Next;        // Pointer to the next node in the list
};

struct nodeList2 {
    List ResultBFS;         // result for BFS
    List ResultDikkstra;    // result for Dijkstra
    int TotalBFS;           // Total Distance for BFS
    int TotalDijkstra;      // Total Distance for Dijkstra
    NodeList2 Next;         // Pointer to the next node in the list
};

struct queue {
    NodeQueue Front;  // Pointer to the front of the queue
    NodeQueue Rear;   // Pointer to the rear of the queue
};

struct nodeQueue {
    Vertex Data;       // Data of the node stored in the queue
    NodeQueue Next;    // Pointer to the next node in the queue
};

struct heap {
    Vertex* Data;      // Array of Vertex pointers representing the heap
    int Capacity;      // Maximum capacity of the heap
    int Size;          // Current number of elements in the heap
};

int main() {
    printf("*********** Welcome to our application of calculating the shortest road between cities ***********\n");
    Graph graph = NULL;     // Create graph to fill it from file in first operation
    char Source[SIZEDATA] = "", Destination[SIZEDATA];  // Create two string for source and destination city
    List ResultBFS, ResultDijkstra;     // Declare two inked list to save result for each algorithm
    int TotalDistanceByBFS, TotalDistanceByDijkstra;    // Declare two integer to save the length of road
    List2 Results = CreateList2();
    int operation = 0;

    while (operation != 5) {     // The menu loop for the application.
        // Print menu for user to choose operation
        printf("\nPlease choose operation:\n");
        printf("1- Load cities from file\n");
        printf("2- Print Graph\n");
        printf("3- Enter source city\n");
        printf("4- Enter destination city and print road\n");
        printf("5- Exit and print information in file\n");
        scanf("%d", &operation);    // Read operation from user

        if (operation == 1) {    // Operation1 to load cities from a file and create the graph.
            graph = ReadFile(); // Reads data from a file and creates the graph.
            printf("Done\n");
        }
        else if (operation == 2) {  // Operation2 to print the current state of the graph.
            if (graph == NULL || graph->Next == NULL)   // Check if graph has vertex to print
                printf("Empty graph please load cities\n");
            else
                PrintGraph(graph); // PrintGraph function is implemented to print the graph.
        }
        else if (operation == 3) {  // Operation3 to enter the source city.
            printf("Please enter source city:\n");
            scanf("%s", Source);    // Read source city from user
            if (Find(graph, Source) == NULL)    // Search for source city if exist in graph
                printf("This city does not exist in the graph. Please try again.\n");
        }
        else if (operation == 4) {  // Operation4 to enter the destination city and print the shortest road.
            if (Find(graph, Source) == NULL)    // Check if source city valid
                printf("Source city has wrong value\n");
            else {  // If source city valid read destination city
                printf("Please enter destination city:\n");
                scanf("%s", Destination);   // Read destination city from user

                while (Find(graph, Destination) == NULL) {  // Loop to ensure the destination city exists in the graph.
                    printf("This city does not exist in the graph. Please enter a new city:\n");
                    scanf("%s", Destination);   // Read destination city again
                }

                BFS(graph, Source, Destination); // Calculates the shortest path using BFS algorithm.
                ResultBFS = FindRoad(graph,Destination); // After apply BFS algorithm store shortest path in list
                TotalDistanceByBFS = Find(graph,Destination)->Distance; // FindFromGraph the length of road

                Dijkstra(graph, Source); // Calculate the shortest path using Dijkstra algorithm.
                ResultDijkstra = FindRoad(graph, Destination);  // After apply Dijkstra algorithm store shortest path in list
                TotalDistanceByDijkstra = Find(graph, Destination)->Distance;   // FindFromGraph the length of road

                // Print the results of both algorithms.
                printf("\nThe result using BFS algorithm\nTotal Distance= %d\n", TotalDistanceByBFS);   // Print total length
                printf("Number of edges= %d\n", NumberOfElement(ResultBFS) - 1); // Number of edge equal number of city - 1
                PrintList(ResultBFS); // Prints the road obtained by BFS.

                printf("\nThe result using Dijkstra's algorithm\nTotal Distance= %d\n", TotalDistanceByDijkstra);   // Print total length
                printf("Number of edges= %d\n", NumberOfElement(ResultDijkstra) - 1);   // Number of edge equal number of city - 1
                PrintList(ResultDijkstra); // Prints the road obtained by Dijkstra's algorithm.

                InsertToList2(Results,ResultBFS,ResultDijkstra,TotalDistanceByBFS,TotalDistanceByDijkstra);
            }
        }
        else if (operation == 5) {  // Operation5 to exit and print information to a file.
            FILE* fp = fopen("shortest_distance.txt", "w"); // to Remove all data in file
            fclose(fp);
            NodeList2 Temp = Results->Next;
            int index=1;
            while (Temp!=NULL){
                WriteFile(Temp->ResultBFS,Temp->ResultDikkstra,Temp->TotalBFS,Temp->TotalDijkstra,index);  // write result in file
                index++;
                Temp=Temp->Next;
            }
            printf("Good Bay");
        }
        else
            printf("You choose an invalid operation. Please try again.\n");  // If choose invalid operation
    }
    return 0;
}

//****************************************************************************************
//                           Functions for Read & Write in file
//****************************************************************************************

Graph ReadFile() {
    Graph graph = MakeGraph();  // Create an empty graph

    FILE* fp = fopen("cities.txt", "r");  // Open the file "cities.txt" in read mode
    if (fp == NULL)
        return graph;  // If the file cannot be opened, return the empty graph

    char City1[SIZEDATA], City2[SIZEDATA], Temp[SIZEDATA];
    int State = fscanf(fp, "%s", City1);  // Read the first city name from the file
    while (State != EOF) {
        fscanf(fp, "%s", City2);  // Read the second city name from the file
        fscanf(fp, "%s", Temp);  // Read the distance as a string from the file

        int Distance = 0;
        for (int i = 0; Temp[i] >= '0' && Temp[i] <= '9'; i++)
            Distance = Distance * 10 + (Temp[i] - '0');  // Convert the distance string to an integer

        AddEdge(graph, City1, City2, Distance);  // Add an edge between the two cities with the given distance
        State = fscanf(fp, "%s", City1);  // Read the next city name from the file
    }

    fclose(fp);  // Close the file
    return graph;  // Return the populated graph
}

void WriteFile(List ResultBFS, List ResultDijkstra, int TotalDistanceByBFS, int TotalDistanceByDijkstra,int index) {
    FILE* fp = fopen("shortest_distance.txt", "a");  // Open a file named "shortest_distance.txt" in write mode

    fprintf(fp, "Result %d:\n", index);
    // Write BFS algorithm result to the file
    fprintf(fp, "The result using BFS algorithm\nTotal Distance = %d\n", TotalDistanceByBFS);  // Write total distance for BFS result
    fprintf(fp, "Number of edges = %d\n", NumberOfElement(ResultBFS) - 1);  // Write the number of edges in the BFS result
    fPrintList(fp, ResultBFS);  // Write the BFS result list to the file

    // Write Dijkstra algorithm result to the file
    fprintf(fp, "\nThe result using Dijkstra algorithm\nTotal Distance = %d\n", TotalDistanceByDijkstra);  // Write total distance for Dijkstra result
    fprintf(fp, "Number of edges = %d\n", NumberOfElement(ResultDijkstra) - 1);  // Write the number of edges in the Dijkstra result
    fPrintList(fp, ResultDijkstra);  // Write the Dijkstra result list to the file

    fclose(fp);  // Close the file after writing all the data
}

//****************************************************************************************
//                                  Functions for Graph
//****************************************************************************************

Graph MakeGraph() {
    Graph graph = (Graph)malloc(sizeof(struct graph));  // Allocate memory for the Graph structure
    if (graph == NULL)  // Check if graph Allocated in memory
        printf("Out of space");

    graph->Next = NULL;  // Set the Next pointer of the Graph to NULL

    return graph;  // Return the created Graph
}

Vertex InsertVertex(Graph graph,char City[]) {
    Vertex newNode = (Vertex)malloc(sizeof(struct graph));  // Allocate memory for a new Vertex
    newNode->Edge = NULL;  // Set the Edge pointer of the new Vertex to NULL
    newNode->Visited = 0;  // Set the Visited flag of the new Vertex to 0
    newNode->Distance = -1;  // Set the Distance of the new Vertex to -1
    strcpy(newNode->City, City);  // Copy the Data string into the new Vertex
    strcpy(newNode->HowToArrive,"");  // Make HowToArrive null
    newNode->Next = graph->Next;  // Update the Next pointer of the new Vertex
    graph->Next = newNode;  // Update the Next pointer of the Graph to point to the new Vertex

    return newNode;  // Return the newly inserted Vertex
}

Vertex Find(Graph graph,char City[]) {
    Vertex temp = graph->Next;  // Declare pointer to visit vertex

    while (temp != NULL && strcmp(City, temp->City) != 0)   // Check vertex
        temp = temp->Next;  // Update pointer

    return temp;  // Return the found Vertex (or NULL if not found)
}

int SetDefault(Graph graph) {
    Vertex temp = graph->Next;  // Declare pointer to visit vertex
    int count = 0;

    while (temp != NULL) {
        temp->Visited = 0;  // Reset the Visited flag of the Vertex to 0
        temp->Distance = -1;  // Reset the Distance of the Vertex to -1
        strcpy(temp->HowToArrive,"");  // Make HowToArrive null
        temp = temp->Next;
        count++;
    }
    return count;  // Return the number of Vertices in the graph
}

void PrintGraph(Graph graph) {
    Vertex temp = graph->Next;  // Start from the first vertex in the graph

    while (temp != NULL) {  // Traverse each vertex in the graph
        printf("Adjacency list of vertex %s\n", temp->City);  // Print the label of the current vertex

        Edge current = temp->Edge;  // Start from the first edge of the current vertex

        for (int i = 1; current != NULL; i++) {  // Traverse each edge in the adjacency list of the vertex
            printf("%d- %s (Weight: %d)\n", i, current->City, current->Weight);  // Print the edge information
            current = current->Next;  // Move to the next edge in the list
        }

        printf("\n");  // Print empty line for better readability
        temp = temp->Next;  // Move to the next vertex in the graph
    }
}

void AddEdge(Graph graph, char City1[], char City2[], int Weight) {
    Vertex V1 = Find(graph,City1), V2 = Find(graph,City2); // Search cities in the graph

    if (V1 == NULL)
        V1 = InsertVertex(graph,City1);  // Insert the first vertex if not found
    if (V2 == NULL)
        InsertVertex(graph,City2);  // Insert the second vertex if not found

    Edge Edge1 = (Edge)malloc(sizeof(struct edge));  // Allocate memory for a new Edge
    Edge1->Next = V1->Edge;  // Update the Next pointer of the new Edge
    V1->Edge = Edge1;  // Update the Edge pointer of V1 to the new Edge
    Edge1->Weight = Weight;  // Set the Weight of the new Edge
    strcpy(Edge1->City, City2);  // Copy the Data string into the new Edge
}

//****************************************************************************************
//                                  Functions for Queue
//****************************************************************************************

Queue CreateQueue() {
    Queue q = (Queue)malloc(sizeof(struct queue));  // Allocate memory for the Queue structure
    q->Front = q->Rear = NULL;  // Set both Front and Rear pointers to NULL
    return q;  // Return the created Queue
}

void Enqueue(Queue queue,Vertex Data) {
    NodeQueue temp = (NodeQueue)malloc(sizeof(struct nodeQueue));  // Allocate memory for a new Node
    temp->Data = Data;
    temp->Next = NULL;  // Set the Next pointer of the new Node to NULL

    if (queue->Front == NULL) {
        queue->Front = queue->Rear = temp;  // If the queue is empty, make the new Node both the Front and Rear
        return;
    }

    queue->Rear->Next = temp;  // Update the Next pointer of the current Rear to point to the new Node
    queue->Rear = temp;  // Update the Rear pointer to the new Node
}

Vertex Dequeue(Queue queue) {
    if (queue->Front == NULL)
        printf("Empty Queue \n");

    NodeQueue temp = queue->Front;  // Store the front Node of the queue
    queue->Front = queue->Front->Next;  // Update the Front pointer to the next Node in the queue

    if (queue->Front == NULL)
        queue->Rear = NULL;  // If the queue becomes empty, update the Rear pointer to NULL

    Vertex Data = temp->Data ; // Get the Distance from the dequeued Node
    free(temp);  // Free the memory of the dequeued Node

    return Data;  // Return the Distance of the dequeued Node
}

//****************************************************************************************
//                               Functions for Linked List
//****************************************************************************************

List CreateList(){
    List list = (List)malloc(sizeof(struct nodeList));  // Allocate memory for the Linked List structure
    if( list == NULL )
        printf("Out of space!");
    list->Next = NULL;  // Set the Next pointer of the List to NULL
    return list;  // Return the created List
}

void InsertToList (List list, char City[], int Distance){   // Add node at the beginning of linked list
    NodeList temp = (NodeList)malloc(sizeof(struct nodeList));  // Allocate memory for a new Node
    if( temp == NULL)
        printf("Out of space!");
    else{
        strcpy(temp->City, City);  // Copy the Data string into the new Node
        temp->Distance = Distance;  // Set the Distance of the new Node
        temp->Next = list->Next;  // Set the Next pointer of the new Node to the current top of the list
        list->Next = temp;  // Set the Next pointer of the list to the new Node
    }
}

void PrintList(List list) {
    NodeList Temp = list->Next;  // Create a temporary pointer 'Temp' to traverse the list
    int Distance1 = Temp->Distance;  // Variable to store the distance of the current node
    int Distance2;  // Variable to store the distance of the next node

    while (Temp != NULL && Temp->Next != NULL) {  // Traverse the list until it reaches the end (NULL)
        printf("%s", Temp->City);  // Print the city name of the current node
        Distance2 = Temp->Next->Distance;  // Get the distance of the next node
        printf(" --%d--> ", Distance2 - Distance1);  // Print the difference in distances between the current and next nodes
        Distance1 = Distance2;  // Update Distance1 with the distance of the next node
        Temp = Temp->Next;  // Move to the next node in the list
    }

    printf("%s\n", Temp->City);  // Print the city name of the last (or the only) node in the list
}

void fPrintList(FILE* fp,List list){
    NodeList Temp = list->Next;  // Create a temporary pointer 'Temp' to traverse the list
    int Distance1 = Temp->Distance;  // Variable to store the distance of the current node
    int Distance2;  // Variable to store the distance of the next node

    while (Temp != NULL && Temp->Next != NULL) {  // Traverse the list until it reaches the end (NULL)
        fprintf(fp,"%s", Temp->City);  // Print the city name of the current node
        Distance2 = Temp->Next->Distance;  // Get the distance of the next node
        fprintf(fp," --%d--> ", Distance2 - Distance1);  // Print the difference in distances between the current and next nodes
        Distance1 = Distance2;  // Update Distance1 with the distance of the next node
        Temp = Temp->Next;  // Move to the next node in the list
    }

    fprintf(fp,"%s\n\n", Temp->City);  // Print the city name of the last (or the only) node in the list
}

int NumberOfElement(List list) {
    int i = 0;  // Initialize a variable 'i' to count the number of elements in the list
    NodeList temp = list->Next;  // Create a temporary pointer 'temp' to traverse the list

    while (temp != NULL) {  // Traverse the list until the end (NULL) is reached
        i++;  // Increment the count for each element encountered
        temp = temp->Next;  // Move to the next element in the list
    }

    return i;  // Return the total number of elements in the list
}

//****************************************************************************************
//                               Functions for Linked List2
//****************************************************************************************

List2 CreateList2(){
    List2 list = (List2)malloc(sizeof(struct nodeList2));  // Allocate memory for the Linked List structure
    if( list == NULL )
        printf("Out of space!");
    list->Next = NULL;  // Set the Next pointer of the List to NULL
    return list;  // Return the created List
}

void InsertToList2 (List2 list, List L1,List L2, int num1,int num2){   // Add node at the beginning of linked list
    NodeList2 temp = (NodeList2)malloc(sizeof(struct nodeList2));  // Allocate memory for a new Node

    NodeList2 flag = list;
    while (flag->Next!=NULL)
        flag = flag->Next;

    temp->ResultBFS=L1;  // Copy the ResultBFS the new Node
    temp->ResultDikkstra =L2;  // Set the ResultDikkstra of the new Node
    temp->TotalBFS =num1;  // Set the Total Distance of the new Node
    temp->TotalDijkstra =num2;  // Set the Total Distance of the new Node
    temp->Next = NULL ; // Set the Next pointer of the new Node to the current top of the list
    flag->Next = temp;  // Set the Next pointer of the list to the new Node
}

//****************************************************************************************
//                                 Functions for Heap
//****************************************************************************************

MinHeap CreateMinHeap(int capacity){
    MinHeap heap = (MinHeap)malloc(sizeof(struct heap));  // Allocate memory for the MinHeap structure
    heap->Capacity = capacity;  // Set the capacity of the heap
    heap->Size = 0;  // Initialize the size of the heap to 0
    heap->Data = (Vertex*)malloc(capacity * sizeof(Vertex));  // Allocate memory for the array that will store the heap elements
    return heap;  // Return the created MinHeap
}

void InsertToHeap(MinHeap heap,Vertex Value) {
    if (heap->Size == heap->Capacity) {  // Check if the heap is full
        printf("Heap is full. Cannot insert any more elements.\n");
        return;
    }

    heap->Data[heap->Size] = Value;  // Insert the new value at the end of the heap array
    HeapUp(heap, heap->Size);  // Call HeapUp to fix the heap property
    heap->Size++;  // Increase the size of the heap
}

Vertex ExtractMin(MinHeap heap) {
    if (heap->Size == 0) {  // Check if the heap is empty
        printf("Heap is empty. Cannot extract minimum element.\n");
        return NULL;
    }

    Vertex min = heap->Data[0];  // Store the minimum value (root of the heap)
    heap->Data[0] = heap->Data[heap->Size-1];  // Replace the root with the last element in the heap
    heap->Size--;  // Decrease the size of the heap
    HeapDown(heap, 0);  // Call HeapDown to fix the heap property
    return min;  // Return the minimum value
}

void HeapUp(MinHeap heap, int index) {
    int parent = (index - 1) / 2;  // Calculate the index of the parent node

    // Check if the parent is valid and if the current node violates the heap property
    if (parent >= 0 && heap->Data[index]->Distance < heap->Data[parent]->Distance) {
        Vertex temp = heap->Data[index];  // Swap the current node with its parent
        heap->Data[index] = heap->Data[parent];
        heap->Data[parent] = temp;
        HeapUp(heap, parent);  // Recursively call HeapUp on the parent node
    }
}

void HeapDown(MinHeap heap, int index){
    int left = 2 * index + 1;  // Calculate the index of the left child
    int right = 2 * index + 2;  // Calculate the index of the right child
    int smallest = 0;  // Initialize the index of the smallest child as 0

    // Check if the left child is valid and if it is smaller than the current node
    if (left < heap->Size && heap->Data[left]->Distance < heap->Data[smallest]->Distance)
        smallest = left;

    // Check if the right child is valid and if it is smaller than the current node or the left child
    if (right < heap->Size && heap->Data[right]->Distance < heap->Data[smallest]->Distance)
        smallest = right;

    // Check if the smallest child is different from the current node
    if (smallest) {
        Vertex temp = heap->Data[index];  // Swap the current node with the smallest child
        heap->Data[index] = heap->Data[smallest];
        heap->Data[smallest] = temp;
        HeapDown(heap, smallest);  // Recursively call HeapDown on the smallest child
    }
}

int FindIndex(MinHeap heap, Vertex Value) {
    for (int i = 0; i < heap->Size; i++)
        if (heap->Data[i] == Value)    // If the current element's (Vertex) matches the target value
            return i;   // return the index.

    return -1;  // If the element is not found, return -1 to indicate that it's not present in the heap.
}

void EditElement(MinHeap heap, int index, int newValue) {
    int oldValue = heap->Data[index]->Distance;     // Get the old value of the element
    heap->Data[index]->Distance = newValue;     // Update the Distance of the vertex with the new value

    if (newValue < oldValue) // If the new value is less than the old value, we need to move the element up in the heap
        HeapUp(heap, index);

    else if (newValue > oldValue) // If the new value is greater than the old value, we need to move the element down in the heap
        HeapDown(heap, index);
}

//****************************************************************************************
//                       Functions for BFS  &&  Dijkstra
//****************************************************************************************

void BFS(Graph graph, char City1[], char City2[]) {
    SetDefault(graph);  // Set default values for all vertices in the graph (Default of table)
    Queue Dummy = CreateQueue();  // Create a queue to store the vertices to be processed

    // FindFromGraph the starting city (City1) in the graph and enqueue it to the queue
    Vertex Temp1 = Find(graph, City1);
    Enqueue(Dummy, Temp1);

    // Mark the starting city as visited and set its distance from the source to 0
    Temp1->Visited = 1;
    Temp1->Distance = 0;

    while (Dummy->Front != NULL) {   // Start BFS traversal while the queue is not empty
        Vertex City = Dequeue(Dummy);   // Dequeue the front vertex from the queue
        Edge edge = City->Edge;  // Get the first edge connected to the current vertex (Data)

        // Explore all neighboring cities of the current city (Data)
        while (edge != NULL) {
            Temp1 = Find(graph, edge->City);    // FindFromGraph the neighboring city (Temp1) in the graph based on the current edge

            if (!Temp1->Visited) {  // If the neighboring city has not been visited yet
                Enqueue(Dummy, Temp1);  // Enqueue the neighboring city to the queue
                Temp1->Distance = City->Distance + edge->Weight;    // Update the distance of the neighboring city from the starting city
                strcpy(Temp1->HowToArrive, City->City); // Record the previous city (Data) from which the neighboring city (Temp1) is reached
                Temp1->Visited = 1; // Mark the neighboring city as visited

                if (!strcmp(Temp1->City, City2))    // If the neighboring city is the destination city (City2), the shortest path is found
                    return;
            }
            edge = edge->Next;  // Move to the next edge of the current city (Data)
        }
    }
}

void Dijkstra(Graph graph, char City1[]) {
    MinHeap Dummy = CreateMinHeap(SetDefault(graph) + 10);   // Create a min-heap to store vertices with their distances
    Vertex Temp1 = Find(graph, City1);  // FindFromGraph the starting city (City1) in the graph
    Temp1->Distance = 0; // set its distance from the source to 0
    InsertToHeap(Dummy, Temp1); // Insert the starting city into the min-heap

    while (Dummy->Size != 0) {  // Continue the algorithm until the min-heap is not empty
        Vertex City = ExtractMin(Dummy);    // Extract the vertex with the minimum distance from the min-heap
        City->Visited = 1;  // Mark the current city as visited (processed)
        Edge edge = City->Edge; // Get the first edge connected to the current vertex (Data)

        while (edge != NULL) {  // Explore all neighboring cities of the current city (Data)
            Temp1 = Find(graph, edge->City);    // FindFromGraph the neighboring city (Temp1) in the graph based on the current edge
            int NewDistance = City->Distance + edge->Weight;     // Calculate the new distance from the starting city to the neighboring city

            // If the new distance is smaller than the current recorded distance for the neighboring city
            // Or the neighboring city has not been visited yet (distance=-1), update its distance and path information
            if (NewDistance < Temp1->Distance || Temp1->Distance == -1) {
                int Index = FindIndex(Dummy, Temp1);    // To check if the neighboring city is already in the min-heap
                if (Index != -1) {
                    EditElement(Dummy, Index, NewDistance); // If the neighboring city is in the min-heap, update its distance value
                } else {
                    Temp1->Distance = NewDistance; // Change to new Distance
                    InsertToHeap(Dummy, Temp1); // If the neighboring city is not in the min-heap, insert it with the new distance
                }
                strcpy(Temp1->HowToArrive, City->City); // Record the previous city (Data) from which the neighboring city (Temp1) is reached
            }
            edge = edge->Next;  // Move to the next edge of the current city (Data)
        }
    }
}

List FindRoad(Graph graph, char Destination[]) {
    List result = CreateList();  // Create a linked list to store the cities in the shortest path and their distances
    Vertex End = Find(graph, Destination);  // FindFromGraph the destination city (End) in the graph
    InsertToList(result, Destination, End->Distance);   // Insert the destination city and its distance into the result list

    // Traverse the path from the destination city back to the starting city
    // The path is reconstructed by following the "HowToArrive" field of each vertex
    while (End->Distance != 0) {
        End = Find(graph, End->HowToArrive);    // FindFromGraph the previous city (End->HowToArrive) in the graph
        InsertToList(result, End->City, End->Distance); // Insert the previous city and its distance into the result list
    }

    // After the loop, the linked list "result" contains the cities in reverse order.
    // The starting city will be at the end of the list, and the destination city will be at the front.
    return result;
}


/*

Another example:
Because most of the paths in the first algorithm and the second algorithm will be similar (in given graph)
There is another example

                                          |-----|
                    6 ------------------> |  C  | <---------------------- 1
                     |                    |-----|                        |
                     |                       ^                           |
                     v                       |                           v
                  |-----|                    |                        |-----|
                  |  A  |                    |                        |  E  |
                  |-----|                    |                        |-----|
                     ^                       |                           ^
                     |                       | 1                         |
                     |                    |-----|                        |
                   2 -------------------> |  B  | <---------------------- 5
                                          |-----|

File:

A C 6
A B 2
B C 1
B E 5
B A 2
C E 1
C A 6
E C 2
E B 5

 */