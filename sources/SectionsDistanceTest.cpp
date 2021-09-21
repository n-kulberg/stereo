#include "SectionsDistanceTest.h"
#include "ComputeDistance.h"
#include "MatrixUtilities.h"

#include "FieldOutput.h"

#include <iostream>

//------------------------------------------------------------------
//
//	created:	2021/09/21	17:09
//	filename: 	SectionsDistanceTest.cpp
//	file path:	q:\Projects\stereo\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

namespace kns_test
{

double	RandomUniform(double minval = -1, double maxval = 1)
{
	return minval + (maxval-minval)*(double(rand())/RAND_MAX);
}

m3_F64 RandomRotationMatrix()
{
	p3_F64 p(RandomUniform(), RandomUniform(), RandomUniform());
	if(is_almost_zero(l1_norma(p))) return identity_matrix();

	return rotation_matrix_to_z_axis(p);
}

//RandomIntersectingSections_xy()

pair<s3_F64, s3_F64> RandomSkewingSections(double z, bool intersect)
{
	s3_F64	s1
	{
		p3_F64{RandomUniform(0, 1), RandomUniform(0, 1), 0}, p3_F64{RandomUniform(-1, 0), RandomUniform(-1, 0), 0}
	};
	s3_F64	s2
	{
		p3_F64{RandomUniform(0, 1), RandomUniform(-1, 0), z}, p3_F64{RandomUniform(-1, z), RandomUniform(0, 1), z},
	};

	if(!intersect) s2 += p3_F64{RandomUniform(1, 2), RandomUniform(-2,-1), 0};

	auto	rm = RandomRotationMatrix();

	s1 = matrix_multiply(rm, s1);
	s2 = matrix_multiply(rm, s2);

	return make_pair(s1, s2);
}
	
}

using namespace kns_test;
using namespace std;

void	TestSegment3Distance()
{
// 	s3_F64	s1{p3_F64{1,1,0},p3_F64{1,2,3}};
// 	s3_F64	s2{p3_F64{3,2,7},p3_F64{-1,5,4}};
//	s3_F64	s2 = s1 + p3_F64{1,1,1};


	s3_F64	s1{p3_F64{0,0,0},p3_F64{1,0,0}};
	s3_F64	s2{p3_F64{0.75,1,0},p3_F64{1.250,-1,0}};


	cout << "s1 = " << s1 << endl << "its radius-vector = " << s1.radius_vector() << endl;
	cout << "s2 = " << s2 << endl << "its radius-vector = " << s2.radius_vector() << endl;



	auto d = ComputeDistance(s1, s2);

	cout << "Distance between s1 and s2 = " << d;

}



