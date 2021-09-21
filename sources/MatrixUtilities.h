#ifndef __MatrixUtilities_h
#define __MatrixUtilities_h

//------------------------------------------------------------------
//
//	created:	2021/09/21	12:07
//	filename: 	MatrixUtilities.h
//	file path:	q:\Projects\stereo\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

#include "Matrix3D.h"
#include "Epsilon.h"

namespace kns_test
{


//!	Returns matrix that rotates the point p so that it lays on z axis
template<class T>
m3_F64	rotation_matrix_to_z_axis(const Point3D<T>& p)
{
	double	r_xyz = p.l2_norma();
	if(!std::isnormal(r_xyz) || is_almost_zero(r_xyz)) throw std::invalid_argument("rotation_matrix_to_z_axis(const Point3D<T>& p), wrong point");

	double	r_xy = hypot(p.x(), p.y());
	if(is_small_o(r_xy, r_xyz)) return identity_matrix();//no rotation is needed

	double	cosfi = p.x()/r_xy;
	double	sinfi = p.y()/r_xy;
	double	costheta = p.z()/r_xyz;
	double	sintheta = r_xy/r_xyz;

	//rotating vector parallel to xy plane. vector is moved to xz plane, its y component is set to zero. z component is not influenced
	m3_F64	m_rotate_in_xy_plane_to_zero_y_component =
	{
		p3_F64{cosfi, sinfi, 0},
		p3_F64{-sinfi, cosfi, 0},
		p3_F64{0, 0, 1},
	};

	//rotating vector parallel to xz plane. vector is moved to z axis, its x component is set to zero. y component is not influenced
	m3_F64	m_rotate_in_xz_plane_to_zero_x_component =
	{
		p3_F64{costheta, 0, -sintheta},
		p3_F64{0, 1, 0},
		p3_F64{sintheta, 0, costheta},
	};
	// superposition of two rotations
	return matrix_multiply(m_rotate_in_xz_plane_to_zero_x_component, m_rotate_in_xy_plane_to_zero_y_component);
}


}//namespace kns_test

#endif //__MatrixUtilities_h
