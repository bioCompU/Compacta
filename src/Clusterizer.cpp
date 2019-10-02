/***********************************************************************
* FILENAME: Clusterizer.cpp
*
* DESCRIPTION:
*	Contains all the method implementation of the class Clusterizer
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

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <utility>
#include <ctime>
#include "Transcript.h"
#include "TranscriptList.h"
#include "ReadList.h"
#include "EdgeHeap.h"
#include "Cluster.h"
#include "ClusterTree.h"
#include "Clusterizer.h"
#include <sam.h>
#include <bam.h>
#include <omp.h>


using namespace std;

void Clusterizer::setFileNames(string filenames)
{
	_fileNames=new string[_nSamples];
	stringstream ssFiles(filenames);
	for(int i=0;i<_nSamples;i++)
	{
		getline(ssFiles,_fileNames[i],',');
	}
	
	if(_sampleNames=="")
	{
		for(int i=0;i<(_nSamples-1);i++)
			_sampleNames=_sampleNames + _fileNames[i] + "\t";
		_sampleNames=_sampleNames + _fileNames[_nSamples-1];
	}
}

void Clusterizer::loadEqClassesFiles()
{
	cout<<"Loading data from equivalence classes files...\n";
	int i,j,val1,val2,val3,k,nEqCla,rCount;
	string fileLine;
	Transcript* tempTrans;
	Edge* edgtemp;
	
	/* checks if the file names were set  */
	if(!_fileNames)
	{
		cerr << "ERROR: No file names were provided.... \n";
		exit(1);
	}
	
	/* 		Tries to open the transcripts file, if it fails the program exits with an error. 	 */
	
	ifstream inTrans;
	inTrans.open((_fileNames[0]+"/aux_info/eq_classes.txt"));
	if (!inTrans.is_open()) 
	{
		cerr << "file "<< (_fileNames[0]+"/aux_info/eq_classes.txt") << " could not be open.\n";
		exit(1);
	}

	inTrans>>val1;
	inTrans>>val2;
	
	nEqCla=val2;
	_transList=new TranscriptList(val1);
	
	/* 	Now that we know the number of transcripts, initiate the clusterTree object */
	_clusterTree=new ClusterTree(_nSamples,val1);

	int counter=0;
	for(i=0;i<val1;i++)
	{
		inTrans>>fileLine;
		tempTrans=new Transcript(i,fileLine,0,_nSamples);
		_transList->add(tempTrans);
	}
	inTrans.close();
	
	deque<int> tempQueue;
	_edgeList=new EdgeList();
	for(i=0;i<_nSamples;i++){  

	/* 		Tries to open the edge file, if it fails the program exits with an error. 	 */
	cout << " Reading file "<<(_fileNames[i]+"/aux_info/eq_classes.txt")<<"\n";
	inTrans.open((_fileNames[i]+"/aux_info/eq_classes.txt"));
	if (!inTrans.is_open()) 
	{
		cerr << "file "<< (_fileNames[i]+"/aux_info/eq_classes.txt") << " could't be open.\n";
		exit(1);
	}
	inTrans>>val1;
	inTrans>>val2;
	Edge * tempEdge;
	counter=0;
	for(k=0;k<val1;k++)
		inTrans>>fileLine;
	/* loops thru all the equivalence classes */
	for(int jj=0;jj<val2;jj++)
	{
		inTrans >> nEqCla;
		for (k=0;k<nEqCla; k++)
		{
			inTrans>>val3;
			tempQueue.push_back(val3);
		}
		inTrans>>rCount;
		while(tempQueue.size()>0)
		{
			/* 	Gets the transcript pointer of the alignment	 */
			tempTrans=_transList->getByID(tempQueue.front());
			if(tempTrans)
			{
				/* 	Adds the counts, and the evidence (size of the read between all the alignments) */
				tempTrans->addCounts(i,(((float) rCount)/((float) nEqCla)));
				tempTrans->addReads(1);
				for(k=1;k<tempQueue.size();k++)
				{
					edgtemp=_edgeList->add(tempTrans->getID(),tempQueue[k],1);
				}	
			}
			tempQueue.pop_front();
		}
	  }
	  	inTrans.close();
	}
	cout<<" done!\n";
	cout<<" loaded "<<_transList->size()<<" nodes with "<<_edgeList->size()<<" edges "<<endl;
	
}

void Clusterizer::loadBamFiles()
{
	cout<<"Loading data from bam files...\n";
	int i;
	string file;
	
	/* checks if the file names were set  */
	if(!_fileNames)
	{
		cerr << "ERROR: No file names were provided.... \n";
		exit(1);
	}
	
	/* 		Tries to open the first file, if it fails the program exits with an error. 	 */
	cout << " Reading header of file "<<_fileNames[0]<<"\n";
	samfile_t *inFile = 0 ;
	if ((inFile = samopen(_fileNames[0].c_str(), "br", NULL)) == 0 | inFile->header == 0) 
	{
		cerr << "file "<< _fileNames[0] << " could't be open.\n";
		exit(1);
	}
	
	
	if(_fastaName==""){
	/* 		Read the header of the first file and get all the transcripts and their size	 */	
		cout  << inFile->header->n_targets << " Transcripts in the transcriptome. "<< "\n";
		for(i = 0; i < inFile->header->n_targets; i++)
			_transList->add(inFile->header->target_name[i],inFile->header->target_len[i],_nSamples);
		samclose(inFile);
	}else{
		ifstream inTransFasta;
		inTransFasta.open((_fastaName));
		unordered_map<string,int> tlist;
		if (!inTransFasta.is_open()) 
		{
			cerr << "file "<< (_fastaName) << " could't be open.\n";
			exit(1);
		}
		string line;
		while(getline(inTransFasta, line)) {
			auto search = tlist.find(line);
			if(search == tlist.end())
				tlist.insert({line,0});
		}	
		inTransFasta.close();
		
		/* 		Read the header of the first file and get all the transcripts and their size	 */	
		cout  << inFile->header->n_targets << " Transcripts in the transcriptome. subset of "<<tlist.size()<<"\n";
		for(i = 0; i < inFile->header->n_targets; i++)
		{
			auto search = tlist.find(inFile->header->target_name[i]);
			if(search != tlist.end())
			{
				_transList->add(inFile->header->target_name[i],inFile->header->target_len[i],_nSamples);
			}
		}
		samclose(inFile);
	}
	
	
/* 		Reading all the bam files (ill make this in parallel LATER )	 */	
	for(i=0;i<_nSamples;i++){  
		cout << " Reading file: "<< _fileNames[i] << "\n";
		
			/* 	checks if the file extension is bam, if its not the program stops and outputs an error. */
		if((_fileNames[i].find(".bam")==string::npos) ){
			cerr<< "Unsupported file format, please use bam file(s) as an input. "<<_fileNames[i]<<" \n";
			exit(1);
		}
		
		/* 	tries to open a bam file, if it fails the program exits with an error. */
		if ((inFile = samopen(_fileNames[i].c_str(), "br", NULL)) == 0 | inFile->header == 0) 
		{
			cerr << "file "<< _fileNames[i] << " could't be open.\n";
			exit(1);
		}
		
		/* 	Now that we know the number of transcripts, initiate the clusterTree object */
		_clusterTree=new ClusterTree(_nSamples,_transList->size());
		 
		Transcript* tempTrans;
		ReadList* readList=new ReadList();
		
		/* 	Creates a read pointer and loops through all the reads getting the name, 
		creates a read object with the size and id of the transcript aligned to and add them to the readlist. */
		bam1_t *bamRead = bam_init1();
		 while (( samread(inFile, bamRead)) >= 0) { 
			if(bamRead->core.tid!=-1)
			{
				tempTrans=_transList->find(string(inFile->header->target_name[bamRead->core.tid]));
				if(tempTrans)
					readList->add(bam1_qname(bamRead),bamRead->core.l_qseq,tempTrans->getID());
			}
          }
		  
		/* 	Destroy the read pointer and close the bam file. */
		bam_destroy1(bamRead);
		samclose(inFile);
		
      	cout << "Done!" << "\n";
	  
	  
		/* 		Finished reading the bam file, now we have to create the list of edges and calculate the evidence and counts.	 */
		cout<< "Creating Edge list...\n";	
		float counts;
		int j,k;
		Edge* edgtemp;
		
		/* 	For each read in the list */
	   	for(vector<Read*>:: iterator itr=readList->begin();itr!=readList->end();itr++)
	   	{
			/* 	calculate the counts (divide the read between all the alignments) */
			counts=1.0/(*itr)->alSize();
			set<int>::iterator alitr=(*itr)->begin();
			
			/* 	Loops through all the alignments of the read  */
			for(j=0;j<(*itr)->alSize();j++)
			{
				
				/* 	Gets the transcript pointer of the alignment	 */
				tempTrans=_transList->getByID(*alitr);
				if(tempTrans)
				{
					/* 	Adds the counts, and the evidence (size of the read between all the alignments) */
					tempTrans->addCounts(i,counts);
					tempTrans->addEvidence((*itr)->size());
					tempTrans->addReads(1);
					set<int>::iterator tempItr=next(alitr,1);
					for(k=j+1;k<(*itr)->alSize();k++)
					{
						edgtemp=_edgeList->add(tempTrans->getID(),*tempItr,1);
						advance(tempItr,1);
					}	
				}
				advance(alitr,1);
			}
	   	 }
		 cout<<" "<<readList->size()<<" reads parsed.\n"<<_edgeList->size()<<" Edges, Edge List updated.\nRead List deleted. \n";
		 delete readList;
    } //End of the cycle.		
}


void Clusterizer::loadComFiles()
{
	cout<<"Loading data from preprocessed files...\n";
	int i,val1,val2,val3;
	string fileLine;
	
	/* checks if the file names were set  */
	if(!_fileNames)
	{
		cerr << "ERROR: No file names were provided.... \n";
		exit(1);
	}
	
	/* 		Tries to open the transcripts file, if it fails the program exits with an error. 	 */
	cout << " Reading header of file "<<(_fileNames[0]+".comtr")<<"\n";
	ifstream inTrans;
	inTrans.open((_fileNames[0]+".comtr"));
	if (!inTrans.is_open()) 
	{
		cerr << "file "<< (_fileNames[0]+".comtr") << " could't be open.\n";
		exit(1);
	}

	inTrans>>val1;
	inTrans>>val2;
	cout<<" Loading "<<val1<<" transcripts"<<" with "<<val2<<" samples..."<<endl;

	_nSamples=val2;
	_transList=new TranscriptList(val1);
	/* 	Now that we know the number of transcripts, initiate the clusterTree object */
	_clusterTree=new ClusterTree(val2,val1);

	Transcript * tempTrans;
	int counter=0;
	while(inTrans.good())
	{
		tempTrans=new Transcript(val2);
		inTrans>>*tempTrans;
		_transList->add(tempTrans);

		counter++;
		if(counter==val1)
			 break;
		
	}
	cout<<" done!\n";
	inTrans.close();
	
	/* 		Tries to open the edge file, if it fails the program exits with an error. 	 */
	cout << " Reading header of file "<<(_fileNames[0]+".comed")<<"\n";
	ifstream inEdge;
	inEdge.open((_fileNames[0]+".comed"));
	if (!inEdge.is_open()) 
	{
		cerr << "file "<< (_fileNames[0]+".comed") << " could't be open.\n";
		exit(1);
	}
	inEdge>>val1;
	inEdge>>val2;
	inEdge>>val3;

	_edgeList=new EdgeList(val1);
	Edge * tempEdge;
	counter=0;
	while(inEdge.good())
	{
		tempEdge=new Edge();
		inEdge>>*tempEdge;
		_edgeList->add(tempEdge);
		counter++;
		if(counter==val1)
		{
			break;
		}	
	}
	cout<<" done!\n";
	inEdge.close();
}

  
/**
*							removeLowEvidenceTranscript
*	This part is after loading all the data, removes transcripts with low evidence.	
*	This part is after loading all the data, removes transcripts with low evidence.	
*	of the reads that aligned to them is less than a number of times the size of the 
*	transcript (default of 2 times the size of the transcript ).
*	NOTE:create more functions with different ways to decide which transcript will be removed. 
*
* @param float sizeCoef : this is the coefficient 
* @return void
*/
void Clusterizer::removeLowEvidenceTranscript(float sizeCoef)
{
	cout<<"Removing transcripts with low evidence, using coefficient="<<sizeCoef<<"\n";
	int contador=0,singleton=0,zeroes=0,contador2;
	for(auto transItr=_transList->begin();transItr!=_transList->end();transItr++)
	{
		Transcript* tempTrans=(*transItr);
		if(tempTrans->getEvidence()==0)
		{
			tempTrans->setStatus(2);
			zeroes++;	
		}else
		{
			if(tempTrans->getEvidence()<(tempTrans->getSize()*sizeCoef))
			{
				tempTrans->setStatus(1);
			 	if(!(_edgeList->deleteNode(tempTrans->getID())))
 					singleton++;
				contador++;
			}
		
		}
	}
	cout<<"Deleted "<<(contador+zeroes)<<" nodes: "<<singleton<<" singletons, " <<contador<<" connected and "<<zeroes<<" transcripts with 0 reads \n";	
}

void Clusterizer::createPreclusters()
{	
	/* 	This variable keeps the status of all the nodes, 0 : undiscovered, 1 : in a cluster */
	short * revisitedTrans=new short[_transList->size()]();
	int contador=0,transcount=0;
	Cluster * clusterRoot=new Cluster(0,_nSamples);
	Transcript* tempTrans;
	for(auto transItr=_transList->begin();transItr!=_transList->end();transItr++)
	{
		if(revisitedTrans[(*transItr)->getID()]!=0)
			continue;
		tempTrans=*transItr;

		/* 	Search for all the nodes connected to the actual node.  */
		vector<int>* tempConnected=_edgeList->getAllNodesConnected(tempTrans->getID());
		/* 	if the node is not connected, add it as transcript to the root  */
		if(tempConnected->size()==0)
		{
			clusterRoot->addTranscript(tempTrans);
			revisitedTrans[tempTrans->getID()]=1;
			continue;
		}
		
		/* Creates a new cluster and adds the actual transcript/node */
		Cluster* tempCluster=new Cluster(_nSamples);
		deque<int>* transDeque=new deque<int>;
		transDeque->insert(transDeque->end(),tempConnected->begin(),tempConnected->end());
		delete tempConnected;
		tempCluster->addTranscript(tempTrans);

		/* If the node has connection add them to the cluster too. */
		int transcriptID;
		while(transDeque->size()>0)
		{
			transcriptID=transDeque->front();
			transDeque->pop_front();
			if(revisitedTrans[transcriptID]!=0)
			{
				continue;
			}
			revisitedTrans[transcriptID]=1;
			if(tempCluster->addTranscript(_transList->getByID(transcriptID)))
			{
				tempConnected=_edgeList->getAllNodesConnected(transcriptID);
				transDeque->insert(transDeque->end(),tempConnected->begin(),tempConnected->end());
				delete tempConnected;
			}	
		}
		contador++;
		transcount+=tempCluster->getNumberTranscripts();
		_clusterTree->addCluster(clusterRoot,tempCluster);
	}
	cout<<contador<<" pre-clusters and "<<clusterRoot->getNumberTranscripts() <<" singleton, a total of "<<transcount<<" nodes used in the preclusters\n";
	_clusterTree->setRoot(clusterRoot);

	
}

void Clusterizer::saveToFile()
{
	cout<<"Exporting Parsed files to "<<_prefix<<"\n";
	cout<<"writing Transcript file "<<(_prefix+".comtr")<<"...";
	ofstream outTrans;
	outTrans.open((_prefix+".comtr"),ios_base::trunc);
	outTrans<<_transList->size()<<"\t"<<_nSamples<<"\n";
	for(auto transItr=_transList->begin(); transItr!=_transList->end(); transItr++)
		outTrans << *(*transItr) << "\n";
	outTrans.close();
	cout<<"Done!\n";
		
	cout<<"writing Edge file "<<(_prefix+".comed")<<"...";
	ofstream outEdge;
	outEdge.open((_prefix+".comed"),ios_base::trunc);	
	outEdge<<_edgeList->size()<<"\t"<<_edgeList->inSize()<<"\t"<<_edgeList->outSize()<<"\n";
	for(auto edgeItr=_edgeList->begin(); edgeItr!=_edgeList->end(); edgeItr++)
		outEdge<<*(*edgeItr) << "\n";
	outEdge.close();
	cout<<"Done!\n";
}

 void Clusterizer::printTranscriptsInCluster(Cluster* clusterId)
 {
	 ofstream ofile;
	 ofile.open((_prefix+"_bigCluster.txt"),ios_base::trunc);
	 ofile<<" cluster "<<clusterId->getID()<<" with: "<<clusterId->getNumberTranscripts()<<endl;
	 for(auto itr=clusterId->transBegin(); itr!=clusterId->transEnd(); itr++)
	 {
		 Transcript* tr=_transList->getByID(*itr);
		 ofile<<tr->getName()<<endl;
	 }
	 ofile.close();
 }
 
 void Clusterizer::processPreclusters(float distanceTreshold)
 {
	 omp_set_num_threads(_nThreads);
	 int nPreclusters=_clusterTree->numberPreclusters();
	 vector<Cluster*>::iterator itt=_clusterTree->subClustBegin();
	 char c='d';
	 #pragma omp parallel for
	 for(int i=0;i<nPreclusters;i++)
	 {
		 time_t startTime;
		 time(&startTime);
		 vector<Cluster*>::iterator itr=itt+i;
		 string sout(">>>>>>>>>>>>precluster "+to_string(i)+" with "+to_string((*itr)->getNumberTranscripts())+" transcripts...<<<<<<<<<<<<<<\n");
		 cout<<sout;
		 int nTranscript=(*itr)->getNumberTranscripts();
		 int tCounter=1;
		 EdgeHeap* localEdgeList=new EdgeHeap();
		 set<int>::iterator tranItr=(*itr)->transBegin();
		 try{
			for(int j=0;j<nTranscript;j++,tranItr++)
			{
					vector<Edge*> tempEdgeList=_edgeList->getEdgesByNode(*tranItr);
					for(auto tempEdgeItr=tempEdgeList.begin(); tempEdgeItr!=tempEdgeList.end(); tempEdgeItr++)
					{
						auto tNode1=_transList->getByID((*tempEdgeItr)->node1());
						auto tNode2=_transList->getByID((*tempEdgeItr)->node2());
						if(tNode1 && tNode2)
						{
						
							(*tempEdgeItr)->setDistance( float((*tempEdgeItr)->weight())/float(min(tNode1->getNumberReads(),tNode2->getNumberReads())) );
							localEdgeList->add(*tempEdgeItr,0);
						}
					
					}		
			}
		}catch(std::exception &e)
		{
			cerr<<"<"<<to_string(i)<<">Error building edge heap for precluster "<<to_string(i)<<endl;
			cerr<<e.what()<<endl;
			exit(1);
		}
		time_t endTime;
		time(&endTime);
		double diff_time=difftime(endTime,startTime);
 	   int hours =(int(diff_time / 3600)) % 24;
 	   int minutes = (int(diff_time / 60)) % 60;
 	   int seconds = ((int) diff_time) % 60;
	   string sout2("precluster "+to_string(i)+" edge List with "+to_string(localEdgeList->size())+" edges created in  "+to_string(hours)+" hours "+to_string(minutes)+" minutes "+to_string(seconds)+" seconds.\n");
	   cout<<sout2;
		
		tranItr=(*itr)->transEnd();
		tranItr--;
		int transcriptLimit=*tranItr;
		int lastTrans=(*tranItr)+1;
		vector<Cluster*> *tempClusterList=new vector<Cluster*>();
		Edge* tempEdge=localEdgeList->getFirstEdge();
		int node1=tempEdge->node1();
		int node2=tempEdge->node2();
		int rNode1,rNode2;
		Cluster* tempCluster;	

		while(tempEdge->distance()>=distanceTreshold)
		{
			if(tCounter%1000==0)
			{
				string ssout("\t Still working on precluster "+to_string(i)+" "+to_string(tCounter)+" out of "+to_string((*itr)->getNumberTranscripts())+" transcripts processed, edgelist: "+to_string(localEdgeList->size())+".\n");
				cout << ssout;	
			}	
							
			if(node1<=transcriptLimit && node2<=transcriptLimit)
			{
				(_transList->getByID(node1))->setStatus(3);
				(_transList->getByID(node2))->setStatus(3);
				tCounter+=2;
			try {
					tempCluster=new Cluster(lastTrans,_nSamples);
					tempCluster->addTranscript(_transList->getByID(node2));
					tempCluster->addTranscript(_transList->getByID(node1));
					tempClusterList->push_back(tempCluster);
					auto edgesTemp=localEdgeList->mergeEdges(node1,node2,lastTrans,0);
					for(auto tempEdgeItr=edgesTemp->begin(); tempEdgeItr!=edgesTemp->end(); tempEdgeItr++)
					{
						if((*tempEdgeItr)->node1()<=transcriptLimit)
						{
							rNode1=(_transList->getByID((*tempEdgeItr)->node1()))->getNumberReads();
						}	
						else 
						{
							rNode1=(*tempEdgeItr)->node1();
							rNode1=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode1] ( Cluster* p){return p->getID()==rNode1;})))->getNumberReads();
						}		
						if((*tempEdgeItr)->node2()<=transcriptLimit)
						{
							rNode2=(_transList->getByID((*tempEdgeItr)->node2()))->getNumberReads();
						}	
						else
						{
							rNode2=(*tempEdgeItr)->node2();
							rNode2=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode2] ( Cluster* p){return p->getID()==rNode2;})))->getNumberReads();	
						}	
						(*tempEdgeItr)->setDistance( float((*tempEdgeItr)->weight())/float(min(rNode1,rNode2)));
						localEdgeList->add(*tempEdgeItr,1);
					}
				}
				catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error merging node1: "<<node1<<" and node2: "<<node2<<" lt:"<<lastTrans<<" 0"<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}		
				lastTrans++;
				if(localEdgeList->size()<=0)break;
				try{
					tempEdge=localEdgeList->getFirstEdge();
					node1=tempEdge->node1();
					node2=tempEdge->node2();
				}
				catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error moving to next edge "<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}
				continue;
			}else if(node1>transcriptLimit && node2>transcriptLimit)
			{
				
				try {
					auto edgesTemp=localEdgeList->mergeEdges(node2,node1,node2,1);
				
					auto tempClusItr=find_if(tempClusterList->begin(),tempClusterList->end(),[node1] ( Cluster* p){return p->getID()==node1;});
					tempCluster=*(find_if(tempClusterList->begin(),tempClusterList->end(),[node2] ( Cluster* p){return p->getID()==node2;}));
					tempCluster->mergeClusters(*tempClusItr);
					delete (*tempClusItr);
					*tempClusItr=tempClusterList->back();
					tempClusterList->pop_back();
					for(auto tempEdgeItr=edgesTemp->begin(); tempEdgeItr!=edgesTemp->end(); tempEdgeItr++)
					{
						if((*tempEdgeItr)->node1()<=transcriptLimit)
							rNode1=(_transList->getByID((*tempEdgeItr)->node1()))->getNumberReads();
						else 
						{
							rNode1=(*tempEdgeItr)->node1();
							rNode1=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode1] ( Cluster* p){return p->getID()==rNode1;})))->getNumberReads();
						}		
						if((*tempEdgeItr)->node2()<=transcriptLimit)
							rNode2=(_transList->getByID((*tempEdgeItr)->node2()))->getNumberReads();
						else
						{
							rNode2=(*tempEdgeItr)->node2();
							rNode2=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode2] ( Cluster* p){return p->getID()==rNode2;})))->getNumberReads();	
						}

						(*tempEdgeItr)->setDistance( float((*tempEdgeItr)->weight())/float(min(rNode1,rNode2)));
						localEdgeList->add(*tempEdgeItr,1);
				
					}
				}catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error merging edges from cluster: "<<node2<<" and cluster: "<<node1<<" stored in cluster:"<<node2<<" 1"<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}
				if(localEdgeList->size()<=0)break;
				try{
					tempEdge=localEdgeList->getFirstEdge();
					node1=tempEdge->node1();
					node2=tempEdge->node2();
				}
				catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error moving to next edge "<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}
				continue;
			}
			else if(node1>transcriptLimit)
			{
				(_transList->getByID(node2))->setStatus(3);
				try {
					tempCluster=*(find_if(tempClusterList->begin(),tempClusterList->end(),[node1] ( Cluster* p){return p->getID()==node1;}));
					tempCluster->addTranscript(_transList->getByID(node2));
		
					auto edgesTemp=localEdgeList->mergeEdges(node1,node2,node1,1);
					for(auto tempEdgeItr=edgesTemp->begin(); tempEdgeItr!=edgesTemp->end(); tempEdgeItr++)
					{
						if((*tempEdgeItr)->node1()<=transcriptLimit)
							rNode1=(_transList->getByID((*tempEdgeItr)->node1()))->getNumberReads();
						else 
						{
							rNode1=(*tempEdgeItr)->node1();
							rNode1=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode1] ( Cluster* p){return p->getID()==rNode1;})))->getNumberReads();
						}		
						if((*tempEdgeItr)->node2()<=transcriptLimit)
							rNode2=(_transList->getByID((*tempEdgeItr)->node2()))->getNumberReads();
						else
						{
							rNode2=(*tempEdgeItr)->node2();
							rNode2=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode2] ( Cluster* p){return p->getID()==rNode2;})))->getNumberReads();	
						}
						(*tempEdgeItr)->setDistance( float((*tempEdgeItr)->weight())/float(min(rNode1,rNode2)));
						localEdgeList->add(*tempEdgeItr,1);
					}	
				}catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error merging cluster: "<<node1<<" with node2: "<<node2<<" 1"<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}
				tCounter++;
				if(localEdgeList->size()<=0)break;
				try{
					tempEdge=localEdgeList->getFirstEdge();
					node1=tempEdge->node1();
					node2=tempEdge->node2();
				}
				catch(std::exception &e)
				{
					cerr<<"<"<<to_string(i)<<">Error moving to next edge "<<endl;
					cerr<<e.what()<<endl;
					exit(1);
				}
				continue;
			} 
			
			try {
				(_transList->getByID(node1))->setStatus(3);
				tempCluster=*(find_if(tempClusterList->begin(),tempClusterList->end(),[node2] ( Cluster* p){return p->getID()==node2;}));
				tempCluster->addTranscript(_transList->getByID(node1));

				auto edgesTemp=localEdgeList->mergeEdges(node2,node1,node2,1);
				for(auto tempEdgeItr=edgesTemp->begin(); tempEdgeItr!=edgesTemp->end(); tempEdgeItr++)
				{
					if((*tempEdgeItr)->node1()<=transcriptLimit)
						rNode1=(_transList->getByID((*tempEdgeItr)->node1()))->getNumberReads();
					else 
					{
						rNode1=(*tempEdgeItr)->node1();
						rNode1=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode1] ( Cluster* p){return p->getID()==rNode1;})))->getNumberReads();
					}		
					if((*tempEdgeItr)->node2()<=transcriptLimit)
						rNode2=(_transList->getByID((*tempEdgeItr)->node2()))->getNumberReads();
					else
					{
						rNode2=(*tempEdgeItr)->node2();
						rNode2=(*(find_if(tempClusterList->begin(),tempClusterList->end(),[rNode2] ( Cluster* p){return p->getID()==rNode2;})))->getNumberReads();	
					}
					(*tempEdgeItr)->setDistance( float((*tempEdgeItr)->weight())/float(min(rNode1,rNode2)));
					localEdgeList->add(*tempEdgeItr,1);
				}
			}catch(std::exception &e)
			{
				cerr<<"<"<<to_string(i)<<">Error merging node1: "<<node1<<" in cluster: "<<node2<<" 1"<<endl;
				cerr<<e.what()<<endl;
				exit(1);
			}
			tCounter++;
			if(localEdgeList->size()<=0)break;
			try{
				tempEdge=localEdgeList->getFirstEdge();
				node1=tempEdge->node1();
				node2=tempEdge->node2();
			}
			catch(std::exception &e)
			{
				cerr<<"<"<<to_string(i)<<">Error moving to next edge "<<endl;
				cerr<<e.what()<<endl;
				exit(1);
			}
		
		}
			delete localEdgeList;
			for(auto scitr=tempClusterList->begin();scitr!=tempClusterList->end();scitr++)
					_clusterTree->addCluster(*itr,*scitr);
	 }

	 
 }
 
 
 void Clusterizer::savePreclustersToFile()
 {
	 ofstream preclusterFile;
	 preclusterFile.open((_prefix+"_preclusters.txt"),ios_base::trunc);
	 Transcript* tempTranscript;
	 Cluster* tempCluster;
	 deque<pair<string,Cluster*>> printQueue;
	 tempCluster=_clusterTree->getRoot();
	 string tempClusName;
	 int counter=tempCluster->getNumberTranscripts();
	 preclusterFile<<" Precluster_ID\tnContigs\tcontigIDs\n";
	 for(auto itr=tempCluster->subClustersBegin();itr!=tempCluster->subClustersEnd();itr++)
	{
		preclusterFile<<counter + (*itr)->getID()<<"\t"<<(*itr)->getNumberTranscripts()<<"\t";
		for(auto trItr=(*itr)->transBegin();trItr!=(*itr)->transEnd();trItr++)
			preclusterFile<<(_transList->getByID((*trItr)))->getName()<<",";
		preclusterFile.seekp(-1,std::ios::end);
		preclusterFile<<"\n";
	}
	 preclusterFile.close();	
 }
	 
 void Clusterizer::saveResultsToFile()
 {
	 ofstream countfile,clustfile,statusfile;
	 countfile.open((_prefix+"_counts.txt"),ios_base::trunc);
	 clustfile.open((_prefix+"_clusters.txt"),ios_base::trunc);
	 //statusfile.open((_prefix+"_status.txt"),ios_base::trunc);
	 deque<pair<string,Cluster*>> printQueue;
	 printQueue.push_back(make_pair("Cluster",_clusterTree->getRoot()));
	 int counter;
	 Cluster* tempCluster;
	 Transcript* tempTranscript;
	 string tempClusName;
	 countfile<<"Cluster_ID\t"<<_sampleNames<<endl;
	 while(printQueue.size()>0)
	 {
		 tempCluster=(printQueue.front()).second;
		 tempClusName=(printQueue.front()).first;
		 printQueue.pop_front();
		 counter=0;
		 if(tempCluster->getSubClusterSize()>0)
		 {
		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++,counter++)
		 	{
				tempTranscript=_transList->getByID(*tranItr);
			 	if(!tempTranscript)
					clustfile<<">";
				clustfile<<tempTranscript->getName()<<"\t"<<tempClusName<<"."<<counter<<"\t";
				if(tempTranscript->isRepresentative()) 
					clustfile<<"1\t";
				else 
					clustfile<<"0\t";
				switch(tempTranscript->getStatus()){
					case 0: {
						// SC singleton not clustereded( the contig was not included in any cluster).
						clustfile<<"SC\t";
						break;
					}
					case 1: {
						//SL Singleton by low evidence, the contig was disconnected because it has not enough evidence.
						clustfile<<"SL\t";
						break;
					}
					case 2: {
						//ZE Zero Evidence, the contig is included in the transcriptome but there is zero evidence (reads mapped to it)
						clustfile<<"ZE\t";
						break;
					}
					case 3: {
						//NC new cluster, the contig was added to a cluster with at least another contig.
						clustfile<<"NC\t";
					}
					
				}
				clustfile<<tempTranscript->getSize()<<"\t"<<tempTranscript->getNumberReads()<<endl;
			 	countfile<<tempClusName<<"."<<counter<<"\t"<<tempTranscript->getAllCounts();
				countfile.seekp(-1,std::ios::end);
				countfile<<"\n";
		 	 }
		 }else
		 {
 		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++,counter++)
 		 	{
 				tempTranscript=_transList->getByID(*tranItr);
 			 	if(!tempTranscript)
 					clustfile<<">";
				clustfile<<tempTranscript->getName()<<"\t"<<tempClusName<<"\t";
				if(tempTranscript->isRepresentative()) 
					clustfile<<"1\t";
				else 
					clustfile<<"0\t";
				switch(tempTranscript->getStatus()){
					case 0: {
						// SC singleton not clustereded( the contig was not included in any cluster).
						clustfile<<"SC\t";
						break;
					}
					case 1: {
						//SL Singleton by low evidence, the contig was disconnected because it has not enough evidence.
						clustfile<<"SL\t";
						break;
					}
					case 2: {
						//ZE Zero Evidence, the contig is included in the transcriptome but there is zero evidence (reads mapped to it)
						clustfile<<"ZE\t";
						break;
					}
					case 3: {
						//NC new cluster, the contig was added to a cluster with at least another contig.
						clustfile<<"NC\t";
					}
					
				}
				clustfile<<tempTranscript->getSize()<<"\t"<<tempTranscript->getNumberReads()<<endl;
 		 	 }
			 countfile<<tempClusName<<"\t"<<tempCluster->getAllCounts();
				countfile.seekp(-1,std::ios::end);
				countfile<<"\n";
		 }
		 for(auto subcItr=tempCluster->subClustersBegin(); subcItr!=tempCluster->subClustersEnd(); subcItr++,counter++)
			printQueue.push_back(make_pair((tempClusName+"."+to_string(counter)),*subcItr));	 
	 }
	 countfile.close();
	 clustfile.close();
 }
 
 void Clusterizer::checkLowConnections(float distanceTreshold)
 {
	 for(auto itr=_edgeHeap->begin(); itr!=_edgeHeap->end(); itr++)
	 {
		 int inCon1=_edgeHeap->getInConnectionsByID(((itr->second)->value())->node1());
		 int inCon2=_edgeHeap->getInConnectionsByID(((itr->second)->value())->node2());
		 int outCon1=_edgeHeap->getOutConnectionsByID(((itr->second)->value())->node1());
		 int outCon2=_edgeHeap->getOutConnectionsByID(((itr->second)->value())->node2());
		 if((inCon1==0 && outCon1==1)|| (inCon2==1 && outCon2==0))
		 {
			 Transcript* node1=_transList->getByID(((itr->second)->value())->node1());
			 Transcript* node2=_transList->getByID(((itr->second)->value())->node2());
			 float distance=float(((itr->second)->value())->weight())/float(min(node1->getNumberReads(),node2->getNumberReads()));
			 if(distance>distanceTreshold)
			 {
				 _edgeHeap->deleteEdge(node1->getID(),node2->getID());
			 } 
		 }
	 }
	 
	
 }
 
 void Clusterizer::checkRepresentative(int type){
	 deque<pair<string,Cluster*>> printQueue;
	 printQueue.push_back(make_pair("Cluster",_clusterTree->getRoot()));
	 int counter;
	 Cluster* tempCluster;
	 Transcript* tempTranscript;
	 Transcript* repTrans;
	 if(type==0){
		 while(printQueue.size()>0)
		 {
			 tempCluster=(printQueue.front()).second;
			 printQueue.pop_front();
			 counter=0;
			 repTrans=NULL;
			 if(tempCluster->getSubClusterSize()>0)
			 {
	   		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++)
	   		 	{
	   				tempTranscript=_transList->getByID(*tranItr);
	 				tempTranscript->setRepresentative();
	 			}
				 for(auto subcItr=tempCluster->subClustersBegin(); subcItr!=tempCluster->subClustersEnd(); subcItr++)
					printQueue.push_back(make_pair("c",*subcItr));
			 }else{
				 counter=0;
	  		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++)
	  		 	{
	  				tempTranscript=_transList->getByID(*tranItr);
					if(tempTranscript->getSize()>counter)
					{
						counter=tempTranscript->getSize();
						repTrans=tempTranscript;
					}
				}
				if(repTrans)
					repTrans->setRepresentative();
			 }
		 }
	 }else if(type==1)
	 {
		 while(printQueue.size()>0)
		 {
			 tempCluster=(printQueue.front()).second;
			 printQueue.pop_front();
			 counter=0;
			 repTrans=NULL;
			 if(tempCluster->getSubClusterSize()>0)
			 {
	   		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++)
	   		 	{
	   				tempTranscript=_transList->getByID(*tranItr);
	 				tempTranscript->setRepresentative();
	 			}
				 for(auto subcItr=tempCluster->subClustersBegin(); subcItr!=tempCluster->subClustersEnd(); subcItr++)
					printQueue.push_back(make_pair("c",*subcItr));
			 }else{
				 counter=0;
	  		 	for(auto tranItr = tempCluster->transBegin(); tranItr != tempCluster->transEnd(); tranItr++)
	  		 	{
	  				tempTranscript=_transList->getByID(*tranItr);
					if(tempTranscript->getNumberReads()>counter)
					{
						counter=tempTranscript->getNumberReads();
						repTrans=tempTranscript;
					}
				}
				if(repTrans)
					repTrans->setRepresentative();
			 }
		 }
	 }

 }
 
 void Clusterizer::printPrecluster()
 {
	 ofstream out;
	 out.open((_prefix+"_bigCluster.txt"),ios_base::trunc);
 	vector<Cluster*>::iterator itt=_clusterTree->subClustBegin();
	for(auto itr=(*itt)->transBegin(); itr!=(*itt)->transEnd(); itr++)
	{
		cout<<(*itr)<<" in: "<<(_edgeHeap)->getInConnectionsByID((*itr))<<" out: "<<(_edgeHeap)->getOutConnectionsByID((*itr))<<endl;
		out<<(*itr)<<"\n";
		_edgeHeap->printConnectionsByID(out,*itr);
	}
	out.close();
 }
