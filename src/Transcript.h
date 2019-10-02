/***********************************************************************
* FILENAME: Transcript.h
*
* DESCRIPTION:
*	Contains the declaration and definitions of the class Transcript.
*	An object of this class can store all the data of a transcript. 
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
* START DATE: september 14, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef TRANSCRIPT_H
#define TRANSCRIPT_H

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace	std;

class Transcript
{
private:
	int _Id;
	string _Name;
	float * _Counts;
	int _Size;
	float _Evidence;
	int _nReads;
	short _nSamples;
	short _status;
	bool _representative;

public:
	Transcript ( int id,string name, int nSamp ) : _Id(id), _Name(name), _nSamples(nSamp),_Size(0),_Evidence(0), _nReads(0),_status(0),_representative(false) { _Counts=new float[nSamp](); };
	Transcript ( int id, string name, int size, int nSamp ); 
	Transcript ( int nSamp ) : _nSamples(nSamp),_status(0),_representative(false){ _Counts=new float[nSamp]; };
	~Transcript () { delete [] _Counts; };
	int getID() { return _Id;};
	int getNumberReads() { return _nReads; };
	string getName() { return _Name;};
	int getSize() { return _Size; };
	short getStatus(){ return _status;};
	float getEvidence() { return _Evidence; };
	float getCounts( int s ) { return (s<_nSamples) ? _Counts[s]: -1;  };
	string getAllCounts() { 
		string s;
		for(int i=0;i<_nSamples;i++)
			s=s+to_string(_Counts[i])+"\t";
		return s;
	};
	float addCounts( int s, float value ) { if (s<_nSamples) _Counts[s]+=value; };
	void setName( string value ) { _Name=value; };
	void setSize( int value ) { _Size=value; };
	void setEvidence( float value ) { _Evidence=value; };
	void addReads( int value ){ _nReads+=value; };
	void addEvidence( float value ) { _Evidence+=value; };
	void setCounts( int s, float value ) { _Counts[s]=value; };
	void setStatus(short value){_status=value;};
	void setRepresentative(){_representative=true;}
	bool isRepresentative(){return _representative;}
	friend ostream& operator<< (std::ostream& stream, const Transcript& obj){
		stream<< obj._Id <<"\t"<< obj._Name <<"\t"<< obj._Size <<"\t"<<obj._nReads<<"\t"<< obj._Evidence<<"\t";
		for(int i = 0; i < obj._nSamples; ++i)
		{
			stream <<  obj._Counts[i] << "\t";
		}
		return stream;
	}
	friend istream& operator>>(istream& stream, Transcript& obj)
	{
		stream>>obj._Id;
		stream>>obj._Name;
		stream>>obj._Size;
		stream>>obj._nReads;
		stream>>obj._Evidence;
		for(int i=0;i<obj._nSamples;i++)
			stream>>obj._Counts[i];
		return stream;
	};
	
};




#endif /* end of include guard: TRANSCRIPT_H */
