
#ifndef __Matrix3D_h
#define __Matrix3D_h

//------------------------------------------------------------------
//
//	created:	2021/09/17	18:12
//	filename: 	Matrix3D.h
//	file path:	Q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------


namespace kns_test
{

template<class T>
class Matrix3D : public FieldObject<Point3D<T>, Matrix3D<T>, 3>
{
public:
	using self = Matrix3D<T>;
	using parent = FieldObject<Point3D<T>, Matrix3D<T>, 3>;
	using value_type = Point3D<T>;
	using point_type = Point3D<T>;

	using parent::parent;
	template<class T2>
	self& operator=(const Matrix3D<T2>& other){ parent::operator=(other); return *this; }
	self& operator=(const self& other){ parent::operator=(other); return *this; }

	point_type multiply(const point_type& other)
	{
		point_type	result{at(0).scalar_product(other), at(1).scalar_product(other),at(2).scalar_product(other)};
		return result;
	}
	using parent::at;
private:
};

}//namespace kns_test


#endif //__Matrix3D_h


#include "Point3D.h"

