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

	operator child_type& (){ return *static_cast<child_type*>(this); }
	operator const child_type& () const { return *static_cast<child_type*>(this); }

	child_type& child_ref(){return (*this); }
	const child_type& child_ref() const { return (*this); }


	T& at(size_t i){ return m_data[i]; }
	const T& at(size_t i) const { return m_data[i]; }

	const auto data() const{ return m_data; }

	// constructors
	FieldObject() = default;
	FieldObject(const self &) = default;
	//since the class is trivial, move constructor is not needed

	template<class T2>
	FieldObject(const std::vector<T2>& il)
	{
		if(il.size()!=n_dimensions) throw std::invalid_argument("FieldObject::FieldObject(vector), invalid argument size");
		std::copy(il.begin(), il.end(), begin());
	}


		// linear algebra operations
	template<class T2, class C2> child_type& operator += (const FieldObject<T2,C2,N>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x += y;}); return child_ref(); }
	template<class T2, class C2> child_type& operator -= (const FieldObject<T2,C2,N>& other){ modify_binary_action(*this, other, [](T& x, const T2& y){x -= y;}); return *this; }
	template<class T2> child_type& operator *= (const T2& scalar){ modify_binary_action(*this, scalar, [](T& x, const T2& y){x *= y;}); return *this; }
	template<class T2> child_type& operator /= (const T2& scalar){ modify_binary_action(*this, scalar, [](T& x, const T2& y){x /= y;}); return *this; }

	template<class T2, class C2> child_type operator + (const FieldObject<T2,C2,N>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x + y;}); }
	template<class T2, class C2> child_type operator - (const FieldObject<T2,C2,N>& other) const { return return_binary_action(*this, other, [](const T& x, const T2& y){return x - y;}); }
	template<class T2> child_type operator * (const T2& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const T2& y){return x * y;}); }
	template<class T2> child_type operator / (const T2& scalar) const { return return_binary_action(*this, scalar, [](const T& x, const T2& y){return x / y;}); }

	template<class T2, class C2> value_type scalar_product(const FieldObject<T2,C2,N>& other) const { return acquire_binary_action(*this, other, [](const T& x, const T2& y){return x * y;}); }
	double	l2_norma() const{ return sqrt(double(scalar_product(*this))); }


private:
	T	m_data[n_dimensions()];

		// data operation algorithms
	template<class T2, class C2, class F>
	void	modify_binary_action(self& p1, const FieldObject<T2,C2,N>& p2, const F& f)
	{
		for(size_t i = 0; i < n_dimensions(); ++i) f(p1.m_data[i], *(p2.data() + i));
	}
	template<class T2, class C2, class F>
	void	modify_binary_action(self& p1, const T2& scalar, const F& f)
	{
		for(size_t i = 0; i < n_dimensions(); ++i) f(p1.m_data[i], scalar);
	}
	template<class T2, class C2, class F>
	child_type	return_binary_action(const self& p1, const FieldObject<T2,C2,N>& p2, const F& f) const
	{
		child_type result;
		for(size_t i = 0; i < n_dimensions(); ++i) result.m_data[i] = f(p1.m_data[i], *(p2.data() + i));
		return result;
	}
	template<class T2, class F>
	child_type	return_binary_action(const self& p1, const T2& scalar, const F& f) const
	{
		child_type result;
		for(size_t i = 0; i < n_dimensions(); ++i) result.m_data[i] = f(p1.m_data[i], scalar);
		return result;
	}

	template<class T2, class C2, class F>
	value_type	acquire_binary_action(const self& p1, const FieldObject<T2,C2,N>& p2, const F& f) const
	{
		value_type result(0);
		for(size_t i = 0; i < n_dimensions(); ++i) result += f(p1.data()[i], p2.data()[i]);
		return result;
	}

};

template<class T, class CHILD_T, size_t N>
std::ostream& operator << (std::ostream& str, const FieldObject<T, CHILD_T, N> &f)
{
	str << "{";
	std::for_each(f.cbegin(), f.cend(), [&str](const T& x){str << x << ";";});
	str << "}";
	return str;
}



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
	Point3D(const T& in_z, const T& in_y, const T& in_x){ x()=in_x; y() = in_y; z() = in_z; }
	Point3D(T& in_z, const T& in_y, const T& in_x){ x()=in_x; y = in_y; z() = in_z; }


	using parent::parent;
	using parent::at;
	using parent::data;
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



protected:
private:
	const size_t m_z = 0, m_y = 1, m_x = 2;
};

template<class T>
class Segment3D : public FieldObject<Point3D<T>, Segment3D<Point3D<T>>, 2>
{
public:
	using parent = FieldObject<Point3D<T>, Segment3D<Point3D<T>>, 2>;
	using parent::parent;
private:
};
}//namespace kns_test

}//namespace xrad
using namespace kns_test;

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