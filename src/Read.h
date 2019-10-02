/***********************************************************************
* FILENAME: Read.h
*
* DESCRIPTION:
*	Contains all the definitions and declarations of the class Read
*	An object of this class can store the data of a read and its aligments.
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
* START DATE: September 20, 2017 
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef READ_H
#define READ_H
#include <vector>
#include <string>
#include <iostream>
#include <set>

using namespace std;

class Read
{
private:
	/* data */
	int size_;
	set<int>tList_;
public:
	Read () : size_(0) { };
	Read(int size,int id):size_(size) { tList_.insert(id); };
	int size() { return size_; };
	int alSize() { return tList_.size(); };
	void addAlignment(int id) { tList_.insert(id); };
	bool hasAlignment(int id) { return tList_.find(id)!=tList_.end() ? true:false;  }
	set<int>::iterator begin() { return tList_.begin(); };
	set<int>::iterator end() { return tList_.end(); };
	~Read (){};
	friend ostream& operator<< (ostream& out,const Read& obj)
	{
		out << " S: " << obj.size_ << " al("<<obj.tList_.size()<<"): ";
		for(auto itr: obj.tList_)
			out<< itr << " - ";
		return out<<"\n";
	}
};

#endif /* end of include guard: READ_H */
