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

double	ComputeDistance(s3_F64 in_s1, s3_F64 in_s2)
{
	// 1. translate both vectors so that s1  is in the origin. Distance is preserved
	auto	s1 = in_s1 - in_s1.p1();
	auto	s2 = in_s2 - in_s1.p1();

	auto	ortho = cross_product(s1.radius_vector(), s2.radius_vector());

	cout << "ortho = " << ortho << endl;
	auto	rotation_matrix = rotation_matrix_to_z_axis(ortho);

	cout << "ortho rotated = " << matrix_multiply(rotation_matrix, ortho) << endl;

	Segment3D	s1r = matrix_multiply(rotation_matrix, s1);
	Segment3D	s2r = matrix_multiply(rotation_matrix, s2);

	cout << "s1 rotated = " << s1r << endl << "its radius-vector = " << s1r.radius_vector() << endl;
	cout << "s2 rotated = " << s2r << endl << "its radius-vector = " << s2r.radius_vector() << endl;
	//yes, one of them lays in plane that is parallel to XY, another coincides with it

	return 0;
}


}//namespace kns_test
