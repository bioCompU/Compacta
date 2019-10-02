/***********************************************************************
* FILENAME: Clusterizer.h
*
* DESCRIPTION:
*	Contains the declarations and definitons of the Clusterizer class
*	An object of this class stores the transcripts, edges, and clusters 
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
* START DATE: september 25, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef CLUSTERIZER_H
#define CLUSTERIZER_H

#include <string>
#include "TranscriptList.h"
#include "EdgeHeap.h"
#include "EdgeList.h"
#include "ClusterTree.h"
using namespace std;

class Clusterizer
{
private:
	/* parameters of the algorithm. */
	int _nSamples;
	int _nThreads;
	string _sampleNames;
	string _fastaName;
	string* _fileNames;
	string _prefix;
	/* 	Objects to store the transcripts, edges and clusters.*/	
	TranscriptList* _transList;
	EdgeList* _edgeList;
	EdgeHeap* _edgeHeap;
	ClusterTree * _clusterTree;

public:
	Clusterizer():_nSamples(0), _nThreads(1),_prefix(""), _fileNames(NULL), _transList(NULL), _edgeHeap(NULL), _clusterTree(NULL){};
	Clusterizer (int nsamples,int nthreads):_nSamples(nsamples),_nThreads(nthreads){
		_fileNames = NULL;
		_transList = new TranscriptList();
		_edgeHeap = new EdgeHeap();
		_edgeList= new EdgeList();
		_clusterTree= NULL;
		_prefix="";
		_fastaName="";
		_sampleNames="";
	};
	~Clusterizer (){
		delete [] _fileNames;
		delete _transList;
		delete _edgeHeap;
		delete _clusterTree;
	};
	void setNsamples(int val) { _nSamples = val; };
	void setSampleNames(string val){ _sampleNames=val;};
	void setFastaName(string val){_fastaName=val;};
	void setFileNames(string);
	void setPrefix(string val){ _prefix=val;};
	void loadBamFiles();
	void loadComFiles();
	void loadEqClassesFiles();
	void removeLowEvidenceTranscript(float sizeCoef);
	void createPreclusters();
	void saveToFile();
	void printTranscriptsInCluster(Cluster*);
	void processPreclusters(float);
	void saveResultsToFile();
	void printPrecluster();
	void checkLowConnections(float);
	void savePreclustersToFile();
	void checkRepresentative(int);
};








#endif /* end of include guard: CLUSTERIZER_H */
