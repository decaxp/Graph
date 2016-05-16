#include <iostream>
#include <stdio.h>
#include "ParallelSpanTreeGraph.h"

#include <vector>
#include <thread>
#include <iostream>

using namespace std;
int main()
{
    ParallelSpanTreeGraph g(3);
    g.readGraphFromFile("graph.txt");
    g.minSpaningTree();
    //g.Print();

    return 0;

}
