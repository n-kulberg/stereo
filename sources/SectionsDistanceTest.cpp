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

pair<s3_F64, s3_F64> RandomParallelSections(double y, double z)
{
	s3_F64 s1 ={p3_F64{0,0,0}, p3_F64{0,0,1}};
	s3_F64 s2 ={p3_F64{0,y,z}, p3_F64{0,y,1+z}};

	auto	rm = RandomRotationMatrix();

	s1 = matrix_multiply(rm, s1);
	s2 = matrix_multiply(rm, s2);

	return make_pair(s1, s2);
}


pair<s3_F64, s3_F64> RandomColinearySections(double z)
{
	s3_F64 s1 ={p3_F64{0,0,0}, p3_F64{0,0,1}};
	s3_F64 s2 ={p3_F64{0,0,z}, p3_F64{0,0,1+z}};

	auto	rm = RandomRotationMatrix();

	s1 = matrix_multiply(rm, s1);
	s2 = matrix_multiply(rm, s2);

	return make_pair(s1, s2);
}


pair<s3_F64, s3_F64> RandomSkewingSections(double z, bool intersect)
{
	s3_F64	s1, s2;
	if(intersect)
	{
		s1 = {p3_F64{RandomUniform(1, 2), RandomUniform( 1, 2), 0}, p3_F64{RandomUniform(-2, -1), RandomUniform(-2, -1), 0}};
		s2 = {p3_F64{RandomUniform(1, 2), RandomUniform(-2, -1), z}, p3_F64{RandomUniform(-2,-1), RandomUniform( 1, 2), z}};
	}
	else
	{
		s1 ={p3_F64{RandomUniform(-1, -2), 0, 0}, p3_F64{RandomUniform(1, 2), 0, 0}};
		double	fi = RandomUniform(0, 3.1415926/2);
		double	x = RandomUniform(s1.p1().x(), s1.p2().x());
		double y0 = z*cos(fi);
		s2 ={p3_F64{x, y0, z*sin(fi)}, p3_F64{x, y0 + RandomUniform(0,2), z*sin(fi)}};		
	}

	auto	rm = RandomRotationMatrix();

// 	s1 = matrix_multiply(rm, s1);
// 	s2 = matrix_multiply(rm, s2);

	return make_pair(s1, s2);
}
	
}

using namespace kns_test;
using namespace std;

void test_skewing(double param, bool intersect_xy)// testing skew sections that may intersect in XY plane or not
{
 		auto	sp = RandomSkewingSections(param, intersect_xy); //passed OK

		double	dist = ComputeDistance(sp.first, sp.second);
		cout << endl << "\tpre-defined param=" << param << "\testimated distance=" << dist;
		if(dist != param)
		{
			cout << "err";
			ComputeDistance(sp.first, sp.second);
		}
}

void test_colineary(double param)// testing colineary sections
{
	auto	sp = RandomColinearySections(param); //Estimated distance must max(z-1, 0). passed OK
	double	dist = ComputeDistance(sp.first, sp.second);
	double true_distance = std::max(param-1., 0.);
	cout << endl << "\tpre-defined param=" << param << "\testimated distance=" << dist << "\ttrue distance =" << true_distance;
}

void test_parallel(double y, double param)// testing parallel sections
{
	auto	sp = RandomParallelSections(y, param); //Estimated distance must hypot(1, max(z-1, 0)). passed OK
	double	dist = ComputeDistance(sp.first, sp.second);
	double true_distance = hypot(y, std::max(param-1., 0.));
	cout << endl << "\testimated distance=" << dist << "\ttrue distance = " << true_distance << endl;
}


void	TestSegment3DistanceAuto()
{
	clock_t	t = clock();
	srand(t);
	size_t	N = 20;
	for(size_t i = 0; i < N; ++i)
	{
		double	z = i*0.25;
		double y = 1;

		cout << "\nTest #" << i << ".";
		test_skewing(z, true);
// 		test_skewing(z, false);
// 		test_colineary(z);
// 		test_parallel(y,z);

	}
}


void	TestSegment3DistanceManual()
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



