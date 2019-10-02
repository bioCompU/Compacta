/***********************************************************************
* FILENAME: EdgeHeap.cpp
*
* DESCRIPTION: Contains the implementations for the method in the EdgeHeap
* class.
*
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
* START DATE: oct 18, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef EDGEHEAP_CPP
#define EDGEHEAP_CPP
#include "EdgeHeap.h"

Edge* EdgeHeap::getFirstEdge()
{
	return _edgeHeap.pop();
}

/* IF the node has no connections return 0, if it has out but not in connnections return 1, otherwise returns 2 */
int EdgeHeap::numberConnections(int id) 
{
	bool hasIn=false;
	bool hasOut=false;
 	auto searchTemp=_outUmap.find(id);
 	if(searchTemp!=_outUmap.end())
 		hasOut=true;
	searchTemp=_inUmap.find(id);
	if(searchTemp!=_inUmap.end())
		hasIn=true;
	if(!hasOut && !hasIn)
		return 0;
	if(!hasIn)
		return 1;
	return 2;
}

void EdgeHeap::printConnections(int id)
{
 	auto searchTemp=_outUmap.find(id);
	vector<Edge*> tempEdgelist;
 	if(searchTemp!=_outUmap.end())
 	{
		cout<<"\tout ("<<(searchTemp->second)->size()<<") : \n\t";
		for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
 		{
			 cout<<*it<<" ";
			 auto tempEdge=_edgeUmap.find(to_string(id) + "," + to_string(*it));
			 if(tempEdge!=_edgeUmap.end())
				 tempEdgelist.push_back((tempEdge->second)->value());
		 }
 	}
	searchTemp=_inUmap.find(id);
	if(searchTemp!=_inUmap.end())
 	{
	    cout<<"\n\tin("<<(searchTemp->second)->size()<<"): \n";
		for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
		{
			cout<<*it<<" ";
			 auto tempEdge=_edgeUmap.find(to_string(*it) + "," + to_string(id));
			 if(tempEdge!=_edgeUmap.end())
				 tempEdgelist.push_back((tempEdge->second)->value());
		 }
	}
	cout<<"\n\tEdgelist("<<tempEdgelist.size()<<"): ";
	for(auto it=tempEdgelist.begin();it!=tempEdgelist.end();it++)
		cout<<"\t"<<*(*it);
	cout<<endl;
};


int EdgeHeap::getInConnectionsByID(int id) 
{
	auto search = _inUmap.find(id);
	if(search != _inUmap.end())
	{
		return (search->second)->size();
	}else
		return 0;
};

int EdgeHeap::getOutConnectionsByID(int id)
 {
	auto search = _outUmap.find(id);
	if(search != _outUmap.end())
	{
		return (search->second)->size();
	}else
		return 0;
};

vector<Edge*> EdgeHeap::getEdgesByNode(int id)
{
 	auto searchTemp=_outUmap.find(id);
	vector<Edge*> tempEdgelist;
 	if(searchTemp!=_outUmap.end())
 	{
		for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
 		{

			 auto tempEdge=_edgeUmap.find(to_string(id) + "," + to_string(*it));
			 if(tempEdge!=_edgeUmap.end())
				 tempEdgelist.push_back((tempEdge->second)->value());
		 }
 	}
	searchTemp=_inUmap.find(id);
	if(searchTemp!=_inUmap.end())
 	{
		for (auto it=(searchTemp->second)->begin();it != (searchTemp->second)->end();it++)
		{
			 auto tempEdge=_edgeUmap.find(to_string(*it) + "," + to_string(id));
			 if(tempEdge!=_edgeUmap.end())
				 tempEdgelist.push_back((tempEdge->second)->value());
		 }
	}
	return tempEdgelist;
};

void EdgeHeap::deleteEdge(int node1, int node2)
{
	auto search = _edgeUmap.find(to_string(node1) + "," + to_string(node2));

	if(search != _edgeUmap.end())
	{
		auto searchTemp=_outUmap.find(node1);
	 	if(searchTemp!=_outUmap.end())
	 	{
			auto tempOutItr=find((searchTemp->second)->begin(),(searchTemp->second)->end(),node2);
			*tempOutItr=(searchTemp->second)->back();
			(searchTemp->second)->pop_back();	
	 	}
		searchTemp=_inUmap.find(node2);
		if(searchTemp!=_inUmap.end())
	 	{
			auto tempInItr=find((searchTemp->second)->begin(),(searchTemp->second)->end(),node1);
			*tempInItr=(searchTemp->second)->back();
			(searchTemp->second)->pop_back();	
	 	}
		_edgeHeap.deleteItem(search->second);
		delete search->second;
		_edgeUmap.erase(search);	
	}
}

vector<Edge*>* EdgeHeap::mergeEdges(int tnode1,int tnode2,int node3,int fla)
{
	//chooses the order of comparison, instead of creating several cases change the order of the nodes to compare
	int node1=tnode1;
	int node2=tnode2;
	int lim=2;
	if(fla==1)
	{
		node1=tnode2;
		node2=tnode1;
		lim=1;
	}
	//creates an edge vector to store 
	vector<Edge*>* tempEdgeList=new vector<Edge*>();
	for(int jk=0;jk<lim;jk++)
	{
		auto searchIn=_inUmap.find(node1);
		if(searchIn!=_inUmap.end())
		{
			//creates a list of integers each one represents an in connection between node1 and the node defined by that integer
			vector<int>* vecTemp=(searchIn->second);
			for(auto vecItr=vecTemp->begin();vecItr!=vecTemp->end();vecItr++)
			{
				auto searchVec=_edgeUmap.find(to_string(*vecItr) + "," + to_string(node1));
				if(searchVec != _edgeUmap.end())
				{
					//if the edge connection is found and its not equal to node 2 (the other node we are about to merge).
					if((*vecItr)!=node2)
					{
						 //adds a new edge between the node previously connected to node1, this new edge connects it to a new node3 (the merged product of node1 and node2).
						auto tempEdge=addEdgeUmap(*vecItr,node3,((searchVec->second)->value())->weight());
						if(tempEdge)
						{
							tempEdgeList->push_back(tempEdge);	
						} 
						 _edgeHeap.deleteItem(searchVec->second);
					}	
					_edgeUmap.erase(searchVec);	
				}
				 auto searchTemp=_outUmap.find(*vecItr);
				 if(searchTemp!=_outUmap.end())
				 {
					auto it = std::find((searchTemp->second)->begin(), (searchTemp->second)->end(),node1);
					     if (it != (searchTemp->second)->end()) 
						 { 
							 (searchTemp->second)->erase(it); 
							 if((searchTemp->second)->size()==0)
								{
									delete searchTemp->second;
									_outUmap.erase(*vecItr);
								}	
						 }
				 }
			}
			 delete vecTemp;
			 _inUmap.erase(node1);
		}
		//looks out for node1 out connections.
		auto searchOut=_outUmap.find(node1);
		if(searchOut!=_outUmap.end())
		{
			//creates a int vector, each element is the number of node connected to node1
			vector<int>* vecTemp=(searchOut->second);
			for(int indx=0;indx<vecTemp->size();indx++)
			{
				int itVal=vecTemp->at(indx);
				auto searchVec=_edgeUmap.find((to_string(node1) + "," + to_string(itVal)));
				if(searchVec != _edgeUmap.end())
				{
					if((itVal)!=node2)
					{
						//add a new edge (the result of merging the two old edges)
						auto tempEdge=addEdgeUmap(itVal,node3,((searchVec->second)->value())->weight());
						if(tempEdge)
							tempEdgeList->push_back(tempEdge);
						_edgeHeap.deleteItem(searchVec->second);
					}
					//delete the edge in the heap and hash
					_edgeUmap.erase(searchVec);	
				}
				 auto searchTemp=_inUmap.find(itVal);
				 if(searchTemp!=_inUmap.end())
				 {
					auto it = std::find((searchTemp->second)->begin(), (searchTemp->second)->end(),node1);
					     if (it != (searchTemp->second)->end()) 
						 { 
							 (searchTemp->second)->erase(it); 
							 if((searchTemp->second)->size()==0)
								{
									delete searchTemp->second;
									_inUmap.erase(itVal);
								}	
						 }
				 }
			}
		 delete vecTemp;
		 _outUmap.erase(node1);
	 	}
		node2=tnode1;
		node1=tnode2;	
	}
	return tempEdgeList;
	
}


Edge* EdgeHeap::findEdge(int node1,int node2)
{
	auto search = _edgeUmap.find(to_string(node1) + "," + to_string(node2));

	if(search != _edgeUmap.end())
	{
		return (search->second)->value();
	}else
		return NULL;
}

Edge* EdgeHeap::add(int node1,int node2,int weight){
	string se;
	if (node1<node2)
		se=to_string(node1) + "," + to_string(node2);
	else 
		se=to_string(node2) + "," + to_string(node1);
	auto search = _edgeUmap.find(se);
	if(search != _edgeUmap.end())
	{
		((search->second)->value())->addWeight(weight);
		return (search->second)->value();
	}else{
		Edge* temp=new Edge(node1,node2,weight);
		auto tempCont=_edgeHeap.add(temp);
		_edgeUmap.insert({temp->name(),tempCont});
		auto searchIn=_inUmap.find(temp->node2());
		if(searchIn!=_inUmap.end())
		{
			(searchIn->second)->push_back(temp->node1());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node1());
			_inUmap.insert({temp->node2(),vecTemp});
		}
		auto searchOut=_outUmap.find(temp->node1());
		if(searchOut!=_outUmap.end())
		{
			(searchOut->second)->push_back(temp->node2());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node2());
			_outUmap.insert({temp->node1(),vecTemp});
		}	
		return temp;
	}
}

Edge* EdgeHeap::addEdgeUmap(int node1,int node2,int weight){
	string se;
	if (node1<node2)
		se=to_string(node1) + "," + to_string(node2);
	else 
		se=to_string(node2) + "," + to_string(node1);
	auto search = _edgeUmap.find(se);
	if(search != _edgeUmap.end())
	{
		((search->second)->value())->addWeight(weight);
		if((search->second)->isDisconnected())
		{
			return nullptr;
		}else
		{
			return (search->second)->value();
		}
	}else{
		Edge* temp=new Edge(node1,node2,weight);
		auto tempCont=_edgeHeap.createWrapper(temp);
		_edgeUmap.insert({temp->name(),tempCont});
		auto searchIn=_inUmap.find(temp->node2());
		if(searchIn!=_inUmap.end())
		{
			(searchIn->second)->push_back(temp->node1());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node1());
			_inUmap.insert({temp->node2(),vecTemp});
		}
		auto searchOut=_outUmap.find(temp->node1());
		if(searchOut!=_outUmap.end())
		{
			(searchOut->second)->push_back(temp->node2());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node2());
			_outUmap.insert({temp->node1(),vecTemp});
		}	
		return temp;
	}
}

void EdgeHeap::add(Edge* ptr,int AdFlag){
	auto search = _edgeUmap.find(ptr->name());
	if(search == _edgeUmap.end())
	{
		auto tempCont=_edgeHeap.add(ptr);
		_edgeUmap.insert({ptr->name(),tempCont});
		auto searchIn=_inUmap.find(ptr->node2());
		if(searchIn!=_inUmap.end())
		{
			(searchIn->second)->push_back(ptr->node1());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(ptr->node1());
			_inUmap.insert({ptr->node2(),vecTemp});
		}
		
		auto searchOut=_outUmap.find(ptr->node1());
		if(searchOut!=_outUmap.end())
		{
			(searchOut->second)->push_back(ptr->node2());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(ptr->node2());
			_outUmap.insert({ptr->node1(),vecTemp});
		}	
	}else if(AdFlag==1)
	{
		if(_edgeHeap.isNodeDisconnected((search->second)))
		{
			_edgeHeap.add(search->second);	
		}
		else
		{
			_edgeHeap.reorder(search->second);
		}
	}
}


vector<int>* EdgeHeap::getAllNodesConnected(int id)
{
	vector<int>* tempList=new vector<int>;
 	auto searchTemp=_outUmap.find(id);
 	if(searchTemp!=_outUmap.end())
 	{
		tempList->insert(tempList->end(),(searchTemp->second)->begin(), (searchTemp->second)->end());	
 	}
	searchTemp=_inUmap.find(id);
	if(searchTemp!=_inUmap.end())
 	{
		tempList->insert(tempList->end(),(searchTemp->second)->begin(), (searchTemp->second)->end());	
 	}
	return tempList;
	
}	

void EdgeHeap::printConnectionsByID(ostream& out,int id)
{
	auto search = _inUmap.find(id);
	if(search != _inUmap.end())
	{	
		out<<"\tin connections:\n";
		for(auto initr=(search->second)->begin(); initr!=(search->second)->end(); initr++)
		{
			auto tempEdge=findEdge((*initr),id);
			out<<"\t"<<(*initr)<<" "<<id<<" "<<tempEdge->weight()<<"\n";
		}
	}
	search = _outUmap.find(id);
	if(search != _outUmap.end())
	{	
		out<<"\tout connections:\n";
		for(auto initr=(search->second)->begin(); initr!=(search->second)->end(); initr++)
		{
			auto tempEdge=findEdge(id,(*initr));
			out<<"\t"<<id<<" "<<(*initr)<<" "<<tempEdge->weight()<<"\n";
		}
	}
	
}


#endif /* end of include guard: EDGEHEAP_CPP */
