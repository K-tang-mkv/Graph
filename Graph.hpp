#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <climits> //for infinite number INT_MAX
#include <iostream>
#include <queue>

enum class VStatus { UNDISCOVERED, DISCOVERED, TREE}; //vertex status, discovered means that the current vertex was traversaled, vice versa, TREE means that the vertex is in the spanning tree
enum class EStatus { UNDISCOVERED, TREE, CROSS, FORWARD, BACKWARD }; //edge status

namespace gra {

struct Path; //for Dijkstra's algorithm

template <typename Tv, typename Te> //vertex type, edge type
class Graph {
private:    
    void reset() {
        for (int i = 0; i < n; i++) {
            status(i) = VStatus::UNDISCOVERED;
            for (int j = 0; j < n; j++) {
                if (exists(i, j)) {
                    Estatus(i, j) = EStatus::UNDISCOVERED;
                }
            }
        }
    }; //reset to original status for all vertexs and edges
    template <typename VST> void BFS(int, VST&); //VST is function type, breadth-first-search for single connected graph
    template <typename VST> void DFS(int, VST&); //depth-first-search for single connected graph
public:
//for vertices
    int n; //the number of vertices
    virtual Tv& verVal(int i_th) = 0; //return the value of the i_th vertex
    virtual VStatus& status(int i_th) = 0; //check what the state of i_th vertex is
    virtual int nextNbr(int i, int j) = 0; //the next adjacent point relative to vertex j
    virtual int firstNbr(int i) = 0; //the first adjacent point of vertex i
    virtual int numNbr(int i_th) = 0; //return the number of the neighbors of the i_th vertex
    virtual int addVertex(Tv const& data) = 0; //insert vertex and return the id
    virtual Tv rmVertex(int i_th) = 0; //delete the i-th vertex and its associate edges and return the data of the vertex
    
    virtual int inDegree(int i) = 0; //return the number of in-degree of the i-th vertex
    virtual int outDegree(int i) = 0; //return the number of out-degree of the i-th vertex
    
//for edges
    int e; //the number of edges
    virtual void addEdge(Te const& data, int w, int source, int target) = 0; //add an edge with weight w between source and target, or (source, target)
    virtual Te rmEdge(int i, int j) = 0; //remove the edge between i and j, or (i, j)
    
    virtual EStatus& Estatus(int i_th, int j_th) = 0;
    virtual bool exists(int i, int j) = 0; //check whether edge(i,j) exists
    virtual int& weight(int i, int j) = 0; //return the weight of an edge
    virtual Te& edgeData(int i, int j) = 0; //return the data of an edge
    
//algorithm
    template <typename VST> void bfs(int i_th, VST& func); //breadth-first-search for full graph
    template <typename VST> void dfs(int i_th, VST& func); //depth-first-search for full graph
    std::vector<int> Prim(); //return the vector which records the order of the vertices push to the MST set
    void getNextMSTver(std::vector<int>& mstSet); //get the next MST vertex with the minimum weight of the edge connected the i_th vertex, push the position of the next vertex to the mstSet
    std::vector<Path> Dijkstra(int start); //find the minimum distance of each vertex from the start vertex
    void getNextDijver(std::vector<int>& dijSet, std::vector<Path>& dijPathSet); //find the next vertex with the minimum distance from the start vertex
};

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::bfs(int s, VST& func) { //starting from any random vertex s
    reset();
    int v = s;
    do {
        if (VStatus::UNDISCOVERED == status(v)) { //once meet the vertex undiscovered
            BFS(v, func); //using BFS from this vertex
        }
    } while (s != (v = (++v % n)) && false); //avoid some vertex from not being traversaled, using module operation to traversal all vertices
}

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::BFS(int s, VST& func) {
    status(s) = VStatus::DISCOVERED;
    std::queue<int> Q;
    Q.push(s); //enqueue v
    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        for (int i = firstNbr(v); i > -1; i = nextNbr(v, i)) {
            if (status(i) != VStatus::DISCOVERED) {
                status(i) = VStatus::DISCOVERED;
                Q.push(i); //put all neghibor vertices unvisited of the vertex v into the queue
            }
        }
        func(verVal(v));
    }
}

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::dfs(int s, VST& func) {
    reset();
    int v = s;
    do {
        if (status(v) == VStatus::UNDISCOVERED) {
            DFS(v, func);
        }
    } while (s != (v = (++v % n)) && false);
}

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::DFS(int s, VST& func) {
    status(s) = VStatus::DISCOVERED;
    func(verVal(s));
    for (int i = firstNbr(s); i > -1; i = nextNbr(s, i)) { //deep-first-search for each neghibour vertex
        if (status(i) == VStatus::UNDISCOVERED) {
            DFS(i, func);
        }
    }
}

//check if one vertex is in the MST set
bool isExistInMST(std::vector<int>& mstSet, int& i_th) {
    for (auto& i : mstSet) {
        if (i == i_th) {
            return true;
        }
    }
    return false;
}
template <typename Tv, typename Te>
void Graph<Tv, Te>::getNextMSTver(std::vector<int>& mstSet) {
    //first, we need to find the cut group according to the current MST set because we need to select an edge with the minimum weight from the cut group to avoid the cycle path from appearing
    for (auto& i_th: mstSet) {
        
        for (int i = firstNbr(i_th); i > -1; i = nextNbr(i_th, i)) {
            if (Estatus(i_th, i) == EStatus::TREE) {
                continue;
            }
            else {
                if (isExistInMST(mstSet, i)) {
                    Estatus(i_th, i) = EStatus::UNDISCOVERED;
                    Estatus(i, i_th) = EStatus::UNDISCOVERED;
                    continue; //if the i-th vertex is in the MST set, then the edge between the i-th vertex and the i_th vertex can not be selected as a cross edge, otherwise it will cause the cycle path
                }
                else {
                    Estatus(i_th, i) = EStatus::CROSS;
                    Estatus(i, i_th) = EStatus::CROSS;
                }
            }
        }
    }
    int source {mstSet[0]}; //it records the position of the vertex which is the source end of the minimum weight edge
    int j = firstNbr(mstSet[0]); //find the first neighbor and use j to record the position of the vertex with the minimum wight edge
    int miniWeight = weight(mstSet[0], j); //assume the weight of the edge between them is the smallest
    for (auto& i_th: mstSet) {
        //and then traversal all the neighbor vertices to find the edge with minimum weight
        for (int i = firstNbr(i_th); i > -1; i = nextNbr(i_th, i)) {
            if (Estatus(i_th, i) != EStatus::CROSS) {
                continue;
            }
            else { //only select an edge from the cut group
                if (miniWeight > weight(i_th, i)) {
                    miniWeight = weight(i_th, i);
                    source = i_th;
                    j = i; //record the position of the vertex with the minimum wight edge
                }
            }
        }
    }
    Estatus(source, j) = EStatus::TREE;
    Estatus(j, source) = EStatus::TREE;
    mstSet.push_back(j); //push the position of the next vertex who will be in the MST set
}

template <typename Tv, typename Te>
std::vector<int> Graph<Tv, Te>::Prim() {
    reset();
    std::vector<int> mstSet; //vertices set stores the position of each vertex in the minimum spanning tree
    std::srand((unsigned)time(nullptr));
    int firstVer = rand() % n;
    mstSet.push_back(firstVer); //pick one vertex to the minimum spanning tree set randomly
    status(firstVer) = VStatus::TREE; //mark the vertex status as TREE
    while (mstSet.size() != n) {
        //traversal the whole MST set to find the next MST vertex
        getNextMSTver(mstSet);
    }
    return mstSet;
}

//Dijkstra's algorithm
struct Path {
    //this represents the minimum distance path from the source vertex to the final vertex
    int dis; //total distance
    std::string path; //like "v1-->vi"
   // bool isIndij = false; //check if the vertex is in dijset
    Path():dis{INT_MAX}, path{" "} {} //at first assume the distances from all nodes to the start node is infinte
};

template <typename Tv, typename Te>
void updateSets(Graph<Tv, Te>& obj, std::vector<Path>& dijPathSet, std::vector<Path>& undijSet, int& start) {
    int mindis = undijSet[0].dis;
    int miniPos = 0; //record the position of minidistance
    for (int i = 0; i < undijSet.size(); i++) {
        if (mindis > undijSet[i].dis) {
            mindis = undijSet[i].dis;
            miniPos = i;
        }
    }
    //update undij set
    for (int i = obj.firstNbr(miniPos); i > -1; i = obj.nextNbr(miniPos, i)) {
        if (obj.status(i) == VStatus::UNDISCOVERED) {
            if (obj.weight(miniPos, i) + undijSet[miniPos].dis < undijSet[i].dis) {
                undijSet[i].path = undijSet[miniPos].path + "-->v" + std::to_string(i);
                undijSet[i].dis = obj.weight(miniPos, i) + undijSet[miniPos].dis;
            }
        }
    }
    //add the vertex with minimal distance to dijPathSet
    dijPathSet.push_back(undijSet[miniPos]);
    obj.status(miniPos) = VStatus::DISCOVERED; //mark the vertex as discovered
    undijSet[miniPos].dis = INT_MAX;
}

template <typename Tv, typename Te>
std::vector<Path> Graph<Tv, Te>::Dijkstra(int start) {
    reset();
    //firstly we divide the set of vertices into two parts
    std::vector<Path> dijPathSet;
    std::vector<Path> undijSet;
    //first inialize the undijset array, "v[start]-->v[i] = dis"
    for (int i = 0; i < n; i++) {
        Path _path;
        _path.path = "v" + std::to_string(start);
        undijSet.push_back(_path);
    }
    
    //then we need find the minimum distance from start to all vertices
    undijSet[start].dis = 0; //first pick up the start itself and set the distance as 0
    //invoke the update function to update two sets
    
    //next find the vertex with minimum distance from it to the start vertex and update the distance in undijSet
    while (dijPathSet.size() != n) {
        updateSets(*this, dijPathSet, undijSet, start);
    }
    
    for (auto &i : dijPathSet) {
        i.path += " = " + std::to_string(i.dis);
    }
    return dijPathSet;
}
}
#endif /* GRAPH_HPP */
