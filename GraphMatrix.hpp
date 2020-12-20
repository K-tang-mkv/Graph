#ifndef GRAPHMATRIX_HPP
#define GRAPHMATRIX_HPP

#include "Graph.hpp"
#include <vector>
#include <cstdlib> //for exit;
#include <memory> //for smart pointer
#include <ctime> //for generating a random number
//type of vertex
template <typename Tv>
struct Vertex {
    Tv data;
    int inDegree {0}; //in-degree of the vertex
    int outDegree {0}; //out-degree of the vertex
    VStatus status { VStatus::UNDISCOVERED }; //status of a vertex
    Vertex(Tv const& d = (Tv)0): data(d) { }
};

//type of Edge
template <typename Te>
struct Edge {
    Te data;
    int weight;
    EStatus status { EStatus::UNDISCOVERED };
    Edge(Te const& d, int w): data(d), weight(w) { }
};


template <typename Tv, typename Te>
class GraphMatrix : public gra::Graph<Tv, Te> {
private:
    using base = gra::Graph<Tv, Te>;
    using base::n; //the number of vertices
    using base::e; //the number of edges
    using spEdge = std::shared_ptr<Edge<Te>>; //smart pointer points to Edge
    std::vector<Vertex<Tv>> V; //set of vertices
    std::vector<std::vector<spEdge>> E; //set of edges (graph two dimension matrix) for directed graph
    
public:
    GraphMatrix() { n = e = 0; }
    GraphMatrix(int nVer, bool numEdge = true, Tv data = (Tv)0); //given a number of vertices with common data to generate a random graph, numEdge is true that indicates it will generate random number of edges, if numEdge is false then no edges will be generated
    
//operations of vertex
    virtual Tv& verVal(int i_th) override;
    virtual VStatus& status(int i_th) override;
    virtual int nextNbr(int i, int j) override;
    virtual int firstNbr(int i) override;
    virtual int numNbr(int i_th) override;
    
    virtual int addVertex(Tv const& data) override;
    virtual Tv rmVertex(int i_th) override;
    
    int const& numVertex(); //return the number of vertices
    int const& numEdge(); //return the number of edges
    
    virtual int inDegree(int i) override;
    virtual int outDegree(int i) override;
    
//operations of edge
    virtual void addEdge(Te const& data, int w, int source, int target) override;
    virtual Te rmEdge(int i, int j) override; //remove the edge at(i,j)
    
    virtual EStatus& Estatus(int i_th, int j_th) override;
    virtual bool exists(int i, int j) override;
    virtual int& weight(int i, int j) override;
    virtual Te& edgeData(int i, int j) override;
    //GraphMatrix
};

template <typename Tv, typename Te>
GraphMatrix<Tv, Te>::GraphMatrix(int nVer, bool numEdge, Tv data) {
    n = 0;
    e = 0;
    for (int i = 0; i < nVer; i++) {
        this->addVertex(data);
    } //build up a graph matrix
    //next, we generate edges randomly
    if (numEdge) {
        std::srand((unsigned)time(nullptr)); //seed of random number
        int iter_num = rand() % nVer + 1; //the number of generating an edge randomly
        while (iter_num--) {
            int i = rand() % nVer;
            int j = rand() % nVer; //generate an edge at (i,j)
            this->addEdge(0,0,i,j); //edge with default data 0 and weight 0
        }
    }
}

template <typename Tv, typename Te>
inline Tv& GraphMatrix<Tv, Te>::verVal(int i_th) {
    return V[i_th].data;
}
template <typename Tv, typename Te>
inline VStatus& GraphMatrix<Tv, Te>::status(int i_th) {
    return V[i_th].status;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::nextNbr(int i, int j) {
    j--; //check if the next vertex is the neighbor
    while (j > -1 && !this->exists(i, j) && !this->exists(j, i)) {
        j--;
    }
    return j;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::firstNbr(int i) {
    return this->nextNbr(i, n);
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::numNbr(int i_th) {
    int _count {0};
    for (int i = 0; i < n; i++) {
        if (i == i_th) {
            continue;
        }
        if (exists(i_th, i) || exists(i, i_th)) {
            _count++;
        }
    }
    return _count;
}

template <typename Tv, typename Te>
int GraphMatrix<Tv, Te>::addVertex(Tv const& data) {
    for (int i = 0; i < n; i++) {
        E[i].emplace_back(nullptr); //extent the number of rows at each column
    }
    n++;
    E.emplace_back(std::vector<spEdge>(n, nullptr)); //increase the number of columns
    V.emplace_back(data);
    return n; //return the id of this vertex
}

template <typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::rmVertex(int i_th) {
    for (int i = 0; i < n; i++) {
        if (E[i_th][i]) {
            V[i].inDegree--;
        }
    }
    E.erase(E.begin()+i_th-1); //remove the i_th row
    for (int i = 0; i < n; i++) {
        if (E[i][i_th]) {
            V[i].outDegree--;
        }
        E[i].erase(E[i].begin()+i_th-1); //remove the i_th column
    }
    n--;
    Tv tem = V[i_th].data;
    V.erase(V.begin()+i_th-1); //remove the i_th vertex
    return tem;
}

template <typename Tv, typename Te>
inline int const& GraphMatrix<Tv, Te>::numVertex() {
    return n;
}

template <typename Tv, typename Te>
inline int const& GraphMatrix<Tv, Te>::numEdge() {
    return e;
}

template <typename Tv, typename Te>
inline int GraphMatrix<Tv, Te>::inDegree(int i) {
    return V[i-1].inDegree;
}

template <typename Tv, typename Te>
inline int GraphMatrix<Tv, Te>::outDegree(int i_th) {
    return V[i_th-1].outDegree;
}

template <typename Tv, typename Te> //add an edge at (i, j), i to j
void GraphMatrix<Tv, Te>::addEdge(Te const& data, int w, int source, int target) {
    //first check if there exists an edge
    if (this->exists(source, target)) {
        return ;
    }
    E[source][target] = std::make_shared<Edge<Te>>(data, w);
    V[source].outDegree++;
    V[target].inDegree++;
    e++;
}
 
template <typename Tv, typename Te>
Te GraphMatrix<Tv, Te>::rmEdge(int i, int j) {
    if (!this->exists(i, j)) {
        std::cout << "There is not an edge can be removed!" << std::endl;
        exit(0);
    }
    Te tem = E[i][j]->data;
    V[i].outDegree--;
    V[j].inDegree--;
    E[i][j].reset(); //remove the edge at (i, j)
    return tem;
}

template <typename Tv, typename Te>
inline EStatus& GraphMatrix<Tv, Te>::Estatus(int i_th, int j_th) {
    return E[i_th][j_th]->status;
}
template <typename Tv, typename Te>
inline bool GraphMatrix<Tv, Te>::exists(int i, int j) {
    if (E[i][j]) {
        return true;
    }
    else
        return false;
}

template <typename Tv, typename Te>
inline int& GraphMatrix<Tv, Te>::weight(int i, int j) {
    return E[i][j]->weight;
}

template <typename Tv, typename Te>
inline Te& GraphMatrix<Tv, Te>::edgeData(int i, int j) {
    return E[i][j]->data;
}

template <typename T>
void print(T obj) {
    std::cout << obj << '\t';
}

#endif /* GRAPHMATRIX_HPP */

