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
    //������� ������������ �����. ���� ��������� �����, �� ����� ������������ ��� �� �������
    int portion= mayEdges.size()/threadCount;
    deque<Edge>::iterator minElement=mayEdges.begin()+i*portion;//������ ��������������� �������

    //�������� �� ������������� �����
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
    sawEdges= new bool[verticleCount];// �����-�� ���� �������
    for(int i=0;i<verticleCount;++i)
        sawEdges[i]=false;

    //������� � 0 �������, ������ �� � ������� mayEdges
    deque<Edge>::iterator iter;
    iter = edges[0].begin();
    while (iter != edges[0].end()){
        mayEdges.push_back(*iter);
        iter++;
    }
    sawEdges[0]=1;//����������� 0-� �������, � �� �� ��������

    //����� �������� ����
    while(true){
        minEdge=mayEdges.begin();//�� ��������� ����� ������ �������
        //�������� �� ������������� �����
        while(true){
            if (minEdge==mayEdges.end() || !sawEdges[(*minEdge).verticleNumber])
                break;
            else ++minEdge;
        }

        for(int i=0; i<threadCount; ++i )
            threads.push_back(std::thread(&ParallelSpanTreeGraph::findMinEdge,this,i) );
        for( auto& t: threads )
            t.join();

        //�������� ����� � ��� ����� ������
        minSpanTreeEdges.push_back(*minEdge);
        sawEdges[(*minEdge).verticleNumber]=1;//����������� ���������� �������
        ++chooseVerticleNumber;//���������� ������������� ������ �����������

        counter=(*minEdge).verticleNumber;
        mayEdges.erase(minEdge);//������ �� ��������� �����

        //������������ ��� �������� = ��������� ��� �������� ������
        if (chooseVerticleNumber==verticleCount) break;

        //������ � ������� ������ ���� ��������� ������
        for(iter=edges[counter].begin();iter!=edges[counter].end();++iter){
            if (sawEdges[(*iter).verticleNumber]==0)
                mayEdges.push_back(*iter);
        }

        threads.clear();
    }

    return IGraph(&minSpanTreeEdges,verticleCount);
}
