#ifndef PARALLELSPANTREEGRAPH_H
#define PARALLELSPANTREEGRAPH_H

#include <IGraph.h>
#include <thread>
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
        deque<std::thread> threads;
        bool * sawEdges;

        void findMinEdge(int i);
};

#endif // PARALLELSPANTREEGRAPH_H
