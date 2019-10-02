/***********************************************************************
* FILENAME: Transcript.cpp
*
* DESCRIPTION:
*		Contains the method implementation of the Transcript class.
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

#include "Transcript.h"


Transcript::Transcript ( int id, string name, int size, int nSamp ) {
	_Id=id;
	_Name=name;
	_Size=size;
	_nSamples=nSamp;
	_nReads=0;
	_Evidence=0;
	_Counts=new float[nSamp]; 
	_status=0;
	_representative=false;
	for(int i=0;i<nSamp;i++)
		_Counts[i]=0;
}
