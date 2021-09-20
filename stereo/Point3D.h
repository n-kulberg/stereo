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
	using parent::n_dimensions;


	T& x(){ return at(m_x_position()); }
	T& y(){ return at(m_y_position()); }
	T& z(){ return at(m_z_position()); }

	const T& x() const { return at(m_x_position()); }
	const T& y() const { return at(m_y_position()); }
	const T& z() const { return at(m_z_position()); }


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
	constexpr size_t m_x_position() const { return 0; }
	constexpr size_t m_y_position() const { return 1; }
	constexpr size_t m_z_position() const { return 2; }
};


//user-friendly type name aliases
using p3_F64 = Point3D<double>;
using p3_F32 = Point3D<float>;
using p3_I32 = Point3D<int32_t>;
using p3_I16 = Point3D<int16_t>;


}//namespace kns_test

#endif //__Point3D_h
