
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

#include "Point3D.h"
#include "Segment3D.h"

#include <stdexcept>
#include <cmath>

namespace kns_test
{

template<class T>
class Matrix3D : public FieldObject<Point3D<T>, Matrix3D<T>, 3>
{
public:
	using self = Matrix3D<T>;
	using parent = FieldObject<Point3D<T>, Matrix3D<T>, 3>;
	using value_type = T;
	using	row_type = Point3D<T>;

	using parent::parent;
	using parent:: operator=;

	//! access functions
	value_type& at(size_t i, size_t j) noexcept { return row(i).at(j); }
	const value_type& at(size_t i, size_t j) const noexcept { return row(i).at(j); }
	using parent::operator [];
	row_type& row(size_t i) noexcept { return parent::at(i); }
	const row_type& row(size_t i) const noexcept { return parent::at(i); }


	self transposed() const noexcept
	{
		return self	{ row_type{at(0,0), at(1,0), at(2,0)},	row_type{at(0,1), at(1,1), at(2,1)}, row_type{at(0,2), at(1,2), at(2,2)}};
	}


	//! \brief Left multiply matrix by vector
	template<class T2>
	row_type multiply(const Point3D<T2> & other) const noexcept
	{
		return row_type	{scalar_product(row(0), other), scalar_product(row(1), other), scalar_product(row(2), other)};
	}

	//! \brief Multiply matrix by matrix
	template<class T2>
	self multiply(const Matrix3D<T2>& other) const noexcept
	{
		auto tr = other.transposed();
		return self
		{
			row_type{scalar_product(row(0), tr.row(0)), scalar_product(row(0), tr.row(1)), scalar_product(row(0), tr.row(2))},
			row_type{scalar_product(row(1), tr.row(0)), scalar_product(row(1), tr.row(1)), scalar_product(row(1), tr.row(2))},
			row_type{scalar_product(row(2), tr.row(0)), scalar_product(row(2), tr.row(1)), scalar_product(row(2), tr.row(2))},
		};
	}

private:
	//Function parent::at(size_t) is not allowed to avoid confusion with redefined operator at(size_t, size_t).
	//But parent::operator[] is available
	using parent::at;
};



//! user-friendly typename aliases
using m3_F64 = Matrix3D<double>;
using m3_F32 = Matrix3D<float>;
using m3_I32 = Matrix3D<int32_t>;
using m3_I16 = Matrix3D<int16_t>;

//! non-member functions for matrix multiply
template<class T1, class T2>
auto	matrix_multiply(const Matrix3D<T1> &m, const Point3D<T2> &p) noexcept
{
	return m.multiply(p);
}

template<class T1, class T2>
auto	matrix_multiply(const Matrix3D<T1>& m, const Matrix3D<T2>& m2) noexcept
{
	return m.multiply(m2);
}

//! Apply transform to segment
template<class T1, class T2>
auto	matrix_multiply(const Matrix3D<T1>& m, const Segment3D<T2>& seg) noexcept
{
	return Segment3D<T1>{m.multiply(seg.p1()), m.multiply(seg.p2())};
}


// utilities

template<class T=double> 
Matrix3D<T> identity_matrix() noexcept
{
	return Matrix3D<T>{ Point3D<T>{ 1, 0, 0 }, Point3D<T>{0,1,0}, Point3D<T>{0,0,1} };
}


template<class T>
m3_F64	rotation_matrix_to_z_axis(const Point3D<T>& p)
{
	const double	epsilon = 1e-9;
	double	r_xyz = p.l2_norma();
	if(!std::isnormal(p.l2_norma())) throw std::invalid_argument("");

	double	r_xy = hypot(p.x(), p.y());
	if(r_xy/r_xyz < epsilon) return identity_matrix();//no rotation is needed

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


#endif //__Matrix3D_h


#include "Point3D.h"

