/***********************************************************************
* FILENAME: ClusterTree.h
*
* DESCRIPTION:
*	Contains all the definitions and declarations of the ClusterTree class
*	An object of this class creates a hierarchy structure with the clusters.
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
* START DATE: september 23, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef CLUSTERTREE_H
#define CLUSTERTREE_H

#include "Cluster.h"

class ClusterTree
{
private:
	Cluster* _root;
	int _nSamples;
	int _nTranscripts;
	int * _transcriptClusterMap;

public:
	ClusterTree (int samples,int transcripts) : _nSamples(samples), _nTranscripts(transcripts) {
		 _root=new Cluster(0,samples,1);
		  _transcriptClusterMap=new int[transcripts](); 
	 };
	~ClusterTree () {
		 delete _root;
	 	 delete [] _transcriptClusterMap;
	 };
	 void addCluster(Cluster* parent,Cluster *subcluster){
		 subcluster->setID(parent->getSubClusterSize());
		 parent->addSubCluster(subcluster);
		 for(auto it=subcluster->transBegin();it!=subcluster->transEnd();it++)
		 {
			 _transcriptClusterMap[*it]=subcluster->getID();
				 parent->removeTranscript(*it);
		 }
	 }
	 int numberPreclusters() { return _root->getSubClusterSize(); };
	 void setRoot(Cluster *clust){ delete _root; _root=clust; };
	 Cluster* getRoot() { return _root; };
	 vector<Cluster*>::iterator subClustBegin(){ return _root->subClustersBegin(); };
	 vector<Cluster*>::iterator subClustEnd(){ return _root->subClustersEnd(); };
	 void printClusterTree()
	 {
		 cout<<">>> Root <<<\n "<<_root->getNumberTranscripts()<<" Disconnected Transcripts, "<<_root->getSubClusterSize()<<" subclusters: \n";
		 for(auto itr=_root->subClustersBegin();itr!=_root->subClustersEnd();itr++)
		{
			cout<<"subcluster: "<<(*itr)->getID()<<" transcripts: "<<(*itr)->getNumberTranscripts()<<" subclusters: "<<(*itr)->getSubClusterSize()<<" counts:";
			for(int i=0;i<_nSamples;i++)
				cout<<(*itr)->getCountsSample(i)<<" ";
			cout<<"\n";
			if((*itr)->getID()==37)
				(*itr)->print();
		}	 
		 
	 }
	 void printClusterTreeToFile(ofstream &out)
	 {
		 int counter=_root->getNumberTranscripts();
		 out<<">>> Root <<<\n "<<_root->getNumberTranscripts()<<" Disconnected Transcripts, "<<_root->getSubClusterSize()<<" subclusters: \n";
		 cout<<" Precluster_ID\tnContigs\tcontigIDs\n";
		 for(auto itr=_root->subClustersBegin();itr!=_root->subClustersEnd();itr++)
		{
			out<<"subcluster: "<<counter + (*itr)->getID()<<" transcripts: "<<(*itr)->getNumberTranscripts()<<" subclusters: "<<(*itr)->getSubClusterSize()<<" counts:";
			for(auto trItr=(*itr)->transBegin();trItr!=(*itr)->transEnd();trItr++)
				out<<(*trItr)<<",";
			out<<"\b\n";
		}
	 }
	 
};

#endif /* end of include guard: CLUSTERTREE_H */
