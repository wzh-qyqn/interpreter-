#pragma once
#ifndef INTERPRETER_MATRIX__H__
#define INTERPRETER_MATRIX__H__

#include "interpreter_err.h"

namespace interpret {

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
	Matrix(const Matrix& pmat);
	Matrix(Matrix&& pmat);
	constexpr size_t row() const;//矩阵行数
	constexpr size_t col() const;//矩阵列数
	constexpr T* data()const;	 //数据源地址
	constexpr T& at(size_t lnum, size_t cnum) const;//访问矩阵元素，与C语言二维数组访问规则一致，从0~row-1 , 0~col-1
	Matrix	rev() const;			//矩阵转置
	Matrix	alg(size_t lnum, size_t cnum) const;//求指定坐标下的余子式
	Matrix	adj()const; //求取伴随阵
	Matrix	inv()const;		//求逆阵
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
		return pmat.inv()*pnum;
	}
	~Matrix();
};

//定义法迭代求行列式的值
template<typename T>
inline void Matrix<T>::sum_mul(T * paddr, size_t * psave, \
	T & res, const size_t & max_num, size_t save_num, bool sign_flag) {
	if (save_num == max_num) {
		T buf_res(1);
		for (size_t i = 0; i < max_num; i++) {
			buf_res *= paddr[i*max_num + psave[i]];
		}
		res += (sign_flag ? T(-1) : T(1))*buf_res;
		return;
	}
	for (size_t i = 0; i < max_num; i++) {//生成排列
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
	paddr = new T[row_num*col_num];
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
	pmat.col_num = 0;
	pmat.row_num = 0;
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
constexpr inline T& Matrix<T>::at(size_t lnum, size_t cnum) const {
	if (lnum >= row_num || cnum >= col_num) {
		throw show_err("矩阵元素访问错误，可访问范围：0~", \
			row_num-1 ," 0~", col_num-1,"输入访问的坐标值：", lnum," ,", cnum);
	}
	return paddr[lnum*col_num + cnum];
}

//求指定坐标下的余子式
template<typename T>
inline Matrix<T> Matrix<T>::alg(size_t lnum, size_t cnum) const {
	if (lnum >= row_num || cnum >= col_num) {
		throw inter_error("矩阵元素访问错误！");
	}
	Matrix<T> pbuf(row_num-1, col_num-1);
	for (size_t i = 0, k = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			if (i != lnum&&j != cnum) {
				pbuf.data()[k]= at(i, j);
				k++;
			}
		}
	}
	return pbuf;
}

template<typename T>
inline Matrix<T> Matrix<T>::adj() const {
	if (row_num == 0 || col_num == 0)
		return Matrix(0, 0);
	if (row_num != col_num) {
		throw inter_error("非方阵无法求取伴随阵");
	}
	Matrix<T> pbuf(row_num, col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pbuf.at(i,j) = ((j+i)%2?T(-1):T(1))*alg(j, i).det();
		}
	}
	return pbuf;
}

//求逆阵
template<typename T>
inline Matrix<T> Matrix<T>::inv() const {
	T&& res_det = det();
	if (res_det == T(0)) {
		throw inter_error("矩阵为奇异阵无法求取逆阵");
	}
	return adj()*(T(1) / res_det);
}

//矩阵转置
template<typename T>
inline Matrix<T> Matrix<T>::rev() const{
	Matrix<T> pbuf(row_num,col_num);
	for (size_t i = 0; i < col_num; i++) {
		for (size_t j = 0; j < row_num; j++) {
			pbuf.at(i,j)= at(j, i);
		}
	}
	return pbuf;
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
	pmat.row_num = 0;
	pmat.col_num = 0;
	return *this;
}

template<typename T>
inline bool  Matrix<T>::operator==(const Matrix<T> &pmat) const {
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法比较");
	}
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			if (at(i, j) != pmat.at(i, j))
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
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法相加");
	}
	Matrix<T> pFirst(row_num,col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst.at(i,j) = at(i, j) + pmat.at(i, j);
		}
	}
	return pFirst;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator-(const Matrix<T> &pmat)const {
	if (row_num != pmat.row_num || col_num != pmat.col_num) {
		throw inter_error("非同型矩阵无法相减");
	}
	Matrix<T> pFirst(row_num, col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst.at(i, j) = at(i, j) - pmat.at(i, j);
		}
	}
	return pFirst;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*(const T &pnum)const {
	Matrix<T> pFirst(row_num, col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pFirst.at(i, j) = at(i, j)*pnum;
		}
	}
	return pFirst;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T> &pmat)const {
	if (col_num != pmat.row_num) {
		throw inter_error("乘数矩阵的行数和被乘数矩阵的列数不等");
	}
	Matrix<T> pFirst(row_num, pmat.col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < pmat.col_num; j++) {
			pFirst.at(i ,j) = at(i, 0)*pmat.at(0, j);
			for (size_t m = 1; m < col_num; m++) {
				pFirst.at(i, j) += at(i, m)*pmat.at(m, j);
			}
		}
	}
	return pFirst;
}

//矩阵除法
template<typename T>
inline Matrix<T> Matrix<T>::operator/(const Matrix<T> &pmat) const {
	return (*this)*(pmat.inv());
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

