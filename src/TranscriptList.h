/***********************************************************************
* FILENAME: TranscriptList.h
*
* DESCRIPTION:
*		Declarations and definitions for the TranscriptList class
*		an object of this class will contain all the transcripts in 
*		the experiment.
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

#ifndef TRANSCRIPTLIST_H
#define TRANSCRIPTLIST_H
#include "Transcript.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class TranscriptList
{
private:
	vector< Transcript* > Transcripts_;
	std::unordered_map<string,Transcript *> TranscriptsHash_;
	int current_;

public:
	TranscriptList ():current_(0){};
	TranscriptList(int size) : current_(size-1) { Transcripts_.resize(size); };
	~TranscriptList(){
		vector<Transcript*>::iterator itr=Transcripts_.begin();
		for(;itr!=Transcripts_.end();itr++)
			delete *itr;
	};
	int size(){return Transcripts_.size();}
	void add(Transcript *);
	Transcript* find(string name);
	Transcript* getByID( int id ) { return Transcripts_.at(id); };
	string getNameByID(int id) { return (Transcripts_.at(id))->getName();}
	string getTranscripCountsByID(int id) { return (Transcripts_.at(id))->getAllCounts();}
	Transcript* add(string name,int size,int nSamples);
    vector<Transcript *>::iterator begin(){return Transcripts_.begin();};
    vector<Transcript *>::iterator end(){return Transcripts_.end();};
	int sayhi();
	friend ostream& operator<<(ostream& out, TranscriptList& obj)
	{
		for(int it=0;it<obj.size();it++)
			out<<*(obj.Transcripts_[it])<<endl;
		return out;
	}
};


#endif /* end of include guard: TRANSCRIPTLIST_H */
