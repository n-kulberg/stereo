#ifndef __FieldOutput_h
#define __FieldOutput_h

//------------------------------------------------------------------
//
//	created:	2021/09/17	18:14
//	filename: 	FieldOutput.h
//	file path:	Q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

#include "Point3D.h"
#include "Matrix3D.h"
#include <iostream>

namespace kns_test
{

using namespace std;
// output auxiliaries

template<class T>
struct separator
{
	static string prefix(){ return ""; }
	static string get(){ return ";"; }
};

template<class T>
struct separator<Point3D<T>>
{
	static string prefix(){ return "\n"; }
	static string get(){ return "\n"; }
};

template<class T>
struct separator<Matrix3D<T>>
{
	static string prefix(){ return "\n"; }
	static string get(){ return "\n"; }
};

// output

template<class T, class CHILD_T, size_t N>
std::ostream& operator << (std::ostream& str, const FieldObject<T, CHILD_T, N>& f)
{
	str << separator<T>::prefix() << "{";
	std::for_each(f.cbegin(), f.cend()-1, [&str](const T& x){str << x << separator<T>::get();});
	str << *(f.cend()-1) << "}";
	return str;
}

}//namespace kns_test

#endif //__FieldOutput_h
