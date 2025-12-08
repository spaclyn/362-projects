SHORTEST PARTH #2

## General Information

-   **Author Information:**
    -   Principal Coder: Atiyah B. Ellerbee
-   **Keywords:** shortest, path, Dijkstra, algorithm
## Data/File Overview

-   **File List and Descriptions:**
    -   shortestPath2.cpp: [Code for shortestPath2.exe, the overall algroithm to see shortest path output. ]
    -   shortestPath2.exe: [Exe debugged from shortestPath2.cpp]


-   **File Formats:** cpp, exe, txt
-   **File Relationships/Structure:** shortestPath2.cpp informs shortestPath2.exe
-   **Date of Last Update/Version:** 12/8/2025
-   **Related Data:** some help from textbook

## Usage/Access Information

-   **Description:** A shortest path algorithm using Dijkstra's algorithm to make it easier
-   **Prerequisites:** ---
-   **Installation/Setup:** dependent on your system, see screen recording for execution expected on a windows machine
-   **Instructions for Use:** 
            debug and run shortestPath2, to calculate the shortest path using Dijkstra's algorithm
-   **Known Issues/Limitations:** 

## Insights 

# Shortest Path in Directed Graph with Cost-Gain Edge Weights

## Problem Overview
Find the shortest path from vertex `a` to vertex `f` in a directed graph where each edge has:
- **cost**: Higher values increase the edge weight
- **gain**: Higher values decrease the edge weight

## Edge Weight Calculation
The key challenge is combining cost and gain into a single weight for Dijkstra's algorithm.

### Chosen Weight Formula: weight = cost / gain

### Rationale:
1. **Mathematical Properties**:
   - As cost increases → weight increases (linear relationship)
   - As gain increases → weight decreases (inverse relationship)
   - The ratio properly balances both factors

2. **Alternative Formulas Considered and Rejected**:
   - `weight = cost - gain`: Problematic when gain > cost (negative weights)
   - `weight = cost + (1/gain)`: Gain has diminishing effect
   - `weight = cost * (1/gain)`: Equivalent to our chosen ratio
   - `weight = α*cost - β*gain`: Requires parameter tuning

3. **Why `cost/gain` is Optimal**:
   - Simple and intuitive interpretation: "cost per unit gain"
   - Preserves monotonic relationships
   - No arbitrary parameters needed
   - Works with the given data ranges

### Example Calculations:
- Edge (a,b): cost=3, gain=2 → weight = 3/2 = 1.500
- Edge (a,c): cost=4, gain=5 → weight = 4/5 = 0.800
- Edge (b,e): cost=1, gain=6 → weight = 1/6 = 0.167

## Graph Structure
Vertices: {a, b, c, d, e, f}
Edges with [cost, gain]:
- (a, b) = [3, 2]
- (a, c) = [4, 5]
- (b, c) = [2, 1]
- (b, d) = [6, 4]
- (b, e) = [1, 6]
- (c, e) = [5, 3]
- (c, f) = [3, 1]
- (d, e) = [3, 2]
- (e, f) = [2, 10]

## Implementation Details
1. **Data Structures**:
   - Adjacency list representation
   - Priority queue for Dijkstra's algorithm
   - Hash maps for distances and predecessors

2. **Algorithm**:
   - Standard Dijkstra's algorithm
   - Uses calculated weights (cost/gain)
   - Returns both shortest distance and path

3. **Complexity (NOTATION)**:
   - Time: O((V+E) log V)
   - Space: O(V+E)

## Contact Information

-   **For Questions/Support:** aellerbe@purdue.edu