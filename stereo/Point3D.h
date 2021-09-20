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


//! \brief Point in 3-dimension space.
//! \detailed Point in space is identical to its radius-vector. For this reason Point is element of the linear vector space (field). Thus, it is inherited from FieldObject class 
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

	Point3D(const T& in_x, const T& in_y, const T& in_z)
	{ 
		static_assert(m_x_position()==0 && m_y_position()==1 && m_z_position()==2, "Argument order in this constructor is xyz. To use other possible order (e.g., zyx) reassign positions functions");
		x()=in_x; 
		y()=in_y; 
		z()=in_z; 
	}

	//! use inherited constructors and operator=
	using parent::parent;
	using parent::operator=;
	
	using parent::n_dimensions;

	//! access to vector components
	T& x(){ return at(m_x_position()); }
	T& y(){ return at(m_y_position()); }
	T& z(){ return at(m_z_position()); }

	const T& x() const { return at(m_x_position()); }
	const T& y() const { return at(m_y_position()); }
	const T& z() const { return at(m_z_position()); }

	//! \brief vector cross product https://en.wikipedia.org/wiki/Cross_product
	//! NB, not operator* to avoid confusion with multiplication by a scalar
	template<class T2> self cross_product(const Point3D<T2>& other) const
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
	//! Functions that determine named vector components order in memory. It may be convenient for different tasks to use (x,y,z) or (z,y,x) order. 
	static constexpr size_t m_x_position() { return 0; }
	static constexpr size_t m_y_position() { return 1; }
	static constexpr size_t m_z_position() { return 2; }
};


//user-friendly type name aliases
using p3_F64 = Point3D<double>;
using p3_F32 = Point3D<float>;
using p3_I32 = Point3D<int32_t>;
using p3_I16 = Point3D<int16_t>;


}//namespace kns_test

#endif //__Point3D_h
