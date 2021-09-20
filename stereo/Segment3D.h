#ifndef __Segment3D_h
#define __Segment3D_h

//------------------------------------------------------------------
//
//	created:	2021/09/17	18:08
//	filename: 	Segment3D.h
//	file path:	Q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

#include "Point3D.h"

namespace kns_test
{


template<class T>
class Segment3D : public FieldObject<Point3D<T>, Segment3D<T>, 2>
{
public:
	using self = Segment3D<T>;
	using parent = FieldObject<Point3D<T>, Segment3D<T>, 2>;
	using value_type = Point3D<T>;
	using point_type = Point3D<T>;

	using parent::parent;

	template<class T2>
	self& operator=(const Segment3D<T2>& other){ parent::operator=(other); return *this; }
	self& operator=(const self& other){ parent::operator=(other); return *this; }

	point_type& p1(){ return at(m_p1); }
	point_type& p2(){ return at(m_p2); }

	const point_type& p1() const { return at(m_p1); }
	const point_type& p2() const { return at(m_p2); }

	point_type	radius_vector() const { return p2()-p1(); };
	double length() const{ return l2_norma(radius_vector()); }

	using parent::at;

private:
	const size_t m_p1 = 0;
	const size_t m_p2 = 1;
};


}//namespace kns_test

#endif //__Segment3D_h
