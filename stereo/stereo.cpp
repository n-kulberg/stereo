//#include "Segment.h"

#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <iostream>

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

namespace kns_test
{
template<class T>
class Point3D
{
public:

	using value_type = T;
	using self = Point3D<T>;

	Point3D() = default;
	Point3D(const self&) = default;
	//since the class is trivial, move constructor is not needed
	Point3D(const T& in_z, const T& in_y, const T& in_x){ x()=in_x; y() = in_y; z() = in_z; }
	Point3D(T& in_z, const T& in_y, const T& in_x){ x()=in_x; y = in_y; z() = in_z; }

	template<class T2>
	Point3D(const std::vector<T2>& il)
	{
		if(il.size()!=3) throw std::invalid_argument("Point3D::Point3D(initializer_list), invalid argument size");
		std::copy(il.begin(), il.end(), m_data);
	}

	T& x(){ return m_data[m_x]; }
	T& y(){ return m_data[m_y]; }
	T& z(){ return m_data[m_z]; }
	const T& x() const { return m_data[m_x]; }
	const T& y() const { return m_data[m_y]; }
	const T& z() const { return m_data[m_z]; }

	// linear algebra operations
	template<class T2> self& operator += (const Point3D<T2>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x += y;}); return *this; }
	template<class T2> self& operator -= (const Point3D<T2>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x -= y;}); return *this; }
	template<class T2> self& operator *= (const T2& scalar){ modify_binary_action(*this, scalar, [](T& x, const T2& y){x *= y;}); return *this; }
	template<class T2> self& operator /= (const T2& scalar){ modify_binary_action(*this, scalar, [](T& x, const T2& y){x /= y;}); return *this; }

	template<class T2> self operator + (const Point3D<T2>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x + y;}); }
	template<class T2> self operator - (const Point3D<T2>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x - y;}); }
	template<class T2> self operator * (const T2& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const T2& y){return x * y;}); }
	template<class T2> self operator / (const T2& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const T2& y){return x / y;}); }

	template<class T2> value_type scalar_product(const Point3D<T2>& other) const { return acquire_binary_action(*this, other, [](const T& x, const T2& y){return x * y;}); }
	double	l2_norma() const{ return sqrt(double(scalar_product(*this))); }

	template<class T2> self vector_product(const Point3D<T2>& other) const
	{
		self result;
		result.x() = y()*other.z() - z()*other.y();
		result.y() = -x()*other.z() + z()*other.x();
		result.z() = x()*other.y() - y()*other.x();
		return result;
	}


	const auto data() const{ return m_data; }

protected:
private:

	const size_t m_z = 0, m_y = 1, m_x = 2;
	T	m_data[3];

	// data operation algorithms
	template<class T2, class F>
	void	modify_binary_action(self& p1, const Point3D<T2>& p2, const F& f)
	{
		for(size_t i = 0; i < 3; ++i) f(p1.m_data[i], *(p2.data() + i));
	}
	template<class T2, class F>
	void	modify_binary_action(self& p1, const T2& scalar, const F& f)
	{
		for(size_t i = 0; i < 3; ++i) f(p1.m_data[i], scalar);
	}
	template<class T2, class F>
	self	return_binary_action(const self& p1, const Point3D<T2>& p2, const F& f) const
	{
		self result;
		for(size_t i = 0; i < 3; ++i) result.m_data[i] = f(p1.m_data[i], *(p2.data() + i));
		return result;
	}
	template<class T2, class F>
	self	return_binary_action(const self& p1, const T2& scalar, const F& f) const
	{
		self result;
		for(size_t i = 0; i < 3; ++i) result.m_data[i] = f(p1.m_data[i], scalar);
		return result;
	}

	template<class T2, class F>
	value_type	acquire_binary_action(const self& p1, const Point3D<T2>& p2, const F& f) const
	{
		value_type result(0);
		for(size_t i = 0; i < 3; ++i) result += f(p1.data()[i], p2.data()[i]);
		return result;
	}
};

}//namespace xrad

int	main(int, char **)
{
	using namespace kns_test;

	int	m[3];
	std::vector<int> v(3);
	std::copy(v.begin(), v.end(), m);

	Point3D<double>	p1 ={1,2,3};
	Point3D<float>	p2 ={2,1,-2};

	double sp = p1.scalar_product(p2);

	std::cout << p1.scalar_product(p2);
	fflush(stdout);

	p1 += p2;
	p2 *= 3;

	auto p3 = p1 + p2;


	std::cout << 1;
	return 0;
}