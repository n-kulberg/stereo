﻿
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


template<class T=double> 
Matrix3D<T> identity_matrix()
{
	return Matrix3D<T>{ Point3D<T>{ 1, 0, 0 }, Point3D<T>{0,1,0}, Point3D<T>{0,0,1} };
}

//! user-friendly typename aliases
using m3_F64 = Matrix3D<double>;
using m3_F32 = Matrix3D<float>;
using m3_I32 = Matrix3D<int32_t>;
using m3_I16 = Matrix3D<int16_t>;

}//namespace kns_test


#endif //__Matrix3D_h


#include "Point3D.h"

