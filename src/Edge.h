/***********************************************************************
* FILENAME: Edge.h
*
* DESCRIPTION:
*	Contains all the declarations and definitions of the class Edge.
*	An object of this class is used to store the connection between two
*	transcripts and the amount of evidence supporting it (wheight).
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
* START DATE: September 21, 2017
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef EDGE_H
#define EDGE_H
#include <string>
#include <iostream>

using namespace std;

class Edge
{
private:
	int _Node1;
	int _Node2;
	int _Weight;
	float _Distance;

public:
	Edge():_Node1(0),_Node2(0),_Weight(0),_Distance(0){};
	Edge (int or1,int or2, int w): _Weight(w),_Distance(0) { if ( or1 <= or2 ) { _Node1=or1; _Node2=or2; } else { _Node1=or2; _Node2=or1; } };
	Edge(Edge*ptr):_Node1(ptr->node1()),_Node2(ptr->node2()),_Weight(ptr->weight()),_Distance(ptr->distance()){};
	int node1(){ return _Node1; };
	int node2(){ return _Node2; };
	int weight() { return _Weight; };
	float distance() { return _Distance; };
	void setDistance(float val) { _Distance=val; };
	void setWeight(int val) { _Weight=val; };
	void addWeight( int value ) { _Weight+=value; };
	string name(){ return to_string(_Node1) + "," + to_string(_Node2); };
	~Edge (){};	
	friend ostream& operator<<(ostream& out,const Edge& obj)
	{
		out<<obj._Node1<<"\t"<<obj._Node2<<"\t"<<obj._Weight;
		return out;
	};
	friend istream& operator>>(istream& stream, Edge& obj)
	{
		stream>>obj._Node1;
		stream>>obj._Node2;
		stream>>obj._Weight;
		return stream;
	};
	
	inline bool operator>(const Edge & ob2)const{ return ob2._Distance < this->_Distance; }
	inline bool operator<(const Edge & ob2)const{ return ob2._Distance > this->_Distance; }
};
#endif /* end of include guard: EDGE_H */
