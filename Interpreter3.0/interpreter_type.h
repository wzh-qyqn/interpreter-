#pragma once
#ifndef INTERPRETER_TYPE_H__
#define INTERPRETER_TYPE_H__
#include <string>
#include <iostream>
#include <complex>
#include "interpreter_matrix.h"
namespace interpret {
	typedef long double Num_Type;
	typedef char Char_Type;
	typedef std::basic_string<Char_Type, \
		std::char_traits<Char_Type>, std::allocator<Char_Type> > Str_Type;
	typedef std::basic_ostream<Char_Type, std::char_traits<Char_Type> >  Ostr_Type;
	typedef std::complex<Num_Type> Complex_Type;
	typedef Matrix<Num_Type> Matrix_Type;
	typedef Matrix<Complex_Type> Cmatrix_Type;
}
#endif
