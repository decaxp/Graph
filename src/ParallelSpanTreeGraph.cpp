#include "ParallelSpanTreeGraph.h"
#include "IGraph.h"
#include <thread>
#include <deque>
#include <iostream>
#include <mutex>

using namespace std;

deque<Edge>::iterator minEdge;
std::mutex report_mutex;

ParallelSpanTreeGraph::ParallelSpanTreeGraph(int thrCount){
    threadCount=thrCount;
    sawEdges=NULL;
}
ParallelSpanTreeGraph::~ParallelSpanTreeGraph(){
    if (sawEdges!=NULL) delete [] sawEdges;
}

void ParallelSpanTreeGraph::findMinEdge(int i){
    //сколько обрабатывает поток. если последний поток, то пусть обрабатывает все до остатка
    int portion= mayEdges.size()/threadCount;
    deque<Edge>::iterator minElement=mayEdges.begin()+i*portion;//первый просматриваемый элемент

    //проверка на существование цикла
    while(true){
        if (sawEdges[(*minElement).verticleNumber]==0)
            break;
        if (minElement==mayEdges.end())
            return;
        ++minElement;
    }

    if (i==threadCount-1){portion=mayEdges.size()-(threadCount-1)*portion;}
    int j=1;

    for(deque<Edge>::iterator i=minElement+1;(i!=mayEdges.end() && j!=portion);++i,++j){
        if ((*i).edgeWeight<(*minElement).edgeWeight && !sawEdges[(*i).verticleNumber]){
            minElement=i;
        }
    }
    lock_guard<mutex> locked(report_mutex);
    if ((*minElement).edgeWeight<(*minEdge).edgeWeight){
        minEdge=minElement;
    }
}

IGraph ParallelSpanTreeGraph::minSpaningTree(){
    int chooseVerticleNumber=1,counter=0;
    deque<Edge>::iterator minElement;
    sawEdges= new bool[verticleCount];// какой-то бред записан
    for(int i=0;i<verticleCount;++i)
        sawEdges[i]=false;

    //начинаю с 0 вершины, заношу всё в очередь mayEdges
    deque<Edge>::iterator iter;
    iter = edges[0].begin();
    while (iter != edges[0].end()){
        mayEdges.push_back(*iter);
        iter++;
    }
    sawEdges[0]=1;//просмотрели 0-ю вершину, с неё мы стартуем

    //далее запускаю цикл
    while(true){
        minEdge=mayEdges.begin();//по умолчанию задаю первый элемент
        //проверка на существование цикла
        while(true){
            if (minEdge==mayEdges.end() || !sawEdges[(*minEdge).verticleNumber])
                break;
            else ++minEdge;
        }

        for(int i=0; i<threadCount; ++i )
            threads.push_back(std::thread(&ParallelSpanTreeGraph::findMinEdge,this,i) );
        for( auto& t: threads )
            t.join();

        //добавляю ребро в мин остов дерево
        minSpanTreeEdges.push_back(*minEdge);
        sawEdges[(*minEdge).verticleNumber]=1;//просмотрели конкретную верщину
        ++chooseVerticleNumber;//количество просмотренных вершин увеличилось

        counter=(*minEdge).verticleNumber;
        mayEdges.erase(minEdge);//удаляю из возможных ребер

        //использовали все вершинны = построили мин остовное дерево
        if (chooseVerticleNumber==verticleCount) break;

        //заношу в очередь список всех доступных вершин
        for(iter=edges[counter].begin();iter!=edges[counter].end();++iter){
            if (sawEdges[(*iter).verticleNumber]==0)
                mayEdges.push_back(*iter);
        }

        threads.clear();
    }

    return IGraph(&minSpanTreeEdges,verticleCount);
}
