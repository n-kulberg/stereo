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

	point_type& p1(){ return at(m_p1()); }
	point_type& p2(){ return at(m_p2()); }

	const point_type& p1() const { return at(m_p1()); }
	const point_type& p2() const { return at(m_p2)(); }

	point_type	radius_vector() const { return p2()-p1(); };
	double length() const{ return l2_norma(radius_vector()); }

	using parent::at;

private:
	constexpr size_t m_p1() const { return 0; }
	constexpr size_t m_p2() const { return 1; }
};

// user-friendly typename aliases
using s3_F64 = Segment3D<double>;
using s3_F32 = Segment3D<float>;
using s3_I32 = Segment3D<int32_t>;
using s3_I16 = Segment3D<int16_t>;

}//namespace kns_test

#endif //__Segment3D_h
