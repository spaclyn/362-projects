#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int dest;      // Destination node
    double weight; // Edge weight
    
    Edge(int d, double w) : dest(d), weight(w) {}
};

// Structure for priority queue (used in Dijkstra's algorithm)
// Stores node index and its current distance from source
struct Node {
    int index;
    double dist;
    
    Node(int i, double d) : index(i), dist(d) {}
    
    // Comparator for min-heap (prioritize smaller distances)
    bool operator>(const Node& other) const {
        return dist > other.dist;
    }
};

// Function to print the shortest path from source to destination
void printPath(vector<int>& parent, int dest, vector<char>& nodeLabels) {
    if (parent[dest] == -1) {
        cout << nodeLabels[dest];
        return;
    }
    printPath(parent, parent[dest], nodeLabels);
    cout << " -> " << nodeLabels[dest];
}

int main() {
    // GRAPH REPRESENTATION
    // ====================
    // The graph is represented using an ADJACENCY LIST
    // - Each node has a vector of edges (destination node + weight)
    
    const int NUM_NODES = 17; // Total number of nodes in the graph
    
    // Node labels for easy identification (A and B are our source/dest)
    vector<char> nodeLabels = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
                               'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'};
    
    // Map node labels to indices for reference
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9, K=10, 
    // L=11, M=12, N=13, O=14, P=15, Q=16
    
    // Create adjacency list - vector of vectors of edges
    vector<vector<Edge>> graph(NUM_NODES);
    
    // BUILDING THE GRAPH FROM THE IMAGE
    // ==================================
    // Adding all edges (undirected graph, so add edge in both directions)
    
    // Node A connections (index 0)
    graph[0].push_back(Edge(15, 0.4));   // A -> P: 0.4
    graph[15].push_back(Edge(0, 0.4));   // P -> A: 0.4
    
    graph[0].push_back(Edge(16, 0.32));  // A -> Q: 0.32
    graph[16].push_back(Edge(0, 0.32));  // Q -> A: 0.32
    
    // Node B connections (index 1)
    graph[1].push_back(Edge(2, 0.2));    // B -> C: 0.2
    graph[2].push_back(Edge(1, 0.2));    // C -> B: 0.2
    
    graph[1].push_back(Edge(3, 0.3));    // B -> D: 0.3
    graph[3].push_back(Edge(1, 0.3));    // D -> B: 0.3
    
    // Node C connections (index 2)
    graph[2].push_back(Edge(4, 0.28));   // C -> E: 0.28
    graph[4].push_back(Edge(2, 0.28));   // E -> C: 0.28
    
    // Node D connections (index 3)
    graph[3].push_back(Edge(4, 0.05));   // D -> E: 0.05
    graph[4].push_back(Edge(3, 0.05));   // E -> D: 0.05
    
    graph[3].push_back(Edge(5, 0.45));   // D -> F: 0.45
    graph[5].push_back(Edge(3, 0.45));   // F -> D: 0.45
    
    // Node E connections (index 4)
    graph[4].push_back(Edge(6, 0.2));    // E -> G: 0.2
    graph[6].push_back(Edge(4, 0.2));    // G -> E: 0.2
    
    // Node F connections (index 5)
    graph[5].push_back(Edge(6, 0.3));    // F -> G: 0.3
    graph[6].push_back(Edge(5, 0.3));    // G -> F: 0.3
    
    // Node G connections (index 6)
    graph[6].push_back(Edge(7, 0.2));    // G -> H: 0.2
    graph[7].push_back(Edge(6, 0.2));    // H -> G: 0.2
    
    graph[6].push_back(Edge(10, 0.3));   // G -> K: 0.3
    graph[10].push_back(Edge(6, 0.3));   // K -> G: 0.3
    
    // Node H connections (index 7)
    graph[7].push_back(Edge(8, 0.35));   // H -> I: 0.35
    graph[8].push_back(Edge(7, 0.35));   // I -> H: 0.35
    
    graph[7].push_back(Edge(11, 0.25));  // H -> L: 0.25
    graph[11].push_back(Edge(7, 0.25));  // L -> H: 0.25
    
    // Node I connections (index 8)
    graph[8].push_back(Edge(9, 0.2));    // I -> J: 0.2
    graph[9].push_back(Edge(8, 0.2));    // J -> I: 0.2
    
    graph[8].push_back(Edge(12, 0.05));  // I -> M: 0.05
    graph[12].push_back(Edge(8, 0.05));  // M -> I: 0.05
    
    // Node J connections (index 9)
    graph[9].push_back(Edge(13, 0.08));  // J -> N: 0.08
    graph[13].push_back(Edge(9, 0.08));  // N -> J: 0.08
    
    graph[9].push_back(Edge(15, 0.4));   // J -> P: 0.4
    graph[15].push_back(Edge(9, 0.4));   // P -> J: 0.4
    
    // Node K connections (index 10)
    graph[10].push_back(Edge(11, 0.28)); // K -> L: 0.28
    graph[11].push_back(Edge(10, 0.28)); // L -> K: 0.28
    
    // Node L connections (index 11)
    graph[11].push_back(Edge(12, 0.15)); // L -> M: 0.15
    graph[12].push_back(Edge(11, 0.15)); // M -> L: 0.15
    
    // Node M connections (index 12)
    graph[12].push_back(Edge(13, 0.2));  // M -> N: 0.2
    graph[13].push_back(Edge(12, 0.2));  // N -> M: 0.2
    
    graph[12].push_back(Edge(14, 0.1));  // M -> O: 0.1
    graph[14].push_back(Edge(12, 0.1));  // O -> M: 0.1
    
    // Node N connections (index 13)
    graph[13].push_back(Edge(14, 0.2));  // N -> O: 0.2
    graph[14].push_back(Edge(13, 0.2));  // O -> N: 0.2
    
    // Node O connections (index 14)
    graph[14].push_back(Edge(16, 0.4));  // O -> Q: 0.4
    graph[16].push_back(Edge(14, 0.4));  // Q -> O: 0.4
    
    // Node P connections (index 15)
    // (already defined above)
    
    // Node Q connections (index 16)
    // (already defined above)
    
    cout << "=== GRAPH REPRESENTATION ===" << endl;
    cout << "Using Adjacency List representation" << endl;
    cout << "Total nodes: " << NUM_NODES << endl << endl;
    
    // Display the graph structure
    for (int i = 0; i < NUM_NODES; i++) {
        cout << "Node " << nodeLabels[i] << " (index " << i << ") connects to: ";
        for (const Edge& e : graph[i]) {
            cout << nodeLabels[e.dest] << "(" << e.weight << ") ";
        }
        cout << endl;
    }
    
    // DIJKSTRA'S ALGORITHM IMPLEMENTATION
    // ====================================
    cout << "\n=== RUNNING DIJKSTRA'S ALGORITHM ===" << endl;
    
    int source = 0;      // Node A (source)
    int destination = 1; // Node B (destination)
    
    cout << "Finding shortest path from " << nodeLabels[source] 
         << " to " << nodeLabels[destination] << endl << endl;
    
    // Initialize distance array (all nodes start at infinity)
    vector<double> dist(NUM_NODES, numeric_limits<double>::infinity());
    dist[source] = 0.0; // Distance to source is 0
    
    // Parent array to reconstruct the path
    vector<int> parent(NUM_NODES, -1);
    
    // Visited array to track processed nodes
    vector<bool> visited(NUM_NODES, false);
    
    // Priority queue (min-heap) to get node with minimum distance
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push(Node(source, 0.0));
    
    // Main Dijkstra's loop
    while (!pq.empty()) {
        // Extract node with minimum distance
        Node current = pq.top();
        pq.pop();
        
        int u = current.index;
        
        // Skip if already visited (can happen with duplicates in pq)
        if (visited[u]) continue;
        
        visited[u] = true;
        
        cout << "Processing node " << nodeLabels[u] 
             << " (current dist: " << dist[u] << ")" << endl;
        
        // Early termination if we reached destination
        if (u == destination) {
            cout << "Reached destination!" << endl;
            break;
        }
        
        // Relax all adjacent edges
        for (const Edge& e : graph[u]) {
            int v = e.dest;
            double weight = e.weight;
            
            // If found a shorter path to v through u
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(Node(v, dist[v]));
                
                cout << "  Updated " << nodeLabels[v] 
                     << " distance to " << dist[v] 
                     << " (via " << nodeLabels[u] << ")" << endl;
            }
        }
    }
    
    // OUTPUT RESULTS
    // ==============
    cout << "\n=== RESULTS ===" << endl;
    
    if (dist[destination] == numeric_limits<double>::infinity()) {
        cout << "No path exists from " << nodeLabels[source] 
             << " to " << nodeLabels[destination] << endl;
    } else {
        cout << "Shortest distance from " << nodeLabels[source] 
             << " to " << nodeLabels[destination] << ": " 
             << dist[destination] << endl;
        
        cout << "Shortest path: ";
        printPath(parent, destination, nodeLabels);
        cout << endl;
    }
    
    return 0;
}