/***********************************************************************
* FILENAME: compactA.cpp
*
* DESCRIPTION:
*	Main function of the compactA program.
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
* START DATE: September 14, 2017
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
#include <unistd.h>
#include <set>
#include <vector>
#include <string>
#include <ctime>
#include "EdgeHeap.h"
#include "Clusterizer.h"

using namespace std;

void printUsage()
{
	cout<<"\n\n";
	cout<<"Compacta clusters contigs and counts reads from de novo assembled transcriptomes using read alignments.\n\n";
	cout<<"Usage: \n\tcompacta [options] [input options] <input files> -o <output prefix>\n\n";
	cout<<"Input Options:\n";
	cout<<"\t-b <string list>  A coma separated list of multi-mapped bam files, they can be single,\n";
	cout<<"\t                  paired end or mixed and do not need to be sorted.\n\n";
	cout<<"\t-c <string>       Input the path to preprocessed compacta files, this must contain the\n";
	cout<<"\t                  path and prefix of the comed and comtr files, example: for the files\n"; 
	cout<<"\t                  /home/user/Documents/test.comed, /home/user/Documents/test.comed\n";
	cout<<"\t                  just input: compacta [options] -c /home/user/Documents/test \n\n";
	cout<<"\t-e <string list>  Coma separated list of of full paths to equivalence class files,\n";
    cout<<"\t                  as output by free alignment mappers like Salmon and Kalisto, example:\n";
    cout<<"\t                  compacta -e /home/User/Documents/sample1/aux_info/eq_classes.txt, \n";
	cout<<"\t                  /home/User/Documents/sample2/aux_info/eq_classes.txt\n\n";
	cout<<"\t-f <string>       (OPTIONAL). Path to a file with a list of transcripts to be used in the\n";
    cout<<"\t                  experiment, this list can be a subgroup of transcripts from the whole \n";
    cout<<"\t                  transcriptome, this option is useful to test small groups of transcripts\n";
	cout<<"\t                  or to merge transcriptomes.\n";
	cout<<"Options:\n";
	cout<<"\t-n <int>          This option sets the number of samples in the experiment. Default: 0.\n\n";
	cout<<"\t-s <string list>  Sample names, this is a comma separarted list of names, each for each\n ";
	cout<<"\t                  sample as it will be printed in the output files.\n";
	cout<<"\t                       e.g. -s sample1,sample2,sampl3.\n\n";
	cout<<"\t-g <int list>     A comma separated list that specifies which sample belongs to which\n";
	cout<<"\t                  experimental group, this list must be the same size and in the same\n";
	cout<<"\t                   order as the bam files in the input, for example: \n";
	cout<<"\t                  -b file1,file2,file3 -g 1,2,3 would define file1 belongs to experimental\n";  
	cout<<"\t                  group #1, file2 belongs to experimental group #2, and file3 belongs to\n"; 
	cout<<"\t                  experimental group #3. If not group list is provided, each sample is \n";
	cout<<"\t                  treated as an independent experimental group.\n\n";
	cout<<"\t-p <true/false>   This option when set to true specifies that only parsing of the bam \n";
	cout<<"\t                  files is needed, .comed and  .comtr files will be created according to\n"; 
	cout<<"\t                  the output prefix supplied. This option is useful if the input bam files\n";
	cout<<"\t                  are very big and multiple clustering test are required.\n\n";
	cout<<"\t                  If set to false or not included compacta will cluster the input data as\n";
	cout<<"\t                  normal. Default: false.\n\n";
	cout<<"\t-d <double>       This value is used as a distance treshold to decide if two clusters are\n";
	cout<<"\t                  close enough to be part of the same cluster, the value must be between \n";
	cout<<"\t                  0 and 1. Default: 0.3 \n\n";
	cout<<"\t-l <double>       This value is used as a coefficient to calculate a bias free way to \n";
	cout<<"\t                  remove transcripts with low read evidence. The value will multiply the size\n";
	cout<<"\t                  of the transcript to create a treshold, if the read size is at least <value>\n";
	cout<<"\t                  times the size of the transcript. Default: 2.\n\n";
	cout<<"\t-t <int>          Number of threads to use, this will make compacta run faster but be careful,\n";
	cout<<"\t                  if your bam files are bigger than your RAM and you run each in a thread it\n";
	cout<<"\t                  might saturate your RAM, in this case use -t 1 -p true to process the bam\n";
	cout<<"\t                  files sequentially and then run compacta on the processed files with -t > 1\n";
	cout<<"\t                  to run the clustering algorithm in parallel. Default 1\n";
	cout<<"output\n";
	cout<<"\t-o                Sets the prefix for the output files, this will be used to create the files \n";
	cout<<"\t                  when parsing the bam files or creating the clusters.\n";
	cout<<"\t                  e.g. -p true -o /home/user/test \n";
	cout<<"\t                  would create the files test.comed and test.comtr in the folder\n\n";
	cout<<"\t                  /home/user/ as the result of preprocesing bam files. Default: ./com_out.\n\n";
	cout<<"\t-r <size/reads>   Sets which value will be used to choose the representative contig of each cluster,\n";
	cout<<"\t                  if size is set the longest contig in the cluster will be choose as its representative\n";
	cout<<"\t                  if reads is set the contig with the biggest number of reads in the cluster will be \n";
	cout<<"\t           	   choose as its representative, Default: size\n";
	cout<<"Citation: \n\n";
	cout<<" Please see https://github.com/fgrazom/clustering/wiki for more information.\n"<<endl;
		
}

void calculateRunningTime(time_t a)
{
	   time_t end_time;
	   time(&end_time);
	   double diff_time=difftime(end_time,a);
	   int weeks =diff_time/604800;
	   int days = (int(diff_time /86400))%7;
	   int hours =(int(diff_time / 3600)) % 24;
	   int minutes = (int(diff_time / 60)) % 60;
	   int seconds = ((int) diff_time) % 60;
	   cout<<" It took compacta "<<weeks<<" weeks "<<days<<" days "<<hours<<" hours "<<minutes<<" minutes "<<seconds<<" seconds to finish."<<endl;
	   
}

int main (int argc, char *argv[])
{
	
	time_t initial_time;
	time(&initial_time);
	float dTreshold=0.3;
	int nSamples=0;
	int nGroups=0;
	int nThreads=1;
	int c;
	int repTranscript=0;
	int typeInput=0;
	float coefficient=2;
	bool parseFiles=false;
	string sampleNames;
	string outputPrefix="";
	string fileNames="";
	string fastaName="";
	vector<int>groups;
	int params=1;
	
    cout<<"\n";
	cout<< "			  ___  __   _  _  ____   __    ___  ____  __  \n";
	cout<< "			 / __)/  \\ ( \\/ )(  _ \\ / _\\  / __)(_  _)/ _\\ \n";
	cout<< "			( (__(  O )/ \\/ \\ ) __//    \\( (__   )( /    \\\n";
	cout<< "			 \\___)\\__/ \\_)(_/(__)  \\_/\\_/ \\___) (__)\\_/\\_/\n";
	
	cout<<" 								version 1.02 "<<endl;
	// VERSION <<endl;
	
    while((c =  getopt(argc, argv, "s:n:g:d:b:c:f:o:p:l:t:e:r:")) != EOF){
		switch(c){
			case 's':{
				sampleNames=string(optarg);
				replace(sampleNames.begin(),sampleNames.end(),',','\t');
				cout<<" Names of the samples: "<<optarg<<endl;
				params+=2;
				break;
			}
			case 'n':{
				cout<<" Setting number of samples: "<<optarg<<"\n";
				nSamples=atoi(optarg);
				params+=2;
				break;
			}
			case 'g':{
				cout<<" Sample groups: "<<optarg<<"\n";
				stringstream inputString(optarg);
				string tempString;
				set<int>groupsSet;
				while(getline(inputString,tempString,','))
				{
					int val=atoi(tempString.c_str());
					//cout<<" grupo: "<<val<<endl;
					groups.push_back(val);
					groupsSet.insert(val);
				}
				nGroups=groupsSet.size();
				cout<<" Number of groups: "<<nGroups<<"\n";
				params+=2;
				break;
			}

		    case 'd':{
		      cout << " Distance Treshold: "<< optarg << "\n";
		      dTreshold=atof(optarg);
		      params+=2;
		      break;
		    }
			case 'b':{
				cout<<" Type of input: bam files.\n";
				cout<<" Input file(s): "<<optarg<<"\n";
				fileNames=string(optarg);
				typeInput=0;
				params+=2;
				break;
			}
			case 'c':{
				cout<<" Type of input: parsed files.\n";
				cout<<" Input file (s): "<<optarg<<"\n";
				fileNames=string(optarg);
				typeInput=1;
				params+=2;
				break;
			}
			case 'e':{
				cout<<" Type of input: equivalence classes files.\n";
				cout<<" Input file (s): "<<optarg<<"\n";
				fileNames=string(optarg);
				typeInput=3;
				params+=2;
				break;
			}
			case 'f':{
				cout<<" Inplement subsetting contigs from: "<<optarg<<"\n";
				fastaName=string(optarg);
				params+=2;
				break;
			}
			case 'o':{
				cout<<" Output prefix: "<<optarg<<"\n";
				outputPrefix=string(optarg);
				params+=2;
				break;
			}
			case 'p':{
				cout<<" Parse files only: "<<optarg<<"\n";
				string tempString(optarg);
		        transform(tempString.begin(), tempString.end(), tempString.begin(), ::tolower);
		        if( tempString.compare("true")==0 | tempString.compare("t")==0 | tempString.compare("1")==0 )
		  			parseFiles=true;
				params+2;
				break;
			}
			case 'l':{
				cout<<" coefficient (for transcript with low evidence removal): "<<optarg<<"\n";
				coefficient=atof(optarg);
				params+=2;
				break;
			}
			case 't':{
				nThreads=atoi(optarg);
				if(nThreads<=0)
				{
					cerr<<"\nError!!, Number of threads not valid "<<nThreads<<"...\n";
					printUsage();
					exit(1);
				}
				cout<<" Setting number of threads to: "<<nThreads<<"\n";
				params+=2;
				break;
			}
			case 'r':{
				cout<<" Choose cluster representative contig using: "<<optarg<<"\n";
				if(string(optarg)=="size")
					repTranscript=0;
				else if(string(optarg)=="reads")
					repTranscript=1;
				else
				{
					cerr<<"\n Error!, unknown option.."<<c<<" "<<optarg<<endl;
					printUsage();
					exit(1);
				}
				params+=2;
				break;
			}
		    case '?':{
		      cerr << "\nError!, Unknown option.. stopping" <<c<<" "<<optarg<< endl;
		      printUsage();
		      exit(1);
		      break;
		    }
		}
	}
	
	if(fileNames=="")
	{
		cerr<<"\nError!!, No input files were supplied...\n";
		printUsage();
		exit(1);
	}
	if(nSamples<=0)
	{
		if(groups.size()!=0)
		{
			cout<<" Number of samples not provided, calculating from groups definition...\n";
			nSamples=groups.size();
			//cout<<"Number of samples "<<nSamples<<"\n";
		}else
		{
			cerr<<"\nError!, Number of samples not valid...\n";
			printUsage();
			exit(1);	
		}
		
	}
	if(groups.size()!=0 && groups.size()!=nSamples)
	{
		cerr<<"\nError!, The number of samples defined in groups does not match the number of samples...\n";
		printUsage();
		exit(1);
	}
	cout<<"\n>>Clustering initiated with "<<nSamples<<" samples\n";
	 Clusterizer clusterMaker(nSamples,nThreads);
	 if(sampleNames!="")
		 clusterMaker.setSampleNames(sampleNames);
	if(fastaName!="")
		clusterMaker.setFastaName(fastaName);
	 if(outputPrefix!="")
		 clusterMaker.setPrefix(outputPrefix);
	 clusterMaker.setFileNames(fileNames);
	 if(typeInput==0)
		 clusterMaker.loadBamFiles();
	 else if (typeInput==1)
		 clusterMaker.loadComFiles();
	 else if (typeInput==3)
		 clusterMaker.loadEqClassesFiles();
	 if(parseFiles)
	 {
	 	clusterMaker.saveToFile();
		calculateRunningTime(initial_time);
		exit(0);
	 }
		
	if(typeInput!=3) 
	  clusterMaker.removeLowEvidenceTranscript(coefficient);
	  clusterMaker.createPreclusters();
	  clusterMaker.savePreclustersToFile();
	//   clusterMaker.printPrecluster();
	   clusterMaker.processPreclusters(dTreshold);
	   clusterMaker.checkRepresentative(repTranscript);
	   clusterMaker.saveResultsToFile();
	   calculateRunningTime(initial_time);

	return 0;
}
