//
//  main.cpp
//  test
//
//  Created by Sellin Tang on 11/10/20.
//  Copyright © 2020 Sellin Tang. All rights reserved.
//

#include <iostream>
#include "GraphMatrix.hpp"

using gra::Path;
template <typename T>
void print(T& lhs) {
    std::cout << lhs << '\t';
}
int main() {
    GraphMatrix<int, int> gra(7, false); // 7 vertices and no edges right now
    for (int i = 0; i < gra.numVertex(); i++) {
        gra.verVal(i) = i;
    }
    gra.addEdge(0, 28, 0, 1); gra.addEdge(0, 28, 1, 0);
    gra.addEdge(0, 10, 0, 5); gra.addEdge(0, 10, 5, 0);
    gra.addEdge(0, 25, 5, 4); gra.addEdge(0, 25, 4, 5);
    gra.addEdge(0, 14, 1, 6); gra.addEdge(0, 14, 6, 1);
    gra.addEdge(0, 16, 1, 2); gra.addEdge(0, 16, 2, 1);
    gra.addEdge(0, 24, 6, 4); gra.addEdge(0, 24, 4, 6);
    gra.addEdge(0, 18, 6, 3); gra.addEdge(0, 18, 3, 6);
    gra.addEdge(0, 22, 4, 3); gra.addEdge(0, 22, 3, 4);
    gra.addEdge(0, 12, 3, 2); gra.addEdge(0, 12, 2, 3);
    void (*func)(int){ &print };
    
    std::cout << "BFS: ";
    gra.bfs(0, func);
    std::cout << std::endl;
    std::cout << "DFS: ";
    gra.dfs(0, func);
    std::cout << std::endl;
    std::cout << "Prim: ";
    std::vector<int> miniSTree = gra.Prim();
    for (auto &i: miniSTree) {
        std::cout << i << '\t';
    }
    std::cout << std::endl;
    std::cout << "Dijsktra: " << std::endl;
    std::vector<Path> dijpath = gra.Dijkstra(0);
    for (auto &i: dijpath) {
        std::cout << i.path << std::endl;
    }
    return 0;
}
