//#include "Segment.h"

#include "Segment3D.h"
#include "Matrix3D.h"

#include "FieldOutput.h"



//------------------------------------------------------------------
//
//	created:	2021/09/16	17:00
//	filename: 	Segment.cpp
//	file path:	q:\Projects\Sandbox\Sandbox\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

using namespace std;

namespace kns_test
{


using point3 = Point3D<double>;




}//namespace kns_test

using namespace kns_test;


void	TestMatrix()
{
	cout << "---------------Test segment------------------------" << endl;
	Matrix3D<double>	m1{Point3D{1,0,0},Point3D{0,2,0},Point3D(0,0,3)};
	cout << "m1" << m1 << endl;

	cout << "m1*v = " << m1.multiply({1,1,1}) << endl;

	m1 /= 2;
	m1[2][2] = 10;
	cout << m1;
}

void	TestSegment()
{
	cout << "---------------Test segment------------------------" << endl;
	Segment3D<double>	s1 {Point3D{1,1,0},Point3D{2,0,2}};

	point3	p1 ={1,2,3};
	point3	p2 ={4,5,6};
	point3 p3;
	p3 = p2;

	Segment3D<double>	s2 {p1, p2};
	Segment3D<double> s3;
	s3 = s2;

	cout << "s1 = " << s1 << endl;
	cout << "s2 = " << s2 << endl;

// 	s1+=s1;
 	s1*=2;
//	s1 * 2;
//	s1+s2;
	cout << "s1*=s2 = " << s1 << endl;

}



void	TestArithmeticsPoint()
{
	cout << "---------------Test point------------------------" << endl;
	Point3D<double>	p1 ={1,2,3};
	Point3D<float>	p2 ={2,1,-2};

	std::cout << "p1 = " << p1 << endl;
	std::cout << "p2 = " << p2 << endl;
	std::cout << "p1+p2 = " << p1+p2 << endl;
	std::cout << "sp(p1,p2) = " << p1.scalar_product(p2) << endl;
	std::cout << "p1*2 = " << p1*2. << endl;
	std::cout << "p1+=p2 = " << (p1+=p2) << endl;

	Point3D<double>	v1 ={0,0,1};
	Point3D<double>	v2 ={0,1,0};

	//trivial vector product check
	std::cout << "v1 = " << v1 << endl;
	std::cout << "v2 = " << v2 << endl;
	std::cout << "v1 X v2 = " << v1.vector_product(v2) << endl;
	std::cout << "v2 X v1 = " << v2.vector_product(v1) << endl;

	//collinearity check
	std::cout << "p1 X p1 = " << p1.vector_product(p1) << endl;
	std::cout << "(p1 X p2) = " << p1.vector_product(p2) << endl;

	//orthogonality check
	std::cout << "(p1 X p2)*p1 = " << (p1.vector_product(p2)).scalar_product(p1) << endl;
	std::cout << "(p1 X p2)*p2 = " << (p1.vector_product(p2)).scalar_product(p2) << endl;



}

int	main(int, char **)
{
	try
	{
		TestArithmeticsPoint();
		TestSegment();
		TestMatrix();
		fflush(stdout);

	}
	catch(exception& ex)
	{
		cout << "An error occured:" << endl << ex.what();
	}


	int n;
	std::cin >> n;
	// to avoid console close

	return 0;
}