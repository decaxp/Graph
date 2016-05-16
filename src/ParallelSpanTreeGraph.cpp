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
    lock_guard<mutex> locked(report_mutex);
    //сколько обрабатывает поток. если последний поток, то пусть обрабатывает все до остатка
    int portion= mayEdges.size()/threadCount;
    deque<Edge>::iterator minElement=mayEdges.begin()+i*portion;//первый просматриваемый элемент

    //проверка на существование цикла
    while(true){
        if (sawEdges[(*minElement).verticleNumber]==0)
            break;
        if (minElement==mayEdges.end())
            return;
        cout<<"\n Increase "<<(*minElement).parentVerticleNumber<<"-"<<(*minElement).verticleNumber<<endl;
        ++minElement;
    }

    if (i==threadCount-1){portion=mayEdges.size()-(threadCount-1)*portion;}
    int j=1;

    cout<<"Thread "<<i<<" portion "<<portion<<" size "<<mayEdges.size()<<" start Index "<<i*portion;
    for(deque<Edge>::iterator i=minElement+1;(i!=mayEdges.end() && j!=portion);++i,++j){
        if ((*i).edgeWeight<(*minElement).edgeWeight && !sawEdges[(*i).verticleNumber]){
            minElement=i;
        }
    }
    cout<<" minElement is "<<(*minElement).parentVerticleNumber<<"-"<<(*minElement).verticleNumber;
    cout<<" minEdge is "<<(*minEdge).parentVerticleNumber<<"-"<<(*minEdge).verticleNumber<<endl;
    if ((*minElement).edgeWeight<(*minEdge).edgeWeight){
        minEdge=minElement;
    }
}

void ParallelSpanTreeGraph::minSpaningTree(){
    int chooseVerticleNumber=1,counter=0;
    deque<Edge>::iterator minElement;
    sawEdges= new bool[verticleCount];// false by default
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


        for(deque<Edge>::iterator it=mayEdges.begin();it!=mayEdges.end();++it){
            cout<<(*it).parentVerticleNumber<<'-'<<(*it).verticleNumber<<"\t";
        }
        cout<<endl;
        for(int iz=0;iz<verticleCount;++iz){
            cout<<sawEdges[iz]<<"\t";
        }
        cout<<"\niteration "<<chooseVerticleNumber<<endl;
        for(int i=0; i<threadCount; ++i )
            threads.push_back(std::thread(&ParallelSpanTreeGraph::findMinEdge,this,i) );
        for( auto& t: threads )
            t.join();
        cout<<"Result min is "<<(*minEdge).parentVerticleNumber<<'-'<<(*minEdge).verticleNumber<<endl;
        cout<<endl;
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

    for(deque<Edge>::iterator it=minSpanTreeEdges.begin();it!=minSpanTreeEdges.end();++it){
        cout<<(*it).parentVerticleNumber<<'-'<<(*it).verticleNumber<<endl;
    }
}
