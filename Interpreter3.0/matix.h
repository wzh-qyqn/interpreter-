#pragma once
#ifndef __MATRIX__H__
#define __MATRIX__H__

#include "interpreter_err.h"

namespace inter {

template <typename T>
class Matrix {
private:
	size_t row_num;
	size_t col_num;
	T* paddr;
	//定义法迭代求行列式的值
	static void sum_mul(T* paddr,size_t* psave,T&res,  const size_t& max_num,\
		size_t save_num = 0, bool sign_flag = false);
public:
	explicit Matrix();
	explicit Matrix(size_t line_num, size_t column_num);
	explicit Matrix(size_t line_num, size_t column_num,T* pfirst, bool auto_alloc = true);
	explicit Matrix(const T& res);
	Matrix(const Matrix& pmat);
	Matrix(Matrix&& pmat);
	constexpr size_t row() const;//矩阵行数
	constexpr size_t col() const;//矩阵列数
	Matrix	transpos();			//矩阵转置
	constexpr T* data()const;
	constexpr T& loc(size_t lnum, size_t cnum) const;//访问矩阵元素，与C语言二维数组访问规则一致，从0~row-1 , 0~col-1
	Matrix	alg_complem(size_t lnum, size_t cnum) const;//求指定坐标下的代数余子式
	Matrix	adjoint()const; //求取伴随阵
	Matrix	reverse()const;	//求逆阵
	T		det() const;	//求取行列式的值
	Matrix	operator=(const Matrix& pmat);
	Matrix	operator=(Matrix&& pmat);
	bool	operator==(const Matrix &pmat) const;
	bool	operator!=(const Matrix &pmat)const;
	Matrix	operator+(const Matrix &pmat)const;
	Matrix	operator-(const Matrix &pmat)const;
	Matrix	operator*(const T &pnum)const;
	Matrix	operator*(const Matrix &pmat)const;
	Matrix	operator/(const Matrix &pmat) const;
	Matrix	operator/(const T &pnum) const;
	Matrix	operator+=(const Matrix &pmat);
	Matrix	operator-=(const Matrix &pmat);
	Matrix	operator*=(const Matrix &pmat);
	Matrix	operator*=(const T &pnum);
	Matrix	operator/=(const Matrix &pmat);
	Matrix	operator/=(const T &pnum);
	friend Matrix operator* (const T& pnum, const Matrix &pmat) {//此处必须要使用友元
		return pmat*pnum;										 //否则当出现 int类型的数乘以double类型的矩阵时，会报错
	}
	friend Matrix operator/(const T& pnum, const Matrix &pmat) {
		return pmat.reverse()*pnum;
	}
	~Matrix();
};




//定义法迭代求行列式的值
template<typename T>
inline void Matrix<T>::sum_mul(T * paddr, size_t * psave, \
	T & res, const size_t & max_num, size_t save_num, bool sign_flag) {
	if (save_num == max_num) {
		T buf_res = 1;
		for (size_t i = 0; i < max_num; i++) {
			buf_res *= paddr[i*max_num + psave[i]];
		}
		res += (sign_flag ? T(-1) : T(1))*buf_res;
		return;
	}
	for (size_t i = 0; i < max_num; i++) {
		bool flag = false;
		bool buf_flag = false;
		for (size_t j = 0; j < save_num; j++) {
			if (i == psave[j]) {
				flag = true;
				break;
			}
			if (psave[j] > i) {
				buf_flag = !buf_flag;
			}
		}
		if (flag)
			continue;
		psave[save_num] = i;
		sum_mul(paddr, psave, res, max_num, save_num + 1, sign_flag^buf_flag);
	}
}

template<typename T>
inline Matrix<T>::Matrix() {
	row_num = 0;
	col_num = 0;
	paddr = nullptr;
}

template<typename T>
inline Matrix<T>::Matrix(size_t line_num, size_t column_num) {
	row_num = line_num;
	col_num = column_num;
	paddr = new T[row_num*col_num]();
}

template<typename T>
inline Matrix<T>::Matrix(size_t line_num, size_t column_num, T * pfirst, bool auto_alloc) {
	row_num = line_num;
	col_num = column_num;
	if (auto_alloc) {
		paddr = new T[row_num*col_num];
		for (size_t i = 0; i < row_num*col_num; i++) {
			paddr[i] = pfirst[i];
		}
	}
	else
		paddr = pfirst;
}

template<typename T>
inline Matrix<T>::Matrix(const T & res) {
	row_num = 1;
	col_num = 1;
	paddr = new T[1];
	*paddr = res;
}

template<typename T>
inline Matrix<T>::Matrix(const Matrix & pmat) {
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	paddr = new T[row_num*col_num];
	for (size_t i = 0; i < row_num*col_num; i++) {
		paddr[i] = pmat.paddr[i];
	}
}

template<typename T>
inline Matrix<T>::Matrix(Matrix && pmat) {
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	paddr = pmat.paddr;
	pmat.paddr = nullptr;
}

template<typename T>
constexpr inline size_t Matrix<T>::row() const {
	return row_num;
}

template<typename T>
constexpr inline size_t Matrix<T>::col() const {
	return col_num;
}

//与C语言二维数组访问规则一致，从0~line-1 , 0~colum-1
template<typename T>
constexpr inline T& Matrix<T>::loc(size_t lnum, size_t cnum) const {
	if (lnum >= row_num || cnum >= col_num) {
		throw show_err("矩阵元素访问错误，可访问范围：0~", \
			row_num-1 ," 0~", col_num-1,"输入访问的坐标值：", lnum," ,", cnum);
	}
	return paddr[lnum*col_num + cnum];
}

//求指定坐标下的代数余子式
template<typename T>
inline Matrix<T> Matrix<T>::alg_complem(size_t lnum, size_t cnum) const {
	T* pbuf;
	T sflag = (lnum + cnum) % 2 ? -1 : 1;
	if (lnum >= row_num || cnum >= col_num) {
		throw inter_error("矩阵元素访问错误！");
	}
	if (row_num == 0 || col_num == 0)
		return Matrix(0, 0);
	pbuf = new T[(row_num - 1)*(col_num - 1)];
	for (size_t i = 0, k = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			if (i != lnum&&j != cnum) {
				pbuf[k] = sflag*loc(i, j);
				k++;
			}
		}
	}
	return Matrix(row_num - 1, col_num - 1, pbuf, false);
}

template<typename T>
inline Matrix<T> Matrix<T>::adjoint() const {
	T* pbuf;
	if (row_num == 0 || col_num == 0)
		return Matrix(0, 0);
	if (row_num != col_num) {
		throw inter_error("非方阵无法求取伴随阵");
	}
	pbuf = new T[row_num*col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pbuf[col()*i + j] = alg_complem(j, i).det();
		}
	}
	return Matrix(row_num, col_num, pbuf, false);
}

//求逆阵
template<typename T>
inline Matrix<T> Matrix<T>::reverse() const {
	T&& res_det = det();
	if (res_det == T(0)) {
		throw inter_error("矩阵为奇异阵无法求取逆阵");
	}
	return adjoint()*(T(1) / res_det);
}

//矩阵转置
template<typename T>
inline Matrix<T> Matrix<T>::transpos() {
	T* pbuf = new T[row_num*col_num];
	for (size_t i = 0; i < col_num; i++) {
		for (size_t j = 0; j < row_num; j++) {
			pbuf[i*row_num + j] = loc(j, i);
		}
	}
	return Matrix(col_num, row_num, pbuf, false);
}

template<typename T>
constexpr inline T * Matrix<T>::data() const{
	return paddr;
}

//定义法求取行列式的值
template<typename T>
inline T Matrix<T>::det() const {
	size_t* pbuf;
	T res(0);
	if (row_num != col_num) {
		throw inter_error("非方阵无法求取行列式的值");
	}
	pbuf = new size_t[row_num];
	sum_mul(paddr, pbuf, res, row_num);
	delete[] pbuf;
	return res;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator=(const Matrix<T>& pmat) {
	if (&pmat == this) {
		return *this;
	}
	delete[] paddr;
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	paddr = new T[row_num*col_num];
	for (size_t i = 0; i < row_num*col_num; i++) {
		paddr[i] = pmat.paddr[i];
	}
	return *this;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator=(Matrix<T>&& pmat) {
	if (&pmat == this) {
		return *this;
	}
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	paddr = pmat.paddr;
	pmat.paddr = nullptr;
	return *this;
}

template<typename T>
inline bool  Matrix<T>::operator==(const Matrix<T> &pmat) const {
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法比较");
	}
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			if (loc(i, j) != pmat.loc(i, j))
				return false;
		}
	}
	return true;
}

template<typename T>
inline bool Matrix<T>::operator!=(const Matrix<T> &pmat)const {
	return !(*this == pmat);
}

template<typename T>
inline Matrix<T> Matrix<T>::operator+(const Matrix<T> &pmat)const {
	T* pFirst;
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法相加");
	}
	pFirst = new T[row_num*col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst[col_num*i + j] = loc(i, j) + pmat.loc(i, j);
		}
	}
	return Matrix(row_num, col_num, pFirst, false);
}
template<typename T>
inline Matrix<T> Matrix<T>::operator-(const Matrix<T> &pmat)const {
	T* pFirst;
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法相减");
	}
	pFirst = new T[row_num*col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst[col_num*i + j] = loc(i, j) - pmat.loc(i, j);
		}
	}
	return Matrix(row_num, col_num, pFirst, false);
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*(const T &pnum)const {
	T* pFirst;
	pFirst = new T[row_num*col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst[col_num*i + j] = loc(i, j)*pnum;
		}
	}
	return Matrix(row_num, col_num, pFirst, false);
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T> &pmat)const {
	T* pFirst;
	if (col_num != pmat.row_num) {
		throw inter_error("乘数矩阵的行数和被乘数矩阵的列数不等");
	}
	pFirst = new T[row_num*pmat.col_num];
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < pmat.col_num; j++) {
			pFirst[pmat.col_num*i + j] = loc(i, 0)*pmat.loc(0, j);
			for (size_t m = 1; m < col_num; m++) {
				pFirst[pmat.col_num*i + j] += loc(i, m)*pmat.loc(m, j);
			}
		}
	}
	return Matrix(row_num, pmat.col_num, pFirst, false);
}

//矩阵除法
template<typename T>
inline Matrix<T> Matrix<T>::operator/(const Matrix<T> &pmat) const {
	return (*this)*(pmat.reverse());
}
template<typename T>
inline Matrix<T> Matrix<T>::operator/(const T &pnum) const {
	return (*this)*(T(1) / pnum);
}
template<typename T>
inline Matrix<T> Matrix<T>::operator+=(const Matrix<T> &pmat) {
	(*this) = (*this) + pmat;
	return *this;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator-=(const Matrix &pmat) {
	(*this) = (*this) - pmat;
	return *this;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*=(const Matrix &pmat) {
	(*this) = (*this) * pmat;
	return *this;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*=(const T &pnum) {
	(*this) = (*this) * pnum;
	return *this;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator/=(const Matrix &pmat) {
	(*this) = (*this) / pmat;
	return *this;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator/=(const T &pnum) {
	(*this) = (*this) / pnum;
	return *this;
}
template<typename T>
Matrix<T>::~Matrix() {
	delete[] paddr;
}



}

#endif

