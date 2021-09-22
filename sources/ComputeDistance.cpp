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
	ortho = cross_product(rv1, std::max(s2.p1(), s2.p2()));
	if(!is_almost_zero(l1_norma(ortho))) 
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

bool	SectionsIntersect(const s3_F64 &s1, const s3_F64 &s2)
{
	//s1.p1 = origin
	//s1 has only nonzero coordinate x
	//s2 has nonzero x and y

	if(!is_almost_zero(l1_norma(s1.p1()))) 
	{
		throw invalid_argument("SectionsCross, not prepared s1.p1");
	}
	if(s1.p2().x() < 0 )
	{
		throw invalid_argument("SectionsCross, not prepared s1.p2.x");
	}
	if(!is_almost_zero(s1.p2().y()) || !is_almost_zero(s1.p2().z())) 
	{
		throw invalid_argument("SectionsCross, not prepared s1.z");
	}
	if(!is_almost_zero(s2.p1().z()) || !is_almost_zero(s2.p2().z())) 
	{
		throw invalid_argument("SectionsCross, not prepared s2.z");
	}

	auto	s2_rv = s2.radius_vector();

	auto	belongs_to_s1 = [&s1](double x)
	{
		// point x belongs to s1
		if(x >= 0 && x <= s1.p2().x()) return true;
		return false;
	};

	// s2 is parallel to x axis, or lays in it, or s2 is zero length
	if(is_almost_zero(s2_rv.y()) || is_almost_zero(s2_rv.l1_norma()))
	{
		// s2 is parallel to x axis, no intersect
		if(!is_almost_zero(s2.p1().y())) return false;
		
		// s2 lays in x axis
		return belongs_to_s1(s2.p1().x()) || belongs_to_s1(s2.p2().x());
	}

	//section s2 does not intersect x axis
	if(s2.p1().y() * s2.p2().y() > 0) return false;

	//section s2 intersects x axis. find intersection point of s2 line and x axis
	double	factor = -s2.p1().y()/s2_rv.y();
	double	x = s2.p1().x() + factor*s2_rv.x();

	return belongs_to_s1(x);
}

double	distance_to_point(s3_F64 s, p3_F64 p)
{
//	if(s.p1().z()) throw invalid_argument("distance_to_point, not prepared s1.p2.x");

	p -= s.p1();
	s-= s.p1();

	//p coincides with origin and with start of s
	if(is_almost_zero(l1_norma(p))) return 0;
	//s is zero-length, both its ends coincide with origin
	if(is_almost_zero(l1_norma(s.radius_vector()))) return l2_norma(p);

	auto	rotation_matrix = rotation_matrix_to_x_axis(s.p2());

	p = matrix_multiply(rotation_matrix, p);
	s = matrix_multiply(rotation_matrix, s);
	//s section now lays in x axis and begins at zero

	if(s.p2().x() < 0)
	{
		s *= -1;
		p *= -1;
	}

	if(p.x() >= 0 && p.x() <=s.p2().x()) return abs(p.y());
	else if(p.x() > s.p2().x()) return l2_norma(s.p2() - p);
	else return l2_norma(p);
}


//! \brief Compute shortest distance between two points of s1 and s2 sections. s1 belongs to xy plane s2 is parallel to it. z coordinate is ignored
double	ComputeDistanceXY(s3_F64 s1, s3_F64 s2)
{
	auto	rotation_matrix = rotation_matrix_to_x_axis(s1.p2());

	s1 = matrix_multiply(rotation_matrix, s1);
	s2 = matrix_multiply(rotation_matrix, s2);

	if(s1.p2().x() < 0)
	{
		s1 *= -1;
		s2 *= -1;
	}

	s2.p1().z() = s2.p2().z() = 0;

	if(SectionsIntersect(s1, s2)) return 0;

// 	cout << "\ns1=" << s1 << endl;
// 	cout << "\ns2=" << s2 << endl;

	double	r1 = distance_to_point(s1, s2.p1());
	double	r2 = distance_to_point(s1, s2.p2());
	double	r3 = distance_to_point(s2, s1.p1());
	double	r4 = distance_to_point(s2, s1.p2());

	return std::min(std::min(r1, r2), std::min(r3, r4));
}

//! \brief Compute shortest distance between two points of s1 and s2 sections
double	ComputeDistance(s3_F64 s1, s3_F64 s2)
{
	// 1. Ensure that s1>s2
	if(s1.radius_vector() < s2.radius_vector()) swap(s1, s2);

	// 2. translate both vectors so that s1  is in the origin. Distance is preserved
	s2 -= s1.p1();
	s1 -= s1.p1();

// 	cout << "\nAfter swap and move to origin" << endl;
// 	cout << "s1 rotated to xy = " << s1 << endl << "its radius-vector = " << s1.radius_vector() << endl;
// 	cout << "s2 rotated to xy = " << s2 << endl << "its radius-vector = " << s2.radius_vector() << endl;

	// 3. Check if s1 is zero length. Since it is longer, both sections are zero-length. Distance is the length of s2.p1()
	if(is_almost_zero(l1_norma(s1.p2()))) return l2_norma(s2.p1());

	auto	rotation_matrix = make_parallel_to_xy_transform(s1,s2);


	s1 = matrix_multiply(rotation_matrix, s1);
	s2 = matrix_multiply(rotation_matrix, s2);

// 	cout << "\ns1 rotated to xy = " << s1 << endl << "its radius-vector = " << s1.radius_vector() << endl;
// 	cout << "s2 rotated to xy = " << s2 << endl << "its radius-vector = " << s2.radius_vector() << endl;
	//yes, s2 lays in plane that is parallel to XY, s1 lays in XY plane

	return hypot(s2.p1().z(), ComputeDistanceXY(s1, s2));
}


}//namespace kns_test
