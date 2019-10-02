/***********************************************************************
* FILENAME: Heap.h
*
* DESCRIPTION:
*	Contains all the declarations and definitions of the class Heap
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
* START DATE:
*
* CHANGES:
*
*	VERSION     DATE     WHO     DETAIL
*
*
***********************************************************************/

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

template <class T> class HeapContainer
{
private:
	T _value;
	HeapContainer *_parent,*_previous,*_next,*_left,*_right;

public:
	HeapContainer (const T& value,HeapContainer *parent,HeapContainer *previous,HeapContainer *next,HeapContainer *left, HeapContainer *right):_value(value), _parent(parent),_previous(previous),_next(next),_left(left),_right(right){};
	~HeapContainer (){ deallocateValue(_value); };
	T & deallocateValue(T& obj){}
	T* deallocateValue(T* obj){ delete obj; }
	T value(){ return _value; };
	void setValue(T val){ _value = val; };
	HeapContainer* parent(){ return _parent; };
	HeapContainer* previous(){ return _previous; };
	HeapContainer* next(){ return _next; };
	HeapContainer* left(){ return _left; };
	HeapContainer* right(){ return _right; };
	void setParent(HeapContainer* val) { _parent = val; };
	void setPrevious(HeapContainer* val){ _previous = val; };
	void setNext(HeapContainer* val){ _next = val; };
	void setLeft(HeapContainer* val){ _left = val; };
	void setRight(HeapContainer* val){ _right = val; };
	bool isDisconnected(){if(!_parent && !_previous && !_next && !_left && !_right) return true; else return false;};
	T& ptr(T&);
	T& ptr(T*);
	friend ostream& operator<<(ostream& out,const HeapContainer<T>& obj)
	{
		out<<obj._value;
		return out;
	};
};

template<class T>
T & ptr(T & obj) { return obj; } 

template<class T>
T & ptr(T * obj) { return *obj; } 



template<class T> class Heap
{
private:
	HeapContainer<T> * _root, * _beginning, * _end,* _addSpot, *Current;
	int _selectedContainer; 
	int _size;
public:
	Heap ();
	~Heap ();
	void init();
	void clean();
	HeapContainer<T>* add(T);
	int size(){ return _size; };
	void replace(HeapContainer<T>*, HeapContainer<T>*);
	T deleteItem(HeapContainer<T>*);
	void print();
	T pop();
	HeapContainer<T>* createWrapper(T);
	void add(HeapContainer<T>*);
	void reorder(HeapContainer<T>*);
	bool isNodeDisconnected(HeapContainer<T>* value){ if(value->isDisconnected()&&value!=_root)return true;else return false; };
	friend ostream& operator<<(ostream& out,const Heap<T>& obj)
	{	
		HeapContainer<T> *temp;
		temp=obj._beginning;
		out<<">------------- size "<<obj._size<<" --------------<\n";
		if(obj._beginning) out<<"\nBeginning: "<<ptr(obj._beginning->value());
		if(obj._end) out<<"\nEnd: "<<ptr(obj._end->value());
		if(obj._root) out<<"\nRoot: "<<ptr(obj._root->value());
		if(obj._addSpot) out<<"\nAddspot: "<<ptr(obj._addSpot->value());
		if(obj.Current) out<<"\nCurrent: "<<ptr(obj.Current->value());
		out<<"\n\n";
		while(temp){
			if(temp->value())out<<"node: "<<ptr(temp->value());
			if(temp->parent())out<<" parent: "<<ptr((temp->parent())->value());
			if(temp->left()) out <<" left: "<<ptr((temp->left())->value());
			if(temp->right()) out<<" right: "<<ptr((temp->right())->value());
			out<<"\n";
			temp=temp->next();
		}
		return out;
	};		
};

/**
* Constructor
*/
template<class T>
Heap<T>::Heap(){
	init();
}

/**
* Destructor
*/
template<class T>
Heap<T>::~Heap(){
	clean();
}

/**
 * Initiates the heap.
 * 
 * @param void
 * @return void
 * @throws	
 */
template<class T>
void Heap<T>::init(){
	_root=NULL;
	_beginning=NULL;
	_end=NULL;
	_addSpot=NULL;
	_selectedContainer=0;
	_size=0;
}
  
/**
 * Frees the memory used by the heap.
 * 
 * @param void
 * @return void
 * @throws	
 */
template<class T>
void Heap<T>::clean(){
	HeapContainer<T>* temp=_beginning;
	while( temp )
	{
		_beginning=_beginning->next();
		delete temp;
		temp=_beginning;
	}
}

template<class T>
void Heap<T>::add(HeapContainer<T>* value)
{
	if(!_root){
		_beginning=value;
		_root= value;
		_end = value;
		_addSpot = value;
		_size++;
		return;
	}

	value->setParent(_addSpot);
	value->setPrevious(_end);
	_end->setNext(value);
	_end = value;
	if(_selectedContainer==0)
	{
		_addSpot->setLeft(value);
		_selectedContainer=1;
	}else{
		_addSpot->setRight(value);
		_selectedContainer=0;
		_addSpot=_addSpot->next();
	}
	_size++;
	while(value->parent())
	{
		if(ptr((value->parent())->value()) < ptr(value->value()))
				replace(value->parent(),value); 
		
		else 
			break;
	}
}

template<class T> 
HeapContainer<T>* Heap<T>::createWrapper(T value)
{
	HeapContainer<T> *temp;
	temp=new HeapContainer<T>(value,NULL,NULL,NULL,NULL,NULL);
	return temp;
}

/**
 * 
 * Inserts a value at the end of the heap and search upwards until it is in the right place.
 * @param T
 * @return void	
 * @throws	
 */
template<class T>
HeapContainer<T>* Heap<T>::add(T value){

	HeapContainer<T> *temp;
	/*  if the heap is empty */
	if(!_root){
		temp=new HeapContainer<T>(value,NULL,NULL,NULL,NULL,NULL);
		_beginning=temp;
		_root= temp;
		_end = temp;
		_addSpot = temp;
		_size++;
		return temp;
	}
	temp = new HeapContainer<T>(value,_addSpot,_end,NULL,NULL,NULL);
	_end->setNext(temp);
	_end = temp;
	if(_selectedContainer==0)
	{
		_addSpot->setLeft(temp);
		_selectedContainer=1;
	}else{
		_addSpot->setRight(temp);
		_selectedContainer=0;
		_addSpot=_addSpot->next();
	}
	_size++;
	while(temp->parent())
	{
		if(ptr((temp->parent())->value()) < ptr(temp->value()))
				replace(temp->parent(),temp); 
		
		else 
			break;
	} 

	return temp;
}		

/**
 * 
 * Replaces one value in the heap for another, it is used to order a newly inserted value.
 * @param HeapContainer*
 * @return void	
 * @throws	
 */
template<class T>
void Heap<T>::replace(HeapContainer<T> *ori,HeapContainer<T> *dest){
	HeapContainer<T> * parent, * lChild, * rChild, * previous, *next;
	parent = ori->parent();
	lChild = ori->left();
	rChild = ori->right();
	previous = ori->previous();
	next = ori->next();

	if (dest->previous() == ori)
	{
		if(dest->next())
		{
			ori->setNext(dest->next());
			(ori->next())->setPrevious(ori);
		}else{
			ori->setNext(NULL);
			_end=ori;
		}
		ori->setPrevious(dest);
		if(previous)
		{
			dest->setPrevious(previous);
			previous->setNext(dest);
		}else{
			dest->setPrevious(NULL);
			_beginning= dest;
		}
		dest->setNext(ori);
		
	}else{
		if(dest->next())
		{
			ori->setNext(dest->next());
			(ori->next())->setPrevious(ori);
		}else{
			ori->setNext(NULL);
			_end = ori;
		}
		
		if(dest->previous())
		{
			ori->setPrevious(dest->previous());
			(ori->previous())->setNext(ori);
		}else{
			ori->setPrevious(NULL);
			_beginning=ori;
		}
		
		if(previous){
			dest->setPrevious(previous);
			previous->setNext(dest);
		}else{
			dest->setPrevious(NULL);
			_beginning=dest;
		}
		
		if(next){
			dest->setNext(next);
			(dest->next())->setPrevious(dest);
		}else{
			dest->setNext(NULL);
			_end=dest;
		}
	}
	
	if(dest->parent() == ori)
		ori->setParent(dest);
	else
		ori->setParent(dest->parent());
	
	ori->setLeft(dest->left());
	ori->setRight(dest->right());

	if(ori->parent())
	{
		if((ori->parent())->left()==dest)
			(ori->parent())->setLeft(ori);
		else if ((ori->parent())->right()==dest)
			(ori->parent())->setRight(ori);
	}
	
	if(parent){
		dest->setParent(parent);
		if(parent->left() == ori)
			parent->setLeft(dest);
		else if (parent->right() == ori)
			parent->setRight(dest);
	}else{
		dest->setParent(NULL);
		_root=dest;
	}
	
	if(lChild == dest)
		dest->setLeft(ori);
	else
		dest->setLeft(lChild);
	if( rChild == dest)
		dest->setRight(ori);
	else
		dest->setRight(rChild);
	
	if (dest->left())
		(dest->left())->setParent(dest);
	if(dest->right())
		(dest->right())->setParent(dest);

	if(ori->left())
		(ori->left())->setParent(ori);
	if(ori->right())
		(ori->right())->setParent(ori);
	if(_addSpot==ori)
		_addSpot=dest;
	else if (_addSpot==dest)
		_addSpot=ori;
}

template<class T>
void Heap<T>::print(){
	HeapContainer<T> *temp;
	temp=_beginning;
	cout<<"-------------- size "<<size()<<" --------------"<<endl;
	if(_beginning) cout<<"Beginning: "<<ptr(_beginning->value())<<" d:"<<(_beginning->value())->distance()<<endl;
	if(_end) cout<<"End: "<<ptr(_end->value())<<" d:"<<(_end->value())->distance()<<endl;
	if(_root) cout<<"Root: "<<ptr(_root->value())<<" d:"<<(_root->value())->distance()<<endl;
	if(_addSpot) //cout<<"si entra al addspot"<<endl;
		cout<<"Addspot: "<<*(_addSpot->value())<<" d:"<<(_addSpot->value())->distance()<<endl;

	if(_selectedContainer)cout<<"SelectedContainer: "<<_selectedContainer<<endl;
	cout<<"...................................."<<endl;
	while(temp){
		if(temp->value()) cout<<"node: "<<ptr(temp->value())<<"\td:"<<ptr(temp->value()).distance()<<"\t"<<endl;
		if(temp->parent())cout<<" parent: "<<ptr((temp->parent())->value())<<endl;
		if(temp->left()) cout <<" left: "<<ptr((temp->left())->value())<<endl;
		if(temp->right()) cout<<" right: "<<ptr((temp->right())->value())<<endl;
		cout<<endl;
		temp=temp->next();
	}
	cout<<"...................................."<<endl;
}

template<class T>
T Heap<T>::pop(){
	HeapContainer<T> *temp;
	T value;
	if(!_root) return 0;
	if(_beginning==_end)
	{
		temp=_beginning;
		_beginning=NULL;
		_end=NULL;
		_root=NULL;
		_addSpot=NULL;
		_selectedContainer=0;
		value=temp->value();
		delete temp;
		_size--;
		return value;
	} 
	replace(_beginning,_end);
	temp=_end;
	if((_end->parent())->left() == _end) 
		(_end->parent())->setLeft(NULL);
	else
		(_end->parent())->setRight(NULL);
	(_end->previous())->setNext(NULL);
	_end=_end->previous();
	value=temp->value();
	if(_addSpot ==_beginning) 
	{
		_addSpot=_end;
		_selectedContainer=0;
	}	
	else if (_addSpot == _end)
	{
		_addSpot=_beginning;
		_selectedContainer=1;
	}	
	else if(_end->parent()!=_addSpot && (!_addSpot->left()) )
	{
		if(_end->parent())
		{
			if(!(_end->parent())->right())
			{
				_addSpot=_end->parent();
				_selectedContainer=1;	
			}else
				_selectedContainer=0;
		}
		
	}
	delete temp;
	temp=_beginning;
	while( temp->left() || temp->right() )
	{
		if( temp->left())
		{
			if( temp->right())
			{
				if( ptr((temp->left())->value()) > ptr((temp->right())->value()))
				{
					if( ptr((temp->left())->value()) > ptr(temp->value()) )
						replace(temp,temp->left());
					else 
						break;
				}else{
					if(ptr((temp->right())->value()) > ptr(temp->value()))
						replace(temp,temp->right());
					else 
						break;
				}
			}else{
				if( ptr((temp->left())->value()) > ptr(temp->value()))
					replace(temp,temp->left());
				else
					break;
			}
			
		}else{
			if(temp->right())
			{
				if( ptr((temp->right())->value()) > ptr(temp->value()))
					replace(temp,temp->right());
				else 
					break;
			}
		}
	}
	_size--;
	return value;
}

template <class T>
T Heap<T>::deleteItem(HeapContainer<T>* edgeCon)
{
	HeapContainer<T> *temp,*temp2;
	T value;
	if(!_root || !edgeCon) return 0;
	if(_beginning==_end && edgeCon==_end)
	{
		temp=_beginning;
		_beginning=NULL;
		_end=NULL;
		_root=NULL;
		_addSpot=NULL;
		_selectedContainer=0;
		value=temp->value();
		delete temp;
		_size=0;
		return value;
	}	
	if(edgeCon!=_end)
	{
		temp2=_end;
		replace(edgeCon,_end);
		temp=_end;
		if(_end->parent())
		{
			if((_end->parent())->left() == _end) 
				(_end->parent())->setLeft(NULL);
			else
				(_end->parent())->setRight(NULL);		
		}
		(_end->previous())->setNext(NULL);
		_end=_end->previous();
		if(_addSpot ==_beginning) 
		{
			_selectedContainer=0;
		}	
		else if (_addSpot == _end)
		{
			_addSpot=_beginning;
			_selectedContainer=1;
		}	
		else if(_end->parent()!=_addSpot && (!_addSpot->left()) )
		{
			if(_end->parent())
			{
				if(!(_end->parent())->right())
				{
					_addSpot=_end->parent();
					_selectedContainer=1;	
				}else
					_selectedContainer=0;
			}
		
		}
		delete temp;
		temp=temp2;
	 
		while(temp->parent())
		{
			if(ptr((temp->parent())->value()) < ptr(temp->value()))
					replace(temp->parent(),temp); 
		
			else 
				break;
		}
		while( temp->left() || temp->right() )
		{
			if( temp->left())
			{
				if( temp->right())
				{
					if( ptr((temp->left())->value()) > ptr((temp->right())->value()))
					{
						if( ptr((temp->left())->value()) > ptr(temp->value()) )
						{
							replace(temp,temp->left());	
						}
						else 
							break;
					}else{
						if(ptr((temp->right())->value()) > ptr(temp->value()))
						{
							replace(temp,temp->right());
						}	
						else 
							break;
					}
				}else{
					if( ptr((temp->left())->value()) > ptr(temp->value()))
					{
						replace(temp,temp->left());					
					}

					else
						break;
				}
			
			}else{
				if(temp->right())
				{
					if( ptr((temp->right())->value()) > ptr(temp->value()))
					{
						replace(temp,temp->right());					
					}
					else 
						break;
				}
			}
		}
	}else{
		temp=_end;
		if(_end->parent())
		{
			if((_end->parent())->left() == _end) 
				(_end->parent())->setLeft(NULL);
			else
				(_end->parent())->setRight(NULL);		
		}
		(_end->previous())->setNext(NULL);
		_end=_end->previous();
		if(_addSpot ==_beginning) 
		{
			_selectedContainer=0;
		}	
		else if (_addSpot == _end)
		{
			_addSpot=_beginning;
			_selectedContainer=1;
		}	
		else if(_end->parent()!=_addSpot && (!_addSpot->left()) )
		{
			if(_end->parent())
			{
				if(!(_end->parent())->right())
				{
					_addSpot=_end->parent();
					_selectedContainer=1;	
				}else
					_selectedContainer=0;
			}
		
		}
		
	}

	_size--;
	return value;
	
}

template<class T>
void Heap<T>::reorder(HeapContainer<T> *ori)
{
	HeapContainer<T> *temp=ori;
	while(temp->parent())
	{
		if(ptr((temp->parent())->value()) < ptr(temp->value()))
				replace(temp->parent(),temp); 
		
		else 
			break;
	}
	while( temp->left() || temp->right() )
	{
		if( temp->left())
		{
			if( temp->right())
			{
				if( ptr((temp->left())->value()) > ptr((temp->right())->value()))
				{
					if( ptr((temp->left())->value()) > ptr(temp->value()) )
					{
						replace(temp,temp->left());	
					}
					else 
						break;
				}else{
					if(ptr((temp->right())->value()) > ptr(temp->value()))
					{
						replace(temp,temp->right());
					}	
					else 
						break;
				}
			}else{
				if( ptr((temp->left())->value()) > ptr(temp->value()))
				{
					replace(temp,temp->left());					
				}

				else
					break;
			}
			
		}else{
			if(temp->right())
			{
				if( ptr((temp->right())->value()) > ptr(temp->value()))
				{
					replace(temp,temp->right());					
				}
				else 
					break;
			}
		}
	}
}

#endif /* end of include guard: HEAP_H */



	