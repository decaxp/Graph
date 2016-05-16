#ifndef IGRAPH_H
#define IGRAPH_H

#include <string>
#include <deque>

using namespace std;

struct Edge
{
    int verticleNumber;//номер вершины конечной
    int parentVerticleNumber;
	int edgeWeight;    //вес ребра
    Edge(){
            verticleNumber = -1;
            edgeWeight = 0;
    }
    Edge(int verNum,int parentNum, int edWeight){
            verticleNumber = verNum;
            parentVerticleNumber=parentNum;
            edgeWeight = edWeight;
      }
};

class IGraph
{
    public:
        IGraph();
        IGraph(deque<Edge>* pEdges,int pVerticleNumber);
        ~IGraph();

        void Print();

        virtual void readGraphFromFile(char* name);
        virtual void saveGraphToFile(char* name);
        void generateRandomGraph();
        virtual IGraph minSpaningTree();

    protected:
        Edge strToEdge(string str,int parentNum);

        deque<Edge> * edges;
        int verticleCount;
};

#endif // IGRAPH_H
