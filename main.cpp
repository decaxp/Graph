#include <iostream>
#include <stdio.h>
#include "ParallelSpanTreeGraph.h"

#include <vector>
#include <thread>
#include <iostream>

using namespace std;
int main()
{

    ParallelSpanTreeGraph g;
    g.readGraphFromFile("graph.txt");
    g.minSpaningTree();
    //g.Print();

    int a;
    cin>>a;


}
