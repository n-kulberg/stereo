#ifndef __FieldObject_h
#define __FieldObject_h

//------------------------------------------------------------------
//
//	created:	2021/09/17	17:57
//	filename: 	FieldObject.h
//	file path:	q:\Projects\stereo\stereo
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------


#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

namespace kns_test
{

//! \brief scalar product operation for built-in arithmetic types
template<class T1, class T2>
typename std::enable_if<std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value, T1>::type	scalar_product(const T1& x, const T2& y) noexcept
{
	return x*y;
}

//! \brief scalar product operation for built-in arithmetic types
template<class T1>
typename std::enable_if<std::is_arithmetic<T1>::value, T1>::type	l1_norma(const T1& x) noexcept
{
	return abs(x);
}


//! \brief Field object is the element of field set (https://en.wikipedia.org/wiki/Field_(mathematics))
//! \detailed We suppose that the field is the synonym of the "linear vector space".
//! Elements of fields (objects belonging to it) can be:
//! 1. added and subtracted with each other;
//! 2. multiplied and divided by scalar.
//! The add/subtract/multiply result is also the element of the field.

template<class T, class CHILD_T, size_t N>
class FieldObject
{
public:
	static constexpr size_t n_dimensions() { return N; }
	using value_type = T;
	using child_type = CHILD_T;
	using self = FieldObject<T, CHILD_T, N>;

	using iterator = T*;
	using const_iterator = const T*;

	iterator begin() noexcept { return m_data; }
	iterator end() noexcept { return m_data + n_dimensions(); }
	const_iterator cbegin() const noexcept { return m_data; }
	const_iterator cend() const noexcept { return m_data + n_dimensions(); }

	operator child_type& () noexcept
	{ 
 		static_assert(sizeof(self) == sizeof(child_type), "FieldObject<T,CHILD_T,N>::operator child_type: Incompatible self and child types.");
		return *reinterpret_cast<child_type*>(this); 
	}
	operator const child_type& () const noexcept
	{
 		static_assert(sizeof(self) == sizeof(child_type), "FieldObject<T,CHILD_T,N>::operator child_type: Incompatible self and child types.");
		return *static_cast<child_type*>(this); 
	}

	child_type& child_ref() noexcept { return (*this); }
	const child_type& child_ref() const noexcept { return (*this); }


	T& at(size_t i){ return m_data[i]; }
	const T& at(size_t i) const { return m_data[i]; }

	T& operator[](size_t i){ return m_data[i]; }
	const T& operator[](size_t i) const { return m_data[i]; }


	// constructors
	//since the class is trivial, move constructor is not needed
	FieldObject() = default;

	template<class T2, class C2>
	FieldObject(const FieldObject<T2, C2, N>& other){ std::copy(other.cbegin(), other.cend(), begin()); }
	FieldObject(const self&) = default;

	template<class T2>
	FieldObject(const std::initializer_list<T2>& il)
	{
		if(il.size()!=n_dimensions()) throw std::invalid_argument("FieldObject::FieldObject(vector), invalid argument size");
		auto	d = begin();
		for(auto it = il.begin(), ie = il.end(); it!=ie; ++it, ++d) *d=static_cast<T>(*it);//sorry, std::copy doesn't work for initializer list
	}

	//operator=
	template<class T2, class C2>
	child_type& operator=(const FieldObject<T2, C2, N>& other)
	{
		std::copy(other.cbegin(), other.cend(), begin());
		return child_ref();
	}

	//template can not replace implicit operator=
	child_type& operator=(const self& other)
	{
		std::copy(other.cbegin(), other.cend(), begin());
		return child_ref();
	}

	// compare operators
	template<class T2, class C2>
	bool operator==(const FieldObject<T2, C2, N>& other) const noexcept
	{
		bool	result = true;
		auto	d = cbegin();
		for(auto it = other.cbegin(), ie = other.cend(); it!=ie; ++it, ++d) result &= (*d==*it);
		return result;
	}

	template<class T2, class C2> bool operator != (const FieldObject<T2, C2, N>& other) const noexcept { return !operator==(other); } 
	template<class T2, class C2> bool operator > (const FieldObject<T2, C2, N>& other) const noexcept { return l1_norma() > other.l1_norma(); }
	template<class T2, class C2> bool operator >= (const FieldObject<T2, C2, N>& other) const noexcept { return l1_norma() >= other.l1_norma(); }
	template<class T2, class C2> bool operator < (const FieldObject<T2, C2, N>& other) const noexcept { return l1_norma() < other.l1_norma(); }
	template<class T2, class C2> bool operator <= (const FieldObject<T2, C2, N>& other) const noexcept { return l1_norma() <= other.l1_norma(); }

	// linear algebra operations
	template<class T2, class C2> child_type& operator += (const FieldObject<T2, C2, N>& other) noexcept { modify_binary_action(*this, other, [](T& x, const T2& y){x += y;}); return child_ref(); }
	template<class T2, class C2> child_type& operator -= (const FieldObject<T2, C2, N>& other) noexcept { modify_binary_action(*this, other, [](T& x, const T2& y){x -= y;}); return child_ref(); }

	//NB no references in these functions argument, high risk of side effects: e.g. Point3D<double> p{2,3,4}; p/=p.x() will result in {1,3,4}
	template<class ST> child_type& operator *= (ST scalar) noexcept { modify_binary_action(*this, scalar, [](T& x, const ST& y){x *= y;}); return child_ref(); }
	template<class ST> child_type& operator /= (ST scalar) noexcept{ modify_binary_action(*this, scalar, [](T& x, const ST& y){x /= y;}); return child_ref(); }

	template<class T2, class C2> child_type operator + (const FieldObject<T2, C2, N>& other) const noexcept { return return_binary_action(*this, other, [](const T& x, const T2& y){return x + y;}); }
	template<class T2, class C2> child_type operator - (const FieldObject<T2, C2, N>& other) const noexcept { return return_binary_action(*this, other, [](const T& x, const T2& y){return x - y;}); }

	template<class ST> child_type operator * (ST scalar) const noexcept { return return_binary_action(*this, scalar, [](const T& x, const ST& y){return x * y;}); }
	template<class ST> child_type operator / (ST scalar) const noexcept { return return_binary_action(*this, scalar, [](const T& x, const ST& y){return x / y;}); }

	child_type operator - () const noexcept { return return_binary_action(*this, *this, [](const T& x, const T){return -x;}); }

	//! \brief scalar product by another object of similar field
	template<class T2, class C2> value_type scalar_product(const FieldObject<T2, C2, N>& other) const noexcept { return acquire_binary_action(*this, other, [](const T& x, const T2& y){return kns_test::scalar_product(x, y);}); }

	//! \brief Lebesgue norma of the field object
	double	l2_norma() const noexcept { return sqrt(static_cast<double>(scalar_product(*this))); }

	//! \brief L1 norma of the field object. It is a bit faster than L2
	value_type l1_norma () const noexcept { return acquire_binary_action(*this, *this, [](const T& x, const T&){return kns_test::l1_norma(x);}); }

private:
	T	m_data[n_dimensions()];
protected:

	//! \brief Apply operation F to every pair of elements in first and second argument. F modifies first argument
	template<class T2, class C2, class F>
	void	modify_binary_action(self& first, const FieldObject<T2, C2, N>& second, F f) noexcept 
	{
		for(size_t i = 0; i < n_dimensions(); ++i) f(first.at(i), second.at(i));
	}

	//! \brief Apply operation F to every element in first. Second argument is the same in the cycle. F modifies first argument
	template<class ST, class F>
	void	modify_binary_action(self& first, ST second, F f) noexcept 
	{
		for(size_t i = 0; i < n_dimensions(); ++i){f(first.at(i), second);}
	}

	//! \brief Apply operation F to every pair of elements in first and second argument. Both arguments are constant, result is returned
	template<class T2, class C2, class F>
	child_type	return_binary_action(const self& first, const FieldObject<T2, C2, N>& second, F f) const noexcept 
	{
		self result;
		for(size_t i = 0; i < n_dimensions(); ++i) result.at(i) = f(first.at(i), second.at(i));
		return std::move(result.child_ref());
	}

	//! \brief Apply operation F to every element in first. Second argument is the same in the cycle. Both arguments are constant, result is returned
	template<class ST, class F>
	child_type	return_binary_action(const self& first, ST second, F f) const noexcept 
	{
		self result;
		for(size_t i = 0; i < n_dimensions(); ++i) {result.at(i) = f(first.at(i), second);}
		return std::move(result.child_ref());
	}

	template<class T2, class C2, class F>
	value_type	acquire_binary_action(const self& first, const FieldObject<T2, C2, N>& second, F f) const noexcept 
	{
		value_type result(0);
		for(size_t i = 0; i < n_dimensions(); ++i) result += f(first.at(i), second.at(i));
		return result;
	}

};

//! \brief Check if the object belongs to some field
template<class T> class is_field_object : public std::false_type {};
template<class T, class CT, size_t N> class is_field_object <FieldObject<T,CT,N>> : public std::true_type {};

//! \brief Scalar product of similar field objects, external function
template<class T1, class T2, class CT1, class CT2, size_t N>
T1	scalar_product(const FieldObject<T1,CT1,N>& x, const FieldObject<T2,CT2,N>& y) noexcept 
{
	return x.scalar_product(y);
}

//! \brief Shorter alias for scalar product
template<class T1, class T2, class CT1, class CT2, size_t N>
T1 sp(const FieldObject<T1, CT1, N>& x, const FieldObject<T2, CT2, N>& y) noexcept 
{
	return scalar_product(x,y);
}

//! \brief L1 norma of field object, non-member function
template<class T1, class CT1, size_t N>
T1	l1_norma(const FieldObject<T1, CT1, N>& x) noexcept 
{
	return x.l1_norma();
}

//! \brief L2 norma of field object, non-member function
template<class T1, class CT1, size_t N>
T1	l2_norma(const FieldObject<T1, CT1, N>& x) noexcept
{
	return x.l2_norma();
}


}//namespace kns_test

#endif //__FieldObject_h
