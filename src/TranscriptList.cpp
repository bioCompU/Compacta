/***********************************************************************
* FILENAME: TranscriptList.cpp
*
* DESCRIPTION:
*	Contains the method implementation of the class TranscriptList.
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

#include "TranscriptList.h"

using namespace std;
/**
*	
*	Adds a transcript to the list, 
*	Checks if the transcript already exists, if it does return its pointer, if it doesn't exists
*	it creates it, adds it to the list and return a pointer to its object.		
*
* @param 
*		string name: The name of the transcript to add.
*		int nSamples: The number of samples in the experiment.
* @return 
*
*/
Transcript* TranscriptList::add(string name, int size, int nSamples){
	auto search = TranscriptsHash_.find(name);
	
	if(search != TranscriptsHash_.end()) {
	        return search->second;
	}else {
	        Transcript* temp=new Transcript(current_,name,size,nSamples);
			current_++;
			Transcripts_.push_back(temp);
			TranscriptsHash_.insert({name,temp});
			return temp;
	}
	
}

void TranscriptList::add(Transcript *tr)
{
	auto search = TranscriptsHash_.find(tr->getName());
	
	if(search == TranscriptsHash_.end()) {
			//Transcripts_.push_back(tr);
		Transcripts_[tr->getID()]=tr;
		TranscriptsHash_.insert({tr->getName(),tr});
	}
}
		
Transcript* TranscriptList::find(string name){
	auto search = TranscriptsHash_.find(name);
	if(search != TranscriptsHash_.end()) {
	        return search->second;
	}else {
		return nullptr;
	}
}


