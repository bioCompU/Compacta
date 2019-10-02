/***********************************************************************
* FILENAME: EdgeHeap.h
*
* DESCRIPTION:
*	Contains the definitions and declarations of the EdgeHeap class
*	An object of this class contains the edges between vertices, an interface
* 	to search them, and internally uses a heap to return the edge with smallest
*	distance. 
*
* PUBLIC FUNCTIONS:
*	type function_name(param_type)
*
* NOTES:
*		Please cite this software in publications where you made 
*		use of it for any part of the data analysis.
*
*		Copyright 2017, Fernando Razo. This program is distributed
*		under the GNU General Public License.
*
* AUTHOR:	Fernando Razo. razoto@gmail.com.
*
* START DATE: october 18, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef EDGEHEAP_H
#define EDGEHEAP_H
#include <iostream>
#include <unordered_map>
#include "Edge.h"
#include "Heap.h"


#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

class EdgeHeap
{
private:
	Heap<Edge*> _edgeHeap;
	unordered_map<string, HeapContainer<Edge*>*> _edgeUmap;
	unordered_map<int,vector<int>*> _inUmap;
	unordered_map<int,vector<int>*> _outUmap;
public:
	EdgeHeap (){};
	~EdgeHeap (){};
	int size(){ return _edgeHeap.size(); };
	int inSize(){ return _inUmap.size(); };
	int outSize(){ return _outUmap.size(); };
	void add(Edge*,int);
	Edge* add(int,int,int);
	void deleteNode(int);
	void deleteEdge(int,int);
	Edge* findEdge(int,int);
	Edge* getFirstEdge();
	vector<Edge*> getEdgesByNode(int);
	vector<int>* getAllNodesConnected(int);
	vector<Edge*>* mergeEdges(int,int,int,int);
	int numberConnections(int);
	int getInConnectionsByID(int);
	int getOutConnectionsByID(int);
	void printConnections(int);
	void printConnectionsByID(ostream&,int);
	Edge* addEdgeUmap(int,int,int);
	void printAll(){
		cout<<"=====Edgeheap.Printall==========="<<endl;
		cout<<"Edge heap\n";
		_edgeHeap.print();
		cout<<endl<<"in connections: \n";
		for(auto it=_inUmap.begin();it!=_inUmap.end();it++)
		{
			cout<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				cout<<*itt<<" ";
			cout<<endl;
		}
		cout<<endl<<"out connections: \n";
		for(auto it=_outUmap.begin();it!=_outUmap.end();it++)
		{
			cout<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				cout<<*itt<<" ";
			cout<<endl;
		}
		cout<<"..........................................................."<<endl;
	};
	
	friend ostream& operator<<(ostream& out, EdgeHeap& obj)
	{
		out<<"Edge heap\n";
		out<<obj._edgeHeap<<"\n";
		out<<endl<<"in connections: \n";
		for(auto it=obj._inUmap.begin();it!=obj._inUmap.end();it++)
		{
			out<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				out<<*itt<<" ";
			out<<endl;
		}
		out<<endl<<"out connections: \n";
		for(auto it=obj._outUmap.begin();it!=obj._outUmap.end();it++)
		{
			out<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				out<<*itt<<" ";
			out<<endl;
		}
			
		return out;
	};
	unordered_map<string, HeapContainer<Edge*>*>::iterator begin(){return _edgeUmap.begin(); };
	unordered_map<string, HeapContainer<Edge*>*>::iterator end(){ return _edgeUmap.end(); };
	unordered_map<int,vector<int>*>::iterator inBegin(){return _inUmap.begin(); };
	unordered_map<int,vector<int>*>::iterator inEnd(){ return _inUmap.end(); };
	unordered_map<int,vector<int>*>::iterator outBegin(){ return _outUmap.begin(); };
	unordered_map<int,vector<int>*>::iterator outEnd(){ return _outUmap.end(); };
	
};



#endif /* end of include guard: EDGEHEAP_H */

