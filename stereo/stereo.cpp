//#include "Segment.h"

#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>

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

template<class T, class CHILD_T, size_t N>
class FieldObject
{
public:
	static constexpr size_t n_dimensions() { return N; }
	using value_type = T;
	using child_type = CHILD_T;
	using self = FieldObject<T, CHILD_T, N>;

	using iterator = T*;
	using const_iterator = const T *;

	iterator begin(){ return m_data; }
	iterator end(){ return m_data + n_dimensions(); }
	const_iterator cbegin() const { return m_data; }
	const_iterator cend() const { return m_data + n_dimensions(); }

	operator child_type& (){ return *reinterpret_cast<child_type*>(this); }//!TODO bad idea
	operator const child_type& () const { return *static_cast<child_type*>(this); }

	child_type& child_ref(){return (*this); }
	const child_type& child_ref() const { return (*this); }


	T& at(size_t i){ return m_data[i]; }
	const T& at(size_t i) const { return m_data[i]; }

	T& operator[](size_t i){ return m_data[i]; }
	const T& operator[](size_t i) const { return m_data[i]; }

//	const auto data() const{ return m_data; }

	// constructors
	FieldObject() = default;

	template<class T2, class C2>
	FieldObject(const FieldObject<T2, C2, N>& other){ std::copy(other.cbegin(), other.cend(), begin());}
	FieldObject(const self&) = default;

	template<class T2, class C2>
	child_type& operator=(const FieldObject<T2,C2,N>& other)
	{ 
		std::copy(other.cbegin(), other.cend(), begin()); 
		return child_ref(); 
	}
	
	child_type& operator=(const self& other)
	{
		std::copy(other.cbegin(), other.cend(), begin());
		return child_ref(); 
	}//template can not replace implicit operator=


	//since the class is trivial, move constructor is not needed

	template<class T2>
	FieldObject(const std::initializer_list<T2>& il)
	{
		if(il.size()!=n_dimensions()) throw std::invalid_argument("FieldObject::FieldObject(vector), invalid argument size");
		auto	d = begin();
		for(auto	it = il.begin(), ie = il.end(); it!=ie; ++it, ++d) *d=static_cast<T>(*it);//sorry, std::copy doesn't work for initializer list
	}


		// linear algebra operations
	template<class T2, class C2> child_type& operator += (const FieldObject<T2,C2,N>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x += y;}); return child_ref(); }
	template<class T2, class C2> child_type& operator -= (const FieldObject<T2,C2,N>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x -= y;}); return *this; }
	template<class ST> child_type& operator *= (const ST& scalar){ modify_binary_action(*this, scalar, [](T& x, const ST& y){x *= y;}); return *this; }
	template<class ST> child_type& operator /= (const ST& scalar){ modify_binary_action(*this, scalar, [](T& x, const ST& y){x /= y;}); return *this; }

	template<class T2, class C2> child_type operator + (const FieldObject<T2,C2,N>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x + y;}); }
	template<class T2, class C2> child_type operator - (const FieldObject<T2,C2,N>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x - y;}); }

	template<class ST> child_type operator * (const ST& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const ST& y){return x * y;}); }

	template<class ST> child_type operator / (const ST& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const ST& y){return x / y;}); }

	template<class T2, class C2> value_type scalar_product(const FieldObject<T2,C2,N>& other) const { return acquire_binary_action(*this, other, [](const T& x, const T2& y){return x * y;}); }
	double	l2_norma() const{ return sqrt(double(scalar_product(*this))); }


private:
	T	m_data[n_dimensions()];

		// data operation algorithms
	template<class T2, class C2, class F>
	void	modify_binary_action(self& p1, const FieldObject<T2,C2,N>& p2, F f)
	{
		for(size_t i = 0; i < n_dimensions(); ++i) f(p1.at(i), p2.at(i));
	}
	template<class ST, class F>
	void	modify_binary_action(self& p1, const ST& scalar, F f)
	{
		for(size_t i = 0; i < n_dimensions(); ++i) f(p1.at(i), scalar);
	}
	template<class T2, class C2, class F>
	child_type	return_binary_action(const self& p1, const FieldObject<T2,C2,N>& p2, F f) const
	{
		//v
		child_type result;
		for(size_t i = 0; i < n_dimensions(); ++i) result.at(i) = f(p1.at(i), p2.at(i));
		return result;
	}
	template<class T2, class F>
	child_type	return_binary_action(const self& p1, const T2& scalar, F f) const
	{
		//s
		child_type result;
		for(size_t i = 0; i < n_dimensions(); ++i) result.at(i) = f(p1.at(i), scalar);
		return result;
	}

	template<class T2, class C2, class F>
	value_type	acquire_binary_action(const self& p1, const FieldObject<T2,C2,N>& p2, F f) const
	{
		value_type result(0);
		for(size_t i = 0; i < n_dimensions(); ++i) result += f(p1.at(i), p2.at(i));
		return result;
	}

};




template<class T>
class Point3D : public FieldObject<T, Point3D<T>, 3>
{
public:
	using parent = FieldObject<T, Point3D<T>, 3>;
	using value_type = T;
	using self = Point3D<T>;

	Point3D() = default;
	Point3D(const self&) = default;
	//since the class is trivial, move constructor is not needed
	Point3D(const T& in_x, const T& in_y, const T& in_z){ x()=in_x; y() = in_y; z() = in_z; }
	Point3D(T& in_x, const T& in_y, const T& in_z){ x()=in_x; y = in_y; z() = in_z; }

	template<class T2>
	self& operator=(const Point3D<T2>& other){ return parent::operator=(other); }
	self& operator=(const self& other){ return parent::operator=(other); }

	using parent::parent;
	//using parent::data;
	using parent::n_dimensions;


	T& x(){ return at(m_x); }
	T& y(){ return at(m_y); }
	T& z(){ return at(m_z); }
	const T& x() const { return at(m_x); }
	const T& y() const { return at(m_y); }
	const T& z() const { return at(m_z); }


	template<class T2> self vector_product(const Point3D<T2>& other) const
	{
		self result;
		result.x() = y()*other.z() - z()*other.y();
		result.y() = -x()*other.z() + z()*other.x();
		result.z() = x()*other.y() - y()*other.x();
		return result;
	}

	using parent::at;


protected:


private:
	const size_t m_z = 2, m_y = 1, m_x = 0;
};


using point3 = Point3D<double>;

template<class T>
class Segment3D : public FieldObject<Point3D<T>, Segment3D<Point3D<T>>, 2>
{
public:
	using self = Segment3D<T>;
	using parent = FieldObject<Point3D<T>, Segment3D<Point3D<T>>, 2>;
	using value_type = Point3D<T>;
	using point_type = Point3D<T>;

 	using parent::parent;
	template<class T2>
	self& operator=(const Segment3D<T2>& other){ parent::operator=(other); return *this; }
	self& operator=(const self& other){ parent::operator=(other); return *this; }

	point_type& p1(){ return at(m_p1); }
	point_type& p2(){ return at(m_p2); }
	
	const point_type& p1() const { return at(m_p1); }
	const point_type& p2() const { return at(m_p2); }

	point_type	radius_vector() const { return p2()-p1(); };
	double length() const{ return l2_norma(radius_vector()); }

	using parent::at;

private:
	const size_t m_p1 = 0;
	const size_t m_p2 = 1;
};


template<class T>
class Matrix3D : public FieldObject<Point3D<T>, Matrix3D<Point3D<T>>, 3>
{
public:
	using self = Matrix3D<T>;
	using parent = FieldObject<Point3D<T>, Matrix3D<Point3D<T>>, 3>;
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


// output auxiliaries

template<class T>
struct separator
{
	static string prefix(){ return ""; }
	static string get(){ return ";"; }
};

template<class T>
struct separator<Point3D<T>>
{
	static string prefix(){ return "\n"; }
	static string get(){ return "\n"; }
};

template<class T>
struct separator<Matrix3D<T>>
{
	static string prefix(){ return "\n"; }
	static string get(){ return "\n"; }
};

// output

template<class T, class CHILD_T, size_t N>
std::ostream& operator << (std::ostream& str, const FieldObject<T, CHILD_T, N>& f)
{
	str << separator<T>::prefix() << "{";
	std::for_each(f.cbegin(), f.cend()-1, [&str](const T& x){str << x << separator<T>::get();});
	str << *(f.cend()-1) << "}";
	return str;
}

}//namespace kns_test

using namespace kns_test;


void	TestMatrix()
{
	cout << "---------------Test segment------------------------" << endl;
	Matrix3D<double>	m1{Point3D{1,0,0},Point3D{0,2,0},Point3D(0,0,3)};
	cout << "m1" << m1 << endl;

	cout << "m1*v = " << endl << m1.multiply({1,1,1}) << endl;

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

	cout << "s1 = \n" << s1 << endl;
	cout << "s2 = \n" << s2 << endl;

// 	s1+=s1;
 	s1*=2;
//	s1 * 2;
//	s1+s2;
	cout << "s1*=s2 = \n" << s1 << endl;

	vector<point3> v{p1, p2};
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