#include "ComputeDistance.h"
#include "Matrix3D.h"
#include "FieldOutput.h"
#include "MatrixUtilities.h"

#include <iostream>

//------------------------------------------------------------------
//
//	created:	2021/09/21	9:23
//	filename: 	ComputeDistance.cpp
//	file path:	q:\Projects\stereo\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

namespace kns_test
{

using namespace std;

//! \brief Compute rotation matrix that makes two sections s1 and s2 parallel to XY plane
m3_F64	make_parallel_to_xy_transform(const s3_F64 s1, const s3_F64 s2)
{
	auto	rv1 = s1.radius_vector();
	auto	rv2 = s2.radius_vector();
	//NB we know that rv1>=rv2

	//try to find vector orthogonal to rv1 and rv2
	auto	ortho = cross_product(rv1, rv2);
	if(!is_almost_zero(l1_norma(ortho)))
	{
		//s1 and s2 are neither parallel nor collinear nor zero-length
		// i.e. they lay on the intersecting or skew lines. This is the most probable variant

		// put s1 to xy axis, make s2 parallel to xy axis
		return rotation_matrix_to_z_axis(ortho);
	}

	// Maybe, they are just parallel, but not collinear or zero?
	ortho = cross_product(rv1, s2.p1());
	if(is_almost_zero(l1_norma(ortho))) 
	{
		//Just put them both to the xy axis
		return rotation_matrix_to_z_axis(ortho);
	}

	if(!is_almost_zero(l1_norma(rv1)))
	{
		// both sections lay on the same line. Just put them to x axis
		return rotation_matrix_to_x_axis(rv1);
	}
	
	//both vectors are zero-length, no rotation is needed
	return identity_matrix();
}

//! \brief Compute shortest distance between two points of s1 and s2 sections. s1 belongs to xy plane s2 is parallel to it. z coordinate is ignored
double	ComputeDistanceXY(s3_F64 s1, s3_F64 s2)
{
	auto	rotation_matrix = rotation_matrix_to_x_axis(s1.p2());

	s1 = matrix_multiply(rotation_matrix, s1);
	s2 = matrix_multiply(rotation_matrix, s2);

	cout << "s1 rotated to xy = " << s1 << endl << "its radius-vector = " << s1.radius_vector() << endl;
	cout << "s2 rotated to xy = " << s2 << endl << "its radius-vector = " << s2.radius_vector() << endl;

	return 0;
}

//! \brief Compute shortest distance between two points of s1 and s2 sections
double	ComputeDistance(s3_F64 s1, s3_F64 s2)
{
	// 1. Ensure that s1>s2
	if(s1.radius_vector() < s2.radius_vector()) swap(s1, s2);

	// 2. translate both vectors so that s1  is in the origin. Distance is preserved
	s2 -= s1.p1();
	s1 -= s1.p1();

	// 3. Check if s1 is zero length. Since it is longer, both sections are zero-length. Distance is the length of s2.p1()
	if(is_almost_zero(l1_norma(s1.p2()))) return l2_norma(s2.p1());

	auto	rotation_matrix = make_parallel_to_xy_transform(s1,s2);


	s1 = matrix_multiply(rotation_matrix, s1);
	s2 = matrix_multiply(rotation_matrix, s2);

	cout << "s1 rotated to xy = " << s1 << endl << "its radius-vector = " << s1.radius_vector() << endl;
	cout << "s2 rotated to xy = " << s2 << endl << "its radius-vector = " << s2.radius_vector() << endl;
	//yes, s2 lays in plane that is parallel to XY, s1 lays in XY plane

	return hypot(s2.p1().z(), ComputeDistanceXY(s1, s2));
}


}//namespace kns_test
