/***********************************************************************
* FILENAME: ReadList.h
*
* DESCRIPTION:
*	Contains all the declarations and definitions of the class ReadList.
*	An object of this class can store all the reads in one bam file (sample),
*	uses an internal hash using the read name as key in order to detect multiple
*	alignments of the same read, and it helps to create the edge's list.
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

#ifndef READLIST_H
#define READLIST_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Read.h"

using namespace std;

class ReadList
{
private:
	vector<Read*> rList_;
	std::unordered_map<string,Read*> rUmap_;

public:
	ReadList (){};
	~ReadList (){
		vector<Read*>::iterator itr=rList_.begin();
		for(;itr!=rList_.end();itr++)
			delete *itr;
	};
	Read* add(string,int,int);
	Read* find(string);
	int size() { return rList_.size(); };
	vector< Read * >::iterator begin() { return rList_.begin(); };
	vector< Read * >::iterator end() { return rList_.end(); };
};

#endif /* end of include guard: READLIST_H */
