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
	using parent::operator =;

	point_type& p1(){ return at(m_p1()); }
	point_type& p2(){ return at(m_p2()); }

	const point_type& p1() const { return at(m_p1()); }
	const point_type& p2() const { return at(m_p2)(); }

	point_type	radius_vector() const { return p2()-p1(); };
	double length() const{ return l2_norma(radius_vector()); }

	//parallel translation by vector
	//NB no references in these functions, high risk of side effects
	template<class T2> self& operator += (Point3D<T2> translation_vector){ parent::modify_binary_action(*this, translation_vector, [](Point3D<T>& x, Point3D<T2> y){x += y;}); return *this; }
	template<class T2> self& operator -= (Point3D<T2> translation_vector){ parent::modify_binary_action(*this, translation_vector, [](Point3D<T>& x, Point3D<T2> y){x -= y;}); return *this; }
	template<class T2> self operator + (Point3D<T2> translation_vector) const { return parent::return_binary_action(*this, translation_vector, [](const Point3D<T>& x, const Point3D<T2>& y){return x + y;}); }
	template<class T2> self operator - (Point3D<T2> translation_vector) const { return parent::return_binary_action(*this, translation_vector, [](const Point3D<T>& x, const Point3D<T2>& y){return x - y;}); }


	//inherited operations
	using parent::at;
	using parent::operator +=;
	using parent::operator -=;
	using parent::operator +;
	using parent::operator -;


private:
	static constexpr size_t m_p1() { return 0; }
	static constexpr size_t m_p2() { return 1; }
};

// user-friendly typename aliases
using s3_F64 = Segment3D<double>;
using s3_F32 = Segment3D<float>;
using s3_I32 = Segment3D<int32_t>;
using s3_I16 = Segment3D<int16_t>;

}//namespace kns_test

#endif //__Segment3D_h
