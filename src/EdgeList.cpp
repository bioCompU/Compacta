/***********************************************************************
* FILENAME: EdgeList.cpp
*
* DESCRIPTION:
*	Contains the method implementations of the class EdgeList.
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
* START DATE:
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#include "EdgeList.h"


vector<Edge*>* EdgeList::mergeEdges(int tnode1,int tnode2,int node3,int fla)
{
	int node1=tnode1;
	int node2=tnode2;
	int lim=2;
	if(fla==1)
	{
		node1=tnode2;
		node2=tnode1;
		lim=1;
	}
	vector<Edge*>* tempEdgeList=new vector<Edge*>();

	for(int jk=0;jk<lim;jk++)
	{
		auto searchIn=inUmap_->find(node1);
		if(searchIn!=inUmap_->end())
		{
			vector<int>* vecTemp=(searchIn->second);
			for(auto vecItr=vecTemp->begin();vecItr!=vecTemp->end();vecItr++)
			{
				auto searchVec=_edgeUmap->find(to_string(*vecItr) + "," + to_string(node1));
				if(searchVec != _edgeUmap->end())
				{
					if(*vecItr!=node2)
					{
						 auto tempEdge=add(*vecItr,node3,(searchVec->second)->weight());
						 tempEdgeList->push_back(tempEdge);
					}	
					 auto it=std::find((edgeList_.begin()),edgeList_.end(),searchVec->second);
					 if(it!=edgeList_.end())
						 edgeList_.erase(it);
					_edgeUmap->erase(searchVec);	
				}
				 auto searchTemp=outUmap_->find(*vecItr);
				 if(searchTemp!=outUmap_->end())
				 {
					auto it = std::find((searchTemp->second)->begin(), (searchTemp->second)->end(),node1);
					     if (it != (searchTemp->second)->end()) 
						 { 
							 (searchTemp->second)->erase(it); 
							 if((searchTemp->second)->size()==0)
								{
									delete searchTemp->second;
									outUmap_->erase(*vecItr);
								}	
						 }
				 }
			}
			 delete vecTemp;
			 inUmap_->erase(node1);
		}
	
		auto searchOut=outUmap_->find(node1);
		if(searchOut!=outUmap_->end())
		{
			vector<int>* vecTemp=(searchOut->second);
			for(auto vecItr=vecTemp->begin();vecItr!=vecTemp->end();vecItr++)
			{
				auto searchVec=_edgeUmap->find((to_string(node1) + "," + to_string(*vecItr)));
				if(searchVec != _edgeUmap->end())
				{
					if(*vecItr!=node2)
					{
						auto tempEdge=add(*vecItr,node3,(searchVec->second)->weight());
						tempEdgeList->push_back(tempEdge);
					}
					 auto it=std::find((edgeList_.begin()),edgeList_.end(),searchVec->second);
					 if(it!=edgeList_.end())
						 edgeList_.erase(it);
					_edgeUmap->erase(searchVec);	
				}
				 auto searchTemp=inUmap_->find(*vecItr);
				 if(searchTemp!=inUmap_->end())
				 {
					auto it = std::find((searchTemp->second)->begin(), (searchTemp->second)->end(),node1);
					     if (it != (searchTemp->second)->end()) 
						 { 
							 (searchTemp->second)->erase(it); 
							 if((searchTemp->second)->size()==0)
								{
									delete searchTemp->second;
									inUmap_->erase(*vecItr);
								}	
						 }
				 }		
			}
		 delete vecTemp;
		 outUmap_->erase(node1);
	 	}
		node2=tnode1;
		node1=tnode2;	
	}
	
	return tempEdgeList;
	
}

Edge* EdgeList::findEdge(int node1,int node2)
{
	auto search = _edgeUmap->find(to_string(node1) + "," + to_string(node2));

	if(search != _edgeUmap->end())
	{
		return search->second;
	}else
		return NULL;
}

Edge* EdgeList::add(int node1,int node2,int weight){
	string se;
	if (node1<node2)
		se=to_string(node1) + "," + to_string(node2);
	else 
		se=to_string(node2) + "," + to_string(node1);
	auto search = _edgeUmap->find(se);

	if(search != _edgeUmap->end())
	{
		(search->second)->addWeight(weight);
		return search->second;
	}else{
		Edge* temp=new Edge(node1,node2,weight);
		edgeList_.push_back(temp);
		_edgeUmap->insert({temp->name(),temp});
		auto searchIn=inUmap_->find(temp->node2());
		if(searchIn!=inUmap_->end())
		{
			(searchIn->second)->push_back(temp->node1());
		}else
		{
			
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node1());
			inUmap_->insert({temp->node2(),vecTemp});
		}
		auto searchOut=outUmap_->find(temp->node1());
		if(searchOut!=outUmap_->end())
		{
			(searchOut->second)->push_back(temp->node2());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(temp->node2());
			outUmap_->insert({temp->node1(),vecTemp});
		}	
		return temp;
	}
}

void EdgeList::add(Edge* ptr){
	auto search = _edgeUmap->find(ptr->name());
	if(search == _edgeUmap->end())
	{
		edgeList_.push_back(ptr);
		_edgeUmap->insert({ptr->name(),ptr});
		auto searchIn=inUmap_->find(ptr->node2());
		if(searchIn!=inUmap_->end())
		{
			(searchIn->second)->push_back(ptr->node1());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(ptr->node1());
			inUmap_->insert({ptr->node2(),vecTemp});
		}
		
		auto searchOut=outUmap_->find(ptr->node1());
		if(searchOut!=outUmap_->end())
		{
			(searchOut->second)->push_back(ptr->node2());
		}else
		{
			vector<int> *vecTemp=new vector<int>;
			vecTemp->push_back(ptr->node2());
			outUmap_->insert({ptr->node1(),vecTemp});
		}	
	}
}

bool EdgeList::deleteNode(int node)
{
	bool val=false;
	 auto search=inUmap_->find(node);
	 if(search!=inUmap_->end())
	 {
		 vector<int>* tempIn=search->second;
		 vector<int>::iterator itr=tempIn->begin();
		 for(;itr!=tempIn->end();itr++)
		 {
			 auto searchNode=_edgeUmap->find(to_string(*itr) + "," + to_string(node));
			 if(searchNode!=_edgeUmap->end())
			 {
				 auto it=std::find((edgeList_.begin()),edgeList_.end(),searchNode->second);
				 if(it!=edgeList_.end())
				 {
					 edgeList_.erase(it);
				 }
				 delete searchNode->second;
				 _edgeUmap->erase(to_string(*itr) + "," + to_string(node));
			 }
			 auto searchTemp=outUmap_->find(*itr);
			 if(searchTemp!=outUmap_->end())
			 {
				auto it = std::find((searchTemp->second)->begin(), (searchTemp->second)->end(),node);
				     if (it != (searchTemp->second)->end()) 
					 { 
						 (searchTemp->second)->erase(it); 
						 if((searchTemp->second)->size()==0)
							{
								delete searchTemp->second;
								outUmap_->erase(*itr);
							}	
					 }
			 }
		 }
		 delete tempIn;
		 inUmap_->erase(node);
		 val=true;
	 }
	 search=outUmap_->find(node);
	 if(search!=outUmap_->end())
	 {
		 vector<int>* tempOut=search->second;
		 vector<int>::iterator itr=tempOut->begin();
		 for(;itr!=tempOut->end();itr++)
		 {
			auto searchNode=_edgeUmap->find(to_string(node) + "," + to_string(*itr) );
			if(searchNode!=_edgeUmap->end())
			{
				 auto it=std::find((edgeList_.begin()),edgeList_.end(),searchNode->second);
				 if(it!=edgeList_.end())
				 {
				 	edgeList_.erase(it);
				 }
				delete searchNode->second;
				_edgeUmap->erase(to_string(node) + "," + to_string(*itr) );
			}
			auto searchTemp=inUmap_->find(*itr);
			if(searchTemp!=inUmap_->end())
			{
				auto it= std::find((searchTemp->second)->begin(),(searchTemp->second)->end(),node);
				if(it != (searchTemp->second)->end()){ 
				 	(searchTemp->second)->erase(it); 
					 if((searchTemp->second)->size()==0)
						{
							delete searchTemp->second;
							inUmap_->erase(*itr);
						}	
				}
			}
		 }
		 delete tempOut;
		 outUmap_->erase(node);
		 val=true;
	 }
	 return val;
}

vector<int>* EdgeList::getAllNodesConnected(int id)
{
	vector<int>* tempList=new vector<int>;
 	auto searchTemp=outUmap_->find(id);
 	if(searchTemp!=outUmap_->end())
 	{
		tempList->insert(tempList->end(),(searchTemp->second)->begin(), (searchTemp->second)->end());	
 	}
	searchTemp=inUmap_->find(id);
	if(searchTemp!=inUmap_->end())
 	{
		tempList->insert(tempList->end(),(searchTemp->second)->begin(), (searchTemp->second)->end());
 	}
	return tempList;
	
}	

void EdgeList::deleteEdges()
{
	auto itr=edgeList_.begin();
	for(;itr!=edgeList_.end();itr++)
	{
		if(!(*itr))
		{
			edgeList_.erase(itr);
		}
	}
}

void EdgeList::deleteEdge(int node1, int node2)
{
	auto search = _edgeUmap->find(to_string(node1) + "," + to_string(node2));

	if(search != _edgeUmap->end())
	{
		auto searchTemp=outUmap_->find(node1);
	 	if(searchTemp!=outUmap_->end())
	 	{
			auto tempOutItr=find((searchTemp->second)->begin(),(searchTemp->second)->end(),node2);
			*tempOutItr=(searchTemp->second)->back();
			(searchTemp->second)->pop_back();	
	 	}
		searchTemp=inUmap_->find(node2);
		if(searchTemp!=inUmap_->end())
	 	{
			auto tempInItr=find((searchTemp->second)->begin(),(searchTemp->second)->end(),node1);
			*tempInItr=(searchTemp->second)->back();
			(searchTemp->second)->pop_back();	
	 	}
		 auto it=std::find((edgeList_.begin()),edgeList_.end(),search->second);
		 if(it!=edgeList_.end())
			 edgeList_.erase(it);
		delete search->second;
		_edgeUmap->erase(search);	
	}
}