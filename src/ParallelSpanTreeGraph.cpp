#include "ParallelSpanTreeGraph.h"
#include "IGraph.h"
#include <thread>
#include <deque>
#include <iostream>

using namespace std;

ParallelSpanTreeGraph::ParallelSpanTreeGraph(int thrCount){
    threadCount=thrCount;
    if (thrCount==0){
        threadCount=thread::hardware_concurrency();//���������� ���������� �������
    }
    sawEdges=NULL;
}
ParallelSpanTreeGraph::~ParallelSpanTreeGraph(){
    if (sawEdges!=NULL) delete [] sawEdges;
}

deque<Edge>::iterator ParallelSpanTreeGraph::findMinEdgeInSec(){
    deque<Edge>::iterator minElement=mayEdges.begin();
    for(deque<Edge>::iterator i=mayEdges.begin()+1;i!=mayEdges.end();++i){
        if ((*i).edgeWeight<(*minElement).edgeWeight && !sawEdges[(*i).verticleNumber]){
            minElement=i;
        }
    }
    return minElement;
}

void ParallelSpanTreeGraph::minSpaningTree(){
 //   for( unsigned i=0; i<nthreads; ++i )
 //       threads.push_back(std::thread(report, i) );

    int chooseVerticleNumber=1,counter=0;
    deque<Edge>::iterator minElement;
    sawEdges= new bool[verticleCount];// false by default

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
        //��� ����� �����
        minElement=findMinEdgeInSec();

        //�������� ����� � ��� ����� ������
        minSpanTreeEdges.push_back(*minElement);
        sawEdges[(*minElement).verticleNumber]=1;//����������� ���������� �������
        ++chooseVerticleNumber;//���������� ������������� ������ �����������

        counter=(*minElement).verticleNumber;
        mayEdges.erase(minElement);//������ �� ��������� �����

        //������������ ��� �������� = ��������� ��� �������� ������
        if (chooseVerticleNumber==verticleCount) break;

        //������ � ������� ������ ���� ��������� ������
        for(iter=edges[counter].begin();iter!=edges[counter].end();++iter){
            mayEdges.push_back(*iter);
        }
    }

    for(deque<Edge>::iterator it=minSpanTreeEdges.begin();it!=minSpanTreeEdges.end();++it){
        cout<<(*it).parentVerticleNumber<<'-'<<(*it).verticleNumber<<endl;
    }
}
