#include <climits> //for infinite number INT_MAX
#include <iostream>
#include <queue>
enum class VStatus { UNDISCOVERED, DISCOVERED }; //vertex status
enum class EStatus { UNDISCOVERED, TREE, CROSS, FORWARD, BACKWARD }; //edge status

namespace gra {

template <typename Tv, typename Te> //vertex type, edge type
class Graph {
private:    
    //void reset(); //reset to original status for all vertexs and edges
    template <typename VST> void BFS(int, VST&); //VST is function type, breadth-first-search for single connected graph
    template <typename VST> void DFS(int, VST&); //depth-first-search for single connected graph
public:
//for vertices
    int n; //the number of vertices
    virtual Tv& verVal(int i_th) = 0; //return the value of the i_th vertex
    virtual VStatus& status(int i_th) = 0; //check what the state of i_th vertex is
    virtual int nextNbr(int i, int j) = 0; //the next adjacent point relative to vertex j
    virtual int firstNbr(int i) = 0; //the first adjacent point of vertex i
    virtual int addVertex(Tv const& data) = 0; //insert vertex and return the id
    virtual Tv rmVertex(int i_th) = 0; //delete the i-th vertex and its associate edges and return the data of the vertex
    
    virtual int inDegree(int i) = 0; //return the number of in-degree of the i-th vertex
    virtual int outDegree(int i) = 0; //return the number of out-degree of the i-th vertex
    
//for edges
    int e; //the number of edges
    virtual void addEdge(Te const& data, int w, int source, int target) = 0; //add an edge with weight w between source and target, or (source, target)
    virtual Te rmEdge(int i, int j) = 0; //remove the edge between i and j, or (i, j)
    
    virtual bool exists(int i, int j) = 0; //check whether edge(i,j) exists
    virtual int& weight(int i, int j) = 0; //return the weight of an edge
    virtual Te& edgeData(int i, int j) = 0; //return the data of an edge
    
//algorithm
    template <typename VST> void bfs(int i_th, VST& func); //breadth-first-search for full graph
    template <typename VST> void dfs(int i_th, VST& func); //depth-first-search for full graph
};

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::bfs(int s, VST& func) { //starting from any random vertex s
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
                i--;
            }
            else
                i--;
        }
        func(verVal(v));
    }
}

template <typename Tv, typename Te> template <typename VST>
void Graph<Tv, Te>::dfs(int s, VST& func) {
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
            i--;
        }
        else
            i--;
    }
}
}
