#include "IGraph.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

IGraph::IGraph(){edges=nullptr;}
IGraph::~IGraph(){
    delete [] edges;
    edges=nullptr;
}

IGraph::IGraph(deque<Edge>* pEdges,int pVerticleNumber){
    verticleCount=pVerticleNumber;
    edges=new deque<Edge>[verticleCount];
    Edge ed;
    for(int i=0;i<pEdges->size();++i){
        edges[(*pEdges)[i].parentVerticleNumber].push_back((*pEdges)[i]);
        ed.verticleNumber=(*pEdges)[i].parentVerticleNumber;
        ed.parentVerticleNumber=(*pEdges)[i].verticleNumber;
        ed.edgeWeight=(*pEdges)[i].edgeWeight;
        edges[(*pEdges)[i].verticleNumber].push_back(ed);
    }
}

void IGraph::readGraphFromFile(char* name){
    fstream file(name);
    std::string line,vertLine;
    getline(file,line);
    stringstream  mstream;
    mstream<<line;
    mstream>>verticleCount;

    this->edges=new deque<Edge>[verticleCount];
    int i=0;

    while(getline(file,line)){
        //получил исходную строку
        mstream.clear();
        mstream<<line;
        while(std::getline(mstream,vertLine,',')){
            //получил вершину-ребро, теперь надо разбить её
            edges[i].push_back(strToEdge(vertLine,i));
        }
        ++i;
    }

    file.close();
}

Edge IGraph::strToEdge(string str,int parentNum){
    stringstream ss;
    ss<<str;
    string line;
    int arr[2];
    int i=0;
    while(getline(ss,line,' ')){
        arr[i]=atoi(line.c_str());
        ++i;
    }
    return Edge(arr[0],parentNum,arr[1]);
}

void IGraph::Print(){
    cout<<"Print VerticleCount: "<<verticleCount<<endl;

    deque<Edge>::iterator iter;
    for (int i = 0; i < verticleCount; i++){
        iter = edges[i].begin();

        while (iter != edges[i].end()){
            cout << (*iter).verticleNumber<<" "<<(*iter).edgeWeight<<',';
            iter++;
        }
        cout<<endl;
  }
}

void IGraph::saveGraphToFile(char* name){}

IGraph IGraph::minSpaningTree(){
    return *this;
}
