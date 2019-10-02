/***********************************************************************
* FILENAME: ReadList.cpp
*
* DESCRIPTION:
*	Contains the method implementation of the class ReadList.
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

#include "ReadList.h"

using namespace std;

Read* ReadList::add(string name,int size,int alignment)
{
	auto search=rUmap_.find(name);
	if(search != rUmap_.end())
	{
		if(!(search->second)->hasAlignment(alignment))
		{
			(search->second)->addAlignment(alignment);
			return search->second;
		}

	}else{
		Read* temp=new Read(size,alignment);
		rList_.push_back(temp);
		rUmap_.insert({name,temp});
		return temp;
	}
}

Read* ReadList::find(string name){
	auto search=rUmap_.find(name);
	if(search != rUmap_.end())
		return search->second;
	return nullptr;
}
