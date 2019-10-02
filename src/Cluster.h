/***********************************************************************
* FILENAME: Cluster.h
*
* DESCRIPTION:
*	Contains all the definitions and declarations of the cluster class
*	An object of this class stores the information of a cluster (transcripts and subclusters).
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
* START DATE:september 23,2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef CLUSTER_H
#define CLUSTER_H

#include <set>
#include "Transcript.h"

class Cluster
{
private:
	int _Id;
	int _nSamples;
	long _nReads;
	float *_Counts;
	set<int> _transList;
	vector<Cluster*> _subClusters;

public:
	Cluster (int id,int samples):_Id(id), _nSamples(samples),_nReads(0){_Counts=new float[samples]();};
	Cluster (int samples) : _nSamples(samples),_nReads(0){ _Counts=new float[samples](); }; 
	~Cluster (){
		delete[] _Counts;
		for(auto it=_subClusters.begin();it!=_subClusters.end();it++)
		{
			delete *it;
		}
	};
	bool operator==(const int obj2){ return _Id==obj2;};
	bool addTranscript(Transcript* id){
		std::pair<std::set<int>::iterator,bool> ret;
		 ret=_transList.insert(id->getID());
		 if(ret.second)
		 {
	  		for(int i=0;i<_nSamples;i++)
	  			_Counts[i] +=  id->getCounts(i);
		 }
		 _nReads+=id->getNumberReads();
		 return ret.second;
	  };
  	string getAllCounts() { 
  		string s;
  		for(int i=0;i<_nSamples;i++)
  			s=s+to_string(_Counts[i])+"\t";
		s.pop_back();
  		return s;
  	};
	 void removeTranscript(int id){
		 _transList.erase(id);
	 } 
	void setID(int id) { _Id=id; };
	int getID() { return _Id; };
	int getSubClusterSize() { return _subClusters.size(); };
	int getNumberTranscripts() { return _transList.size(); };
	long getNumberReads(){ return _nReads; };
	void mergeClusters(Cluster* clus){
		for(auto it=clus->transBegin();it!=clus->transEnd();it++)
			_transList.insert((*it));
		for(auto it=clus->subClustersBegin();it!=subClustersEnd();it++)
			_subClusters.push_back(*it);
		for(int i=0;i<_nSamples;i++)
			_Counts[i]+=clus->getCountsSample(i);
		_nReads+= clus->getNumberReads();
	};
	
	float getCountsSample(int sample) {return (sample<_nSamples) ? _Counts[sample] : -1; };
	void addSubCluster(Cluster* id) { 
		_subClusters.push_back(id);
	 };
	 set<int>::iterator transBegin(){ return _transList.begin(); };
	 set<int>::iterator transEnd(){ return _transList.end(); };
	 vector<Cluster*>::iterator subClustersBegin() { return _subClusters.begin(); };
	 vector<Cluster*>::iterator subClustersEnd() { return _subClusters.end(); };
	 friend ostream& operator <<(ostream& out, const Cluster &obj)
	 {
		 out<<" cluster "<<obj._Id<<" nSamples: "<<obj._nSamples<<" counts: ";
		 for(int i=0;i<obj._nSamples;i++) 
			 out<<obj._Counts[i]<<" ";
		 out<<"\n\tTranscript List ("<<obj._transList.size()<<"): ";
		 int i=0;
		 for(auto it=obj._transList.begin();it!=obj._transList.end();it++,i++)
			 out<<i<<":"<<*it<<" ";
		 out<<"\n<<<<<<<<<\n";
	 }
	 void print()
	 {
		 cout<<" cluster "<<_Id<<" nSamples: "<<_nSamples<<" counts: ";
		 for(int i=0;i<_nSamples;i++) 
			 cout<<_Counts[i]<<" ";
		 cout<<"\tTranscript List ("<<_transList.size()<<"): \n";
		 int i=0;
		 for(auto it=_transList.begin();it!=_transList.end();it++,i++)
		 			 cout<<i<<":"<<*it<<" ";
		 cout<<"\tSubcluster list ("<<_subClusters.size()<<"): \n";
		 for(auto it=_subClusters.begin();it!=_subClusters.end();it++)
			 cout<<"sc:"<<(*it)->getID()<<" transcripts "<<(*it)->getNumberTranscripts()<<" subclus: "<<(*it)->getSubClusterSize()<<endl;
		 cout<<"\n<<<<<<<<<\n";
	 }
};


#endif /* end of include guard: CLUSTER_H */
