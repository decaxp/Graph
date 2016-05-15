#ifndef PARALLELSPANTREEGRAPH_H
#define PARALLELSPANTREEGRAPH_H

#include <IGraph.h>
#include <thread>
#include <vector>
#include <deque>

class ParallelSpanTreeGraph : public IGraph
{
    public:
        ParallelSpanTreeGraph(int thrCount=0);
        virtual ~ParallelSpanTreeGraph();

        void minSpaningTree();
    protected:
        int threadCount;
        deque<Edge> mayEdges;
        deque<Edge> minSpanTreeEdges;
        vector<std::thread> threads;
        bool * sawEdges;

        deque<Edge>::iterator findMinEdgeInSec();//последовательная версия
};

#endif // PARALLELSPANTREEGRAPH_H
