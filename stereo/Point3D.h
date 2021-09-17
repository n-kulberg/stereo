#ifndef __Point3D_h
#define __Point3D_h

//------------------------------------------------------------------
//
//	created:	2021/09/17	18:06
//	filename: 	Point3D.h
//	file path:	Q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

#include "FieldObject.h"

namespace kns_test
{



template<class T>
class Point3D : public FieldObject<T, Point3D<T>, 3>
{
public:
	using parent = FieldObject<T, Point3D<T>, 3>;
	using value_type = T;
	using self = Point3D<T>;

	Point3D() = default;
	Point3D(const self&) = default;
	//since the class is trivial, move constructor is not needed
	Point3D(const T& in_x, const T& in_y, const T& in_z){ x()=in_x; y() = in_y; z() = in_z; }
	Point3D(T& in_x, const T& in_y, const T& in_z){ x()=in_x; y = in_y; z() = in_z; }

	template<class T2>
	self& operator=(const Point3D<T2>& other){ return parent::operator=(other); }
	self& operator=(const self& other){ return parent::operator=(other); }

	using parent::parent;
	//using parent::data;
	using parent::n_dimensions;


	T& x(){ return at(m_x); }
	T& y(){ return at(m_y); }
	T& z(){ return at(m_z); }
	const T& x() const { return at(m_x); }
	const T& y() const { return at(m_y); }
	const T& z() const { return at(m_z); }


	template<class T2> self vector_product(const Point3D<T2>& other) const
	{
		self result;
		result.x() = y()*other.z() - z()*other.y();
		result.y() = -x()*other.z() + z()*other.x();
		result.z() = x()*other.y() - y()*other.x();
		return result;
	}

	using parent::at;


protected:


private:
	const size_t m_z = 2, m_y = 1, m_x = 0;
};


}//namespace kns_test

#endif //__Point3D_h
