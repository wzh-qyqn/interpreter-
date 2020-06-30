/********************************************************************************
* @File name: interpreter_math.cpp
* @Author: wzh
* @Version: 3.1
* @Date: 2020-5-12
* @Description: 定义支持Base_Data数据类型的数学函数
********************************************************************************/
#include "interpreter.h"


using namespace inter;
namespace {
typedef inter::Interpreter::Base_Data Use_Data;
static inline Cmatrix_Type Num_To_Complex(const Matrix_Type& a) {
	Complex_Type* pbuf;
	size_t row_num = a.row();
	size_t col_num = a.col();
	pbuf = new Complex_Type[row_num*col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pbuf[i] = a.loc(i, j);
		}
	}
	return Cmatrix_Type(row_num, col_num, pbuf, false);
}
//定义所有支持类型的加法
static inline Num_Type	raddition(const Num_Type& a, const Num_Type& b) {
	return a + b;
}
static inline Complex_Type	raddition(const Num_Type& a, const Complex_Type& b) {
	return a + b;
}
static inline Num_Type	raddition(const Num_Type& a, const bool& b) {
	return a + b;
}
static inline Complex_Type	raddition(const Complex_Type& a, const Num_Type& b) {
	return a + b;
}
static inline Complex_Type	raddition(const Complex_Type& a, const Complex_Type& b) {
	return a + b;
}
static inline Complex_Type	raddition(const Complex_Type& a, const bool& b) {
	return a + Num_Type(b);
}
static inline Num_Type	raddition(const bool& a, const Num_Type& b) {
	return a + b;
}
static inline Complex_Type	raddition(const bool& a, const Complex_Type& b) {
	return Num_Type(a) + b;
}
static inline Num_Type	raddition(const bool& a, const bool& b) {
	return a + b;
}
static inline Matrix_Type raddition(const Matrix_Type* a, const Matrix_Type* b) {
	return *a + *b;
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Matrix_Type* b) {
	return *a + Num_To_Complex(*b);
}
static inline Cmatrix_Type raddition(const Matrix_Type* a, const Cmatrix_Type* b) {
	return *b + Num_To_Complex(*a);
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	return *a + *b;
}
static inline Matrix_Type raddition(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Matrix_Type raddition(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
static inline Matrix_Type raddition(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Matrix_Type raddition(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
static inline Matrix_Type raddition(const Matrix_Type* a, const bool& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Matrix_Type raddition(const bool& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const bool& b) {
	throw show_err("矩阵与数无法相加");
}
static inline Cmatrix_Type raddition(const bool& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相加");
}
//定义所有支持类型的减法
static inline Num_Type	rsubstration(const Num_Type& a, const Num_Type& b) {
	return a - b;
}
static inline Complex_Type	rsubstration(const Num_Type& a, const Complex_Type& b) {
	return a - b;
}
static inline Num_Type	rsubstration(const Num_Type& a, const bool& b) {
	return a - b;
}
static inline Complex_Type	rsubstration(const Complex_Type& a, const Num_Type& b) {
	return a - b;
}
static inline Complex_Type	rsubstration(const Complex_Type& a, const Complex_Type& b) {
	return a - b;
}
static inline Complex_Type	rsubstration(const Complex_Type& a, const bool& b) {
	return a - Num_Type(b);
}
static inline Num_Type	rsubstration(const bool& a, const Num_Type& b) {
	return a - b;
}
static inline Complex_Type	rsubstration(const bool& a, const Complex_Type& b) {
	return Num_Type(a) - b;
}
static inline Num_Type	rsubstration(const bool& a, const bool& b) {
	return a - b;
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const Matrix_Type* b) {
	return *a - *b;
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Matrix_Type* b) {
	return *a - Num_To_Complex(*b);
}
static inline Cmatrix_Type rsubstration(const Matrix_Type* a, const Cmatrix_Type* b) {
	return *b - Num_To_Complex(*a);
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	return *a - *b;
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Matrix_Type rsubstration(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Matrix_Type rsubstration(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const bool& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Matrix_Type rsubstration(const bool& a, const Matrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const bool& b) {
	throw show_err("矩阵与数无法相减");
}
static inline Cmatrix_Type rsubstration(const bool& a, const Cmatrix_Type* b) {
	throw show_err("矩阵与数无法相减");
}
//定义所有支持类型的乘法
static inline Num_Type	rmultiply(const Num_Type& a, const Num_Type& b) {
	return a * b;
}
static inline Complex_Type	rmultiply(const Num_Type& a, const Complex_Type& b) {
	return a * b;
}
static inline Num_Type	rmultiply(const Num_Type& a, const bool& b) {
	return a * b;
}
static inline Complex_Type	rmultiply(const Complex_Type& a, const Num_Type& b) {
	return a * b;
}
static inline Complex_Type	rmultiply(const Complex_Type& a, const Complex_Type& b) {
	return a * b;
}
static inline Complex_Type	rmultiply(const Complex_Type& a, const bool& b) {
	return a * Num_Type(b);
}
static inline Num_Type	rmultiply(const bool& a, const Num_Type& b) {
	return a * b;
}
static inline Complex_Type	rmultiply(const bool& a, const Complex_Type& b) {
	return Num_Type(a) * b;
}
static inline Num_Type	rmultiply(const bool& a, const bool& b) {
	return a * b;
}
static inline Matrix_Type rmultiply(const Matrix_Type* a, const Matrix_Type* b) {
	return *a * *b;
}
static inline Cmatrix_Type rmultiply(const Cmatrix_Type* a, const Matrix_Type* b) {
	return *a * Num_To_Complex(*b);
}
static inline Cmatrix_Type rmultiply(const Matrix_Type* a, const Cmatrix_Type* b) {
	return *b * Num_To_Complex(*a);
}
static inline Cmatrix_Type rmultiply(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	return *a * *b;
}
static inline Matrix_Type rmultiply(const Matrix_Type* a, const Num_Type& b) {
	return *a * b;
}
static inline Matrix_Type rmultiply(const Num_Type& a, const Matrix_Type* b) {
	return a * *b;
}
static inline Cmatrix_Type rmultiply(const Matrix_Type* a, const Complex_Type& b) {
	return b * Num_To_Complex(*a);
}
static inline Cmatrix_Type rmultiply(const Complex_Type& a, const Matrix_Type* b) {
	return a * Num_To_Complex(*b);
}
static inline Matrix_Type rmultiply(const Matrix_Type* a, const bool& b) {
	return *a * b;
}
static inline Matrix_Type rmultiply(const bool& a, const Matrix_Type* b) {
	return a * *b;
}
static inline Cmatrix_Type rmultiply(const Cmatrix_Type* a, const Num_Type& b) {
	return *a * b;
}
static inline Cmatrix_Type rmultiply(const Num_Type& a, const Cmatrix_Type* b) {
	return a * *b;
}
static inline Cmatrix_Type rmultiply(const Cmatrix_Type* a, const Complex_Type& b) {
	return *a * b;
}
static inline Cmatrix_Type rmultiply(const Complex_Type& a, const Cmatrix_Type* b) {
	return a * *b;
}
static inline Cmatrix_Type rmultiply(const Cmatrix_Type* a, const bool& b) {
	return *a * b;
}
static inline Cmatrix_Type rmultiply(const bool& a, const Cmatrix_Type* b) {
	return a * *b;
}
//定义所有支持类型的除法
static inline Num_Type	rdivision(const Num_Type& a, const Num_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Num_Type& a, const Complex_Type& b) {
	return a / b;
}
static inline Num_Type	rdivision(const Num_Type& a, const bool& b) {
	throw show_err("请不要将逻辑类型作为除数，因为这很大可能会出现除零错误");
}
static inline Complex_Type	rdivision(const Complex_Type& a, const Num_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Complex_Type& a, const Complex_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Complex_Type& a, const bool& b) {
	throw show_err("请不要将逻辑类型作为除数，因为这很大可能会出现除零错误");
}
static inline Num_Type	rdivision(const bool& a, const Num_Type& b) {
	return Num_Type(a) /b;
}
static inline Complex_Type	rdivision(const bool& a, const Complex_Type& b) {
	return Num_Type(a) / b;
}
static inline Num_Type	rdivision(const bool& a, const bool& b) {
	throw show_err("请不要将逻辑类型作为除数，因为这很大可能会出现除零错误");
}
static inline Matrix_Type rdivision(const Matrix_Type* a, const Matrix_Type* b) {
	return *a / *b;
}
static inline Cmatrix_Type rdivision(const Cmatrix_Type* a, const Matrix_Type* b) {
	return *a / Num_To_Complex(*b);
}
static inline Cmatrix_Type rdivision(const Matrix_Type* a, const Cmatrix_Type* b) {
	return *b / Num_To_Complex(*a);
}
static inline Cmatrix_Type rdivision(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	return *a / *b;
}
static inline Matrix_Type rdivision(const Matrix_Type* a, const Num_Type& b) {
	return *a / b;
}
static inline Matrix_Type rdivision(const Num_Type& a, const Matrix_Type* b) {
	return a / *b;
}
static inline Cmatrix_Type rdivision(const Matrix_Type* a, const Complex_Type& b) {
	return b / Num_To_Complex(*a);
}
static inline Cmatrix_Type rdivision(const Complex_Type& a, const Matrix_Type* b) {
	return a / Num_To_Complex(*b);
}
static inline Matrix_Type rdivision(const Matrix_Type* a, const bool& b) {
	return *a / b;
}
static inline Matrix_Type rdivision(const bool& a, const Matrix_Type* b) {
	return a / *b;
}
static inline Cmatrix_Type rdivision(const Cmatrix_Type* a, const Num_Type& b) {
	return *a / b;
}
static inline Cmatrix_Type rdivision(const Num_Type& a, const Cmatrix_Type* b) {
	return a / *b;
}
static inline Cmatrix_Type rdivision(const Cmatrix_Type* a, const Complex_Type& b) {
	return *a / b;
}
static inline Cmatrix_Type rdivision(const Complex_Type& a, const Cmatrix_Type* b) {
	return a / *b;
}
static inline Cmatrix_Type rdivision(const Cmatrix_Type* a, const bool& b) {
	return *a / b;
}
static inline Cmatrix_Type rdivision(const bool& a, const Cmatrix_Type* b) {
	return a / *b;
}
//定义所有支持类型的乘方
static inline Use_Data		rpow(const Num_Type& a, const Num_Type& b) {
	Num_Type&& res = pow(a, b);
	if (isnan(res))
		return Use_Data(pow(Complex_Type(a), Complex_Type(b)));
	else
		return Use_Data(res);
}
static inline Complex_Type	rpow(const Num_Type& a, const Complex_Type& b) {
	return pow(a,b);
}
static inline Num_Type	rpow(const Num_Type& a, const bool& b) {
	return pow(a,b);
}
static inline Complex_Type	rpow(const Complex_Type& a, const Num_Type& b) {
	return pow(a, b);
}
static inline Complex_Type	rpow(const Complex_Type& a, const Complex_Type& b) {
	return pow(a, b);
}
static inline Complex_Type	rpow(const Complex_Type& a, const bool& b) {
	return pow(a, b);
}
static inline Num_Type	rpow(const bool& a, const Num_Type& b) {
	return pow(a, b);
}
static inline Complex_Type	rpow(const bool& a, const Complex_Type& b) {
	return pow(a, b);
}
static inline Num_Type	rpow(const bool& a, const bool& b) {
	return pow(a, b);
}
static inline Matrix_Type rpow(const Matrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Matrix_Type rpow(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Matrix_Type rpow(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Matrix_Type rpow(const Matrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Matrix_Type rpow(const bool& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵幂运算");
}
static inline Cmatrix_Type rpow(const bool& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵幂运算");
}
//定义所有支持类型的求余
static inline Num_Type	rmod(const Num_Type& a, const Num_Type& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const Num_Type& a, const Complex_Type& b) {
	throw show_err("未定义复数取余");
}
static inline Num_Type	rmod(const Num_Type& a, const bool& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const Complex_Type& a, const Num_Type& b) {
	return Complex_Type(fmod(a.real(), b), fmod(a.imag(), b));
}
static inline Complex_Type	rmod(const Complex_Type& a, const Complex_Type& b) {
	throw show_err("未定义复数取余");
}
static inline Complex_Type	rmod(const Complex_Type& a, const bool& b) {
	throw show_err("未定义复数取余");
}
static inline Num_Type	rmod(const bool& a, const Num_Type& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const bool& a, const Complex_Type& b) {
	throw show_err("未定义复数取余");
}
static inline Num_Type	rmod(const bool& a, const bool& b) {
	return fmod(a, b);
}
static inline Matrix_Type rmod(const Matrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Matrix_Type rmod(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Matrix_Type rmod(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Matrix_Type rmod(const Matrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Matrix_Type rmod(const bool& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵求余运算");
}
static inline Cmatrix_Type rmod(const bool& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵求余运算");
}
//比大小
static inline bool	rgreater(const Num_Type& a, const Num_Type& b) {
	return a > b;
}
static inline bool	rgreater(const Num_Type& a, const Complex_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rgreater(const Num_Type& a, const bool& b) {
	return a > Num_Type(b);
}
static inline bool	rgreater(const Complex_Type& a, const Num_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rgreater(const Complex_Type& a, const Complex_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rgreater(const Complex_Type& a, const bool& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rgreater(const bool& a, const Num_Type& b) {
	return Num_Type(a) > b ;
}
static inline bool	rgreater(const bool& a, const Complex_Type& b) {
	throw show_err("未定义复数取余");
}
static inline bool	rgreater(const bool& a, const bool& b) {
	return a > b;
}
static inline Matrix_Type rgreater(const Matrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rgreater(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rgreater(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rgreater(const Matrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rgreater(const bool& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const Cmatrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rgreater(const bool& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline bool	rless(const Num_Type& a, const Num_Type& b) {
	return a < b;
}
static inline bool	rless(const Num_Type& a, const Complex_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rless(const Num_Type& a, const bool& b) {
	return a < Num_Type(b);
}
static inline bool	rless(const Complex_Type& a, const Num_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rless(const Complex_Type& a, const Complex_Type& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rless(const Complex_Type& a, const bool& b) {
	throw show_err("复数比大小无意义！");
}
static inline bool	rless(const bool& a, const Num_Type& b) {
	return Num_Type(a) < b;
}
static inline bool	rless(const bool& a, const Complex_Type& b) {
	throw show_err("未定义复数取余");
}
static inline bool	rless(const bool& a, const bool& b) {
	return a < b;
}
static inline Matrix_Type rless(const Matrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rless(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rless(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rless(const Matrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Matrix_Type rless(const bool& a, const Matrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const Cmatrix_Type* a, const bool& b) {
	throw show_err("暂不支持矩阵比较运算");
}
static inline Cmatrix_Type rless(const bool& a, const Cmatrix_Type* b) {
	throw show_err("暂不支持矩阵比较运算");
}
Num_Type inline imag(const Num_Type& a) {
    return 0;
}
Num_Type inline real(const Num_Type& a) {
    return a;
}
Num_Type inline imag(const Complex_Type& a) {
    return a.imag();
}
Num_Type inline real(const Complex_Type& a) {
    return a.real();
}
static inline Use_Data urtgamma(const Use_Data&a);
static inline Interpreter::_Data_Array rtgamma(const Interpreter::_Data_Array* a) {
	Interpreter::_Data_Array pvec;
	for (size_t i = 0; i < a->size(); i++) {
		pvec.push_back(urtgamma((*a)[i]));
	}
	return pvec;
}
static inline Matrix_Type rtgamma(const Matrix_Type* a1) {
	Num_Type* pbuf;
	size_t row_num = a1->row();
	size_t col_num = a1->col();
	pbuf = new Num_Type[row_num*col_num];
	for (size_t i = 0; i<row_num; i++) {
		for (size_t j = 0; j<col_num; j++) {
			pbuf[i*col_num + j] = tgamma(a1->loc(i, j));
		}
	}
	return Matrix_Type(row_num, col_num, pbuf, false);
}
static inline Cmatrix_Type rtgamma(const Cmatrix_Type* a1){
	throw show_err("不支持复数的阶乘");
}
static inline Num_Type rtgamma(const Num_Type& a) {
	return tgamma(a);
}
static inline Num_Type rtgamma(const bool& a) {
	return tgamma(Num_Type(a));
}
static inline Complex_Type rtgamma(const Complex_Type& a) {
	throw show_err("不支持复数的阶乘");
}

#define SINGLEVAR_FUNCTION_DECLARE(name) Use_Data u##name##(const Use_Data& a) {\
                                        switch (a.get_type()) {\
                                        case Interpreter::DATA_DOUBLE: {\
                                            Num_Type a1;\
                                            a.get_data(a1);\
                                            return Use_Data(name##(a1));\
                                        }\
                                        case Interpreter::DATA_COMPLEX: {\
                                            Complex_Type a1;\
                                            a.get_data(a1);\
                                            return Use_Data(name##(a1));\
                                        }\
										case Interpreter::DATA_BOOL:{\
											bool a1;\
											a.get_data(a1);\
											return Use_Data(name##(a1));\
										}\
										case Interpreter::DATA_ARRAY: {\
											Interpreter::_Data_Array* a1;\
											a.get_data(a1);\
											return Use_Data(name##(a1));\
										}\
										case Interpreter::DATA_MATRIX:{\
											Matrix_Type* a1;\
											a.get_data(a1);\
											return Use_Data(name##(a1));\
										}\
										case Interpreter::DATA_CMATRIX:{\
											Cmatrix_Type* a1;\
											a.get_data(a1);\
											return Use_Data(name##(a1));\
										}\
                                        default:\
                                            throw show_err("函数不支持其类型");\
                                        }\
                                        }
#define BINARY_FUNCTION_DECLARE(name)	Use_Data u##name##(const Use_Data& a, const Use_Data& b){\
												switch(a.get_type()){\
												case Interpreter::DATA_DOUBLE:{\
													Num_Type a1;\
													a.get_data(a1);\
													switch (b.get_type()) {\
													case Interpreter::DATA_DOUBLE: {\
														Num_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_COMPLEX: {\
														Complex_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_BOOL:{\
														bool b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_MATRIX:{\
														Matrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_CMATRIX:{\
														Cmatrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_ARRAY: {\
														Interpreter::_Data_Array *b1;\
														Interpreter::_Data_Array pvec;\
														b.get_data(b1);\
														for (int i = 0; i < b1->size(); i++) {\
															pvec.push_back(u##name##(a, (*b1)[i]));\
														}\
														return Use_Data(pvec);\
														}\
													default:\
														throw show_err("此函数不支持此类型，错误类型：",b.get_type());\
													}\
												}\
												case Interpreter::DATA_COMPLEX:{\
													Complex_Type a1;\
													a.get_data(a1);\
													switch (b.get_type()) {\
													case Interpreter::DATA_DOUBLE: {\
														Num_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_COMPLEX: {\
														Complex_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_BOOL:{\
														bool b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_MATRIX:{\
														Matrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_CMATRIX:{\
														Cmatrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_ARRAY: {\
														Interpreter::_Data_Array *b1;\
														Interpreter::_Data_Array pvec;\
														b.get_data(b1);\
														for (int i = 0; i < b1->size(); i++) {\
															pvec.push_back(u##name##(a, (*b1)[i]));\
														}\
														return Use_Data(pvec);\
														}\
													default:\
														throw show_err("此函数不支持此类型，错误类型：",b.get_type());\
													}\
												}\
												case Interpreter::DATA_BOOL:{\
													bool a1;\
													a.get_data(a1);\
													switch (b.get_type()) {\
													case Interpreter::DATA_DOUBLE: {\
														Num_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_COMPLEX: {\
														Complex_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_BOOL:{\
														bool b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_MATRIX:{\
														Matrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_CMATRIX:{\
														Cmatrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_ARRAY: {\
														Interpreter::_Data_Array *b1;\
														Interpreter::_Data_Array pvec;\
														b.get_data(b1);\
														for (int i = 0; i < b1->size(); i++) {\
															pvec.push_back(u##name##(a, (*b1)[i]));\
														}\
														return Use_Data(pvec);\
														}\
													default:\
														throw show_err("此函数不支持此类型，错误类型：",b.get_type());\
													}\
												}\
												case Interpreter::DATA_MATRIX:{\
													Matrix_Type* a1;\
													a.get_data(a1);\
													switch (b.get_type()) {\
													case Interpreter::DATA_DOUBLE: {\
														Num_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_COMPLEX: {\
														Complex_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_BOOL:{\
														bool b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_MATRIX:{\
														Matrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_CMATRIX:{\
														Cmatrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_ARRAY: {\
														Interpreter::_Data_Array *b1;\
														Interpreter::_Data_Array pvec;\
														b.get_data(b1);\
														for (int i = 0; i < b1->size(); i++) {\
															pvec.push_back(u##name##(a, (*b1)[i]));\
														}\
														return Use_Data(pvec);\
														}\
													default:\
														throw show_err("此函数不支持此类型，错误类型：",b.get_type());\
													}\
												}\
												case Interpreter::DATA_CMATRIX:{\
													Cmatrix_Type* a1;\
													a.get_data(a1);\
													switch (b.get_type()) {\
													case Interpreter::DATA_DOUBLE: {\
														Num_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_COMPLEX: {\
														Complex_Type b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1,b1));\
														}\
													case Interpreter::DATA_BOOL:{\
														bool b1;\
														b.get_data(b1);\
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_MATRIX:{\
														Matrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_CMATRIX:{\
														Cmatrix_Type* b1;\
														b.get_data(b1); \
														return Use_Data(name##(a1, b1)); \
														}\
													case Interpreter::DATA_ARRAY: {\
														Interpreter::_Data_Array *b1;\
														Interpreter::_Data_Array pvec;\
														b.get_data(b1);\
														for (int i = 0; i < b1->size(); i++) {\
															pvec.push_back(u##name##(a, (*b1)[i]));\
														}\
														return Use_Data(pvec);\
														}\
													default:\
														throw show_err("此函数不支持此类型，错误类型：",b.get_type());\
													}\
												}\
												case Interpreter::DATA_ARRAY: {\
													Interpreter::_Data_Array *a1;\
													Interpreter::_Data_Array pvec;\
													a.get_data(a1);\
													if (b.get_type() == Interpreter::DATA_ARRAY) {\
														Interpreter::_Data_Array *b1;\
														b.get_data(b1);\
														if (a1->size() != b1->size())\
															throw show_err("数组运算维数不匹配");\
														for (int i = 0; i < a1->size(); i++) {\
															pvec.push_back(u##name##((*a1)[i], (*b1)[i]));\
														}\
														return Use_Data(pvec);\
													}\
													else {\
														for (int i = 0; i < a1->size(); i++) {\
															pvec.push_back(u##name##((*a1)[i], b));\
														}\
														return Use_Data(pvec);\
													}\
													}\
												default:\
														throw show_err("此函数不支持此类型，错误类型：",a.get_type());\
											}\
											}
 

#define SINGLEVAR_FUNC_NEWDECLARE(name)		Use_Data ur##name##(const Use_Data& a);\
											static inline Interpreter::_Data_Array r##name##(const Interpreter::_Data_Array* a){\
												Interpreter::_Data_Array pvec;\
												for (size_t i = 0; i < a->size(); i++) {\
													pvec.push_back(ur##name##((*a)[i]));\
												}\
												return pvec;\
											}\
											static inline Matrix_Type r##name##(const Matrix_Type* a1){\
												Num_Type* pbuf;\
												size_t row_num = a1->row();\
												size_t col_num = a1->col();\
												pbuf = new Num_Type[row_num*col_num];\
												for (size_t i = 0; i<row_num; i++) {\
													for (size_t j = 0; j<col_num; j++) {\
														pbuf[i*col_num + j] = name##(a1->loc(i, j));\
													}\
												}\
												return Matrix_Type(row_num, col_num, pbuf, false);\
											}\
											static inline Cmatrix_Type r##name##(const Cmatrix_Type* a1){\
												Complex_Type* pbuf;\
												size_t row_num = a1->row();\
												size_t col_num = a1->col();\
												pbuf = new Complex_Type[row_num*col_num];\
												for (size_t i = 0; i<row_num; i++) {\
													for (size_t j = 0; j<col_num; j++) {\
														pbuf[i*col_num + j] = name##(a1->loc(i, j));\
													}\
												}\
												return Cmatrix_Type(row_num, col_num, pbuf, false);\
											}\
											static inline Num_Type r##name##(const Num_Type&a){\
												return name##(a);\
											}\
											static inline Num_Type r##name##(const bool&a){\
												return name##(Num_Type(a));\
											}\
											static inline Complex_Type r##name##(const Complex_Type&a){\
												return name##(a);\
											}\
											SINGLEVAR_FUNCTION_DECLARE(r##name)

SINGLEVAR_FUNC_NEWDECLARE(sin)
SINGLEVAR_FUNC_NEWDECLARE(cos)
SINGLEVAR_FUNC_NEWDECLARE(tan)
SINGLEVAR_FUNC_NEWDECLARE(acos)
SINGLEVAR_FUNC_NEWDECLARE(asin)
SINGLEVAR_FUNC_NEWDECLARE(atan)
SINGLEVAR_FUNC_NEWDECLARE(abs)
SINGLEVAR_FUNC_NEWDECLARE(log10)
SINGLEVAR_FUNC_NEWDECLARE(log)
SINGLEVAR_FUNC_NEWDECLARE(sinh)
SINGLEVAR_FUNC_NEWDECLARE(cosh)
SINGLEVAR_FUNC_NEWDECLARE(tanh)
SINGLEVAR_FUNC_NEWDECLARE(asinh)
SINGLEVAR_FUNC_NEWDECLARE(acosh)
SINGLEVAR_FUNC_NEWDECLARE(atanh)
SINGLEVAR_FUNC_NEWDECLARE(exp)
SINGLEVAR_FUNC_NEWDECLARE(imag)
SINGLEVAR_FUNC_NEWDECLARE(real)
BINARY_FUNCTION_DECLARE(raddition)
BINARY_FUNCTION_DECLARE(rsubstration)
BINARY_FUNCTION_DECLARE(rmultiply)
BINARY_FUNCTION_DECLARE(rdivision)
BINARY_FUNCTION_DECLARE(rmod)
BINARY_FUNCTION_DECLARE(rpow)
BINARY_FUNCTION_DECLARE(rless)
BINARY_FUNCTION_DECLARE(rgreater)
SINGLEVAR_FUNCTION_DECLARE(rtgamma)

}

const std::array<Interpreter::Binary_Func, Interpreter::binary_func_num>Interpreter::binary_func = {
    '+',4,uraddition,
    '-',4,ursubstration,
    '*',5,urmultiply,
    '/',5,urdivision,
    '%',5,urmod,
    '^',6,urpow,
    '>',3,urgreater,
    '<',3,urless,
};
const std::array<Interpreter::SingleVar_Func, Interpreter::singlevar_func_num>Interpreter::singlevar_func = {
    "sin"	,ursin,
    "cos"	,urcos,
    "tan"	,urtan,
    "acos"	,uracos,
    "asin"	,urasin,
    "atan"	,uratan,
    "abs"	,urabs,
    "log"	,urlog10,
    "ln"	,urlog,
    "sinh"	,ursinh,
    "cosh"	,urcosh,
    "tanh"	,urtanh,
    "asinh"	,urasinh,
    "acosh"	,uracosh,
    "atanh"	,uratanh,
    "exp"	,urexp,
    "real"	,urreal,
    "imag"	,urimag
};
const std::array<Interpreter::Unary_Func, \
    Interpreter::unary_func_num>Interpreter::unary_func = {
    '!',8,urtgamma,
};
const std::array<Interpreter::Const_Num, \
    Interpreter::const_BaseData_num>Interpreter::const_BaseData = {
    "pi",Use_Data(Num_Type(3.14159265358979323846)),
    "e",Use_Data(Num_Type(2.71828182845904523536)),
    "i",Use_Data(Complex_Type(0,1)),
    "j",Use_Data(Complex_Type(0,1)),
    "true",Use_Data(bool(true)),
    "false",Use_Data(bool(false)),
};
const Str_Type Interpreter::result_string = "ans";
std::ostream* Interpreter::os_err = &std::cerr;
