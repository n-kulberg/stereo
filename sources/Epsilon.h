#ifndef __Epsilon_h
#define __Epsilon_h

//------------------------------------------------------------------
//
//	created:	2021/09/21	11:02
//	filename: 	Epsilon.h
//	file path:	Q:\Projects\stereo\sources
//	author:		kns
//	
//	purpose:	
//
//------------------------------------------------------------------

#include <cmath>

//! \file Functions determined in this file allow exclude formally nonzero, but negligibly small values
//! In addition to std::numeric_limits<T>::epsilon() I add here the controlled scalefators. 
//! But now these factors are almost random

namespace kns_test
{


//! \brief Absolute computation error allowed (factor to std::numeric_limits<T>::denorm_min())
constexpr double	absolute_error_factor(){ return 1000; }

//! \brief Relative computation error allowed (factor to std::numeric_limits<T>::epsilon()
constexpr double	relative_error_factor(){ return 100; }



//! \brief Absolute computation error allowed. Determine if the value x is negligibly small
template<class T>
inline bool is_almost_zero(T x) { return std::abs(x) <= absolute_error_factor()*std::numeric_limits<T>::denorm_min(); } // strict < is incorrect, <= is needed

//! \brief Determine if value x is negligible to y. Notation inspired by https://en.wikipedia.org/wiki/Big_O_notation
template<class T>
inline bool is_small_o(T x, T y){ return std::abs(x) <= std::abs(y)* relative_error_factor()*std::numeric_limits<T>::epsilon(); } // strict < is incorrect, <= is needed

}//namespace kns_test

#endif //__Epsilon_h
