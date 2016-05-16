#include "ParallelSpanTreeGraph.h"
#include <iostream>

using namespace std;
int main()
{
    int threadCount;
    cout<<"Enter thread count\n";
    cin>>threadCount;
    ParallelSpanTreeGraph g(threadCount);
    g.readGraphFromFile("graph.txt");
    IGraph result=g.minSpaningTree();
    result.Print();

    return 0;
}
