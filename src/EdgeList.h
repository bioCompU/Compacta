/***********************************************************************
* FILENAME: EdgeList.h
*
* DESCRIPTION:
*	Contains the definitions and declarations of the class EdgeList
*	An object of this class can store the edges (connections between transcripts)
*	and their weights, with a hash interface to find the edges fast.
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
* START DATE: september 20, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef EDGELIST_H
#define EDGELIST_H

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Edge.h"
#include "TranscriptList.h"

using namespace std;

class EdgeList
{
	typedef vector<Edge*> edgeTList;
private:
	edgeTList edgeList_;
	unordered_map<string, Edge*> * _edgeUmap;
	unordered_map<int,vector<int>*> *inUmap_;
	unordered_map<int,vector<int>*> *outUmap_;


public:
	typedef typename edgeTList::iterator iterator;
	EdgeList (){
		inUmap_=new unordered_map<int,vector<int>*>;
		outUmap_=new unordered_map<int,vector<int>*>; 
		_edgeUmap=new unordered_map<string, Edge*>;
	};
	EdgeList( int size ) {
		 edgeList_.reserve(size); 
 		inUmap_=new unordered_map<int,vector<int>*>;
 		outUmap_=new unordered_map<int,vector<int>*>; 
		_edgeUmap=new unordered_map<string, Edge*>;
	 };
	 
	~EdgeList (){
		auto itr=edgeList_.begin();
		for(;itr!=edgeList_.end();itr++)
			delete *itr;
	};
	Edge* add(int, int, int);
	void add(Edge*);
	bool deleteNode(int);
	void deleteEdges();
	void deleteEdge(int,int);
	int size() { return edgeList_.size(); };
	Edge* findEdge(int,int);
	int inSize(){ return inUmap_->size(); };
	int outSize(){ return outUmap_->size(); };
	vector<int>* getAllNodesConnected(int id);
	vector<Edge*>* mergeEdges(int,int,int,int);
	void sortEdges() { sort(edgeList_.begin(),edgeList_.end(),[]( Edge* obj1, Edge* obj2){ return (obj1->distance() > obj2->distance()); }); };
	/* IF the node has no connections return 0, if it has out but not in connnections return 1, otherwise returns 2 */
	int NumberConnections(int id) {
		bool hasIn=false;
		bool hasOut=false;
	 	auto searchTemp=outUmap_->find(id);
	 	if(searchTemp!=outUmap_->end())
	 		hasOut=true;
		searchTemp=inUmap_->find(id);
		if(searchTemp!=inUmap_->end())
			hasIn=true;
		if(!hasOut && !hasIn)
			return 0;
		if(!hasIn)
			return 1;
		return 2;
	}
	void printConnections(int id){
	 	auto searchTemp=outUmap_->find(id);
		vector<Edge*> tempEdgelist;
	 	if(searchTemp!=outUmap_->end())
	 	{
			cout<<"\tout ("<<(searchTemp->second)->size()<<") : \n\t";
			for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
	 		{
				 cout<<*it<<" ";
				 auto tempEdge=_edgeUmap->find(to_string(id) + "," + to_string(*it));
				 if(tempEdge!=_edgeUmap->end())
					 tempEdgelist.push_back(tempEdge->second);
			 }
	 	}
		searchTemp=inUmap_->find(id);
		if(searchTemp!=inUmap_->end())
	 	{
		    cout<<"\n\tin("<<(searchTemp->second)->size()<<"): \n";
			for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
			{
				cout<<*it<<" ";
				 auto tempEdge=_edgeUmap->find(to_string(*it) + "," + to_string(id));
				 if(tempEdge!=_edgeUmap->end())
					 tempEdgelist.push_back(tempEdge->second);
			 }
		}
		cout<<"\n\tEdgelist("<<tempEdgelist.size()<<"): ";
		for(auto it=tempEdgelist.begin();it!=tempEdgelist.end();it++)
			cout<<"\t"<<*(*it);
		cout<<endl;
	};
	int getInConnectionsByID(int id) {
		auto search = inUmap_->find(id);
		if(search != inUmap_->end())
		{
			return (search->second)->size();
		}else
			return 0;
	};
	int getOutConnectionsByID(int id) {
		auto search = outUmap_->find(id);
		if(search != outUmap_->end())
		{
			return (search->second)->size();
		}else
			return 0;
	};
	friend ostream& operator<<(ostream& out, EdgeList& obj)
	{
		auto itr=obj.edgeList_.begin();
		for(;itr!=obj.edgeList_.end();itr++)
			out<<**itr<<"\n";
		out<<endl<<"in connections: \n";
		for(auto it=obj.inUmap_->begin();it!=obj.inUmap_->end();it++)
		{
			out<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				out<<*itt<<" ";
			out<<endl;
		}
		out<<endl<<"out connections: \n";
		for(auto it=obj.outUmap_->begin();it!=obj.outUmap_->end();it++)
		{
			out<<"\t"<<it->first<<" "<<(it->second)->size()<<" :  ";
			for(auto itt=(it->second)->begin();itt!=(it->second)->end();itt++)
				out<<*itt<<" ";
			out<<endl;
		}
			
		return out;
	};
	void printConnectionsByID(ostream& out,int id)
	{
		auto search = inUmap_->find(id);
		if(search != inUmap_->end())
		{	
			out<<"\tin connections:\n";
			for(auto initr=(search->second)->begin(); initr!=(search->second)->end(); initr++)
			{
				auto tempEdge=findEdge((*initr),id);
				out<<"\t"<<(*initr)<<" "<<id<<" "<<tempEdge->weight()<<"\n";
			}
		}
		search = outUmap_->find(id);
		if(search != outUmap_->end())
		{	
			out<<"\tout connections:\n";
			for(auto initr=(search->second)->begin(); initr!=(search->second)->end(); initr++)
			{
				auto tempEdge=findEdge(id,(*initr));
				out<<"\t"<<id<<" "<<(*initr)<<" "<<tempEdge->weight()<<"\n";
			}
		}
		
	}
	vector<Edge*> getEdgesByNode(int id)
	{
	 	auto searchTemp=outUmap_->find(id);
		vector<Edge*> tempEdgelist;
	 	if(searchTemp!=outUmap_->end())
	 	{
			int contador=0;
			int contador2=0;
			for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
	 		{
				
				 auto tempEdge=_edgeUmap->find(to_string(id) + "," + to_string(*it));
				 if(tempEdge!=_edgeUmap->end())
				{
					tempEdgelist.push_back(tempEdge->second);
				}	 
			 }
	 	}
		searchTemp=inUmap_->find(id);
		if(searchTemp!=inUmap_->end())
	 	{
			for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
			{
				 auto tempEdge=_edgeUmap->find(to_string(*it) + "," + to_string(id));
				 if(tempEdge!=_edgeUmap->end())
					 tempEdgelist.push_back(tempEdge->second);
			 }
		}
		return tempEdgelist;
	};

	iterator begin(){return edgeList_.begin(); };
	iterator end(){ return edgeList_.end(); };
	unordered_map<int,vector<int>*>::iterator inBegin(){return inUmap_->begin(); };
	unordered_map<int,vector<int>*>::iterator inEnd(){ return inUmap_->end(); };
	unordered_map<int,vector<int>*>::iterator outBegin(){ return outUmap_->begin(); };
	unordered_map<int,vector<int>*>::iterator outEnd(){ return outUmap_->end(); };
};




#endif /* end of include guard: EDGELIST_H */
