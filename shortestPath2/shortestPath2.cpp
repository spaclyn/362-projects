#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <string>
#include <cmath>

using namespace std;

class Graph {
private:
    struct Edge {
        string dest;
        int cost;
        int gain;
        double weight; // Calculated weight based on cost and gain
        
        Edge(string d, int c, int g) : dest(d), cost(c), gain(g) {
            // Weight calculation: cost increases weight, gain decreases weight
            // Using ratio: weight = cost / gain
            // Higher cost → higher weight, higher gain → lower weight
            weight = static_cast<double>(cost) / gain;
        }
    };
    
    unordered_map<string, vector<Edge>> adjList;
    
public:
    void addEdge(const string& src, const string& dest, int cost, int gain) {
        adjList[src].emplace_back(dest, cost, gain);
    }
    
    // Dijkstra's algorithm to find shortest path from start to end
    pair<double, vector<string>> dijkstra(const string& start, const string& end) {
        // Priority queue: (distance, vertex)
        priority_queue<pair<double, string>, 
                      vector<pair<double, string>>, 
                      greater<pair<double, string>>> pq;
        
        // Distance map
        unordered_map<string, double> dist;
        unordered_map<string, string> prev;
        unordered_map<string, bool> visited;
        
        // Initialize distances to infinity
        for (const auto& vertex : adjList) {
            dist[vertex.first] = numeric_limits<double>::infinity();
        }
        
        // Initialize start vertex
        dist[start] = 0;
        pq.push({0, start});
        
        // Dijkstra's algorithm
        while (!pq.empty()) {
            string u = pq.top().second;
            pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            // if reached the destination, can break early
            if (u == end) break;
            
            // Explore neighbors
            for (const Edge& edge : adjList[u]) {
                string v = edge.dest;
                double weight = edge.weight;
                
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        
        // Reconstruct path
        vector<string> path;
        if (dist[end] == numeric_limits<double>::infinity()) {
            return {numeric_limits<double>::infinity(), path};
        }
        
        string current = end;
        while (current != start) {
            path.insert(path.begin(), current);
            current = prev[current];
        }
        path.insert(path.begin(), start);
        
        return {dist[end], path};
    }
    
    void printGraph() {
        cout << "Graph Structure with Weights:" << endl;
        cout << "=============================" << endl;
        for (const auto& vertex : adjList) {
            cout << "Vertex " << vertex.first << " -> ";
            for (const Edge& edge : vertex.second) {
                printf("%s [cost=%d, gain=%d, weight=%.3f]  ", 
                       edge.dest.c_str(), edge.cost, edge.gain, edge.weight);
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    Graph g;
    
    // Add edges with [cost, gain] pairs
    g.addEdge("a", "b", 3, 2);
    g.addEdge("a", "c", 4, 5);
    g.addEdge("b", "c", 2, 1);
    g.addEdge("b", "d", 6, 4);
    g.addEdge("b", "e", 1, 6);
    g.addEdge("c", "e", 5, 3);
    g.addEdge("c", "f", 3, 1);
    g.addEdge("d", "e", 3, 2);
    g.addEdge("e", "f", 2, 10);
    
    // Print graph structure
    g.printGraph();
    
    // Find shortest path from a to f
    string start = "a";
    string end = "f";
    
    auto result = g.dijkstra(start, end);
    double shortestDistance = result.first;
    vector<string> shortestPath = result.second;
    
    cout << "Shortest Path from " << start << " to " << end << ":" << endl;
    cout << "==================================" << endl;
    
    if (shortestDistance == numeric_limits<double>::infinity()) {
        cout << "No path exists!" << endl;
    } else {
        cout << "Path: ";
        for (size_t i = 0; i < shortestPath.size(); i++) {
            cout << shortestPath[i];
            if (i < shortestPath.size() - 1) cout << " -> ";
        }
        cout << endl;
        printf("Total weight: %.3f\n", shortestDistance);
        
        // Print detailed weight calculation
        cout << "\nDetailed Path Analysis:" << endl;
        cout << "-----------------------" << endl;
        double total = 0;
        for (size_t i = 0; i < shortestPath.size() - 1; i++) {
            // In a real implementation, you would look up the actual weight here
            // For simplicity, we'll just note the edges
            cout << shortestPath[i] << " -> " << shortestPath[i + 1] << endl;
        }
    }
    
    return 0;
}