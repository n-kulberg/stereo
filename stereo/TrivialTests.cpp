#include "TrivialTests.h"
#include "Matrix3D.h"
#include "Segment3D.h"
#include "FieldOutput.h"
#include <cmath>
//------------------------------------------------------------------
//
//	created:	2021/09/20	15:01
//	filename: 	TrivialTests.cpp
//	file path:	q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

using namespace kns_test;

void	TestMatrix()
{
	cout << "---------------Test matrix------------------------" << endl;

	m3_F64	m1{p3_F64{1,0,0},p3_F64{0,2,0},p3_F64(0,0,3)};
	p3_F64 v{1,2,3};

	cout << "m1" << m1 << endl;
	cout << "v" << v << endl;

	cout << "m1*v = " << m1.multiply(v) << endl;

	auto E = identity_matrix();

	cout << "E" << E << endl;
	cout << "m1 X E = " << m1.multiply(E) << endl;
	cout << "E X m1/2 = " << E.multiply(m1/2) << endl;


	double	fi = -3.1415926/2;
	m3_F64 rotxy =
	{
		p3_F64{cos(fi),-sin(fi),0},
		p3_F64{sin(fi),cos(fi),0},
		p3_F64{0,0,1}
	};

	cout << "rotxy X v = " << rotxy.multiply(v) << endl;

	p3_F64	rv{1,1,1};
	auto	rm = rotation_matrix_to_z_axis(rv);
	cout << "rv = " << rv << endl;
	cout << "rm = " << rm << endl;
	cout << "rm x rv" << rm.multiply(rv) << endl;
}

void	TestSegment()
{
	cout << "---------------Test segment------------------------" << endl;
	s3_F64	s1{p3_F64{1,1,0},p3_F64{2,0,2}};


	p3_F64	p1 ={1,2,3};
	p3_F64	p2 ={4,5,6};
	p3_F64 p3;
	p3 = p2;


	s3_F64	s2{p1, p2};
	s3_F64 s3;
	s3 = s2;

	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;

	cout << "s1*2 = " << s1*2 << endl;
	cout << "s1*=s2 = " << (s1*=2) << endl;
	cout << "s1+=s1 = " << (s1+=s1) << endl;

}



void	TestArithmeticsPoint()
{
	cout << "---------------Test point------------------------" << endl;
	p3_F64	p1 ={1,2,3};
	p3_F32	p2 ={2,1,-2};

	std::cout << "p1 = " << p1 << endl;
	std::cout << "p2 = " << p2 << endl;
	std::cout << "p1+p2 = " << p1+p2 << endl;
	std::cout << "scalar_product(p1,p2) = " << p1.scalar_product(p2) << endl;
	std::cout << "p1*2 = " << p1*2. << endl;
	std::cout << "p1+=p2 = " << (p1+=p2) << endl;

	std::cout << "sp(scalar,scalar) = " << scalar_product(2, 3) << endl;
	std::cout << "sp(vector,vector) = " << scalar_product(p3_F64{1,2,3}, p3_F64{3,2,1}) << endl;


	p3_F64	v1 ={0,0,1};
	p3_F64	v2 ={0,1,0};

	//trivial vector product check
	std::cout << "v1 = " << v1 << endl;
	std::cout << "v2 = " << v2 << endl;
	std::cout << "v1 X v2 = " << v1.cross_product(v2) << endl;
	std::cout << "v2 X v1 = " << v2.cross_product(v1) << endl;

	//collinearity check
	std::cout << "p1 X p1 = " << p1.cross_product(p1) << endl;
	std::cout << "(p1 X p2) = " << p1.cross_product(p2) << endl;

	//orthogonality check
	std::cout << "(p1 X p2)*p1 = " << (p1.cross_product(p2)).scalar_product(p1) << endl;
	std::cout << "(p1 X p2)*p2 = " << (p1.cross_product(p2)).scalar_product(p2) << endl;



}
