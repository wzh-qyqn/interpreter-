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
	bool trans;
	//���巨����������ʽ��ֵ
	static void sum_mul(T* paddr,size_t* psave,T&res,  const size_t& max_num,\
		size_t save_num = 0, bool sign_flag = false);
public:
	explicit Matrix();
	explicit Matrix(size_t line_num, size_t column_num);
	explicit Matrix(size_t line_num, size_t column_num,T* pfirst, bool auto_alloc = true);
	explicit Matrix(const T& res);
	Matrix(const Matrix& pmat);
	Matrix(Matrix&& pmat);
	constexpr size_t row() const;//��������
	constexpr size_t col() const;//��������
	void	transpos();		//����ת��
	constexpr T* data()const;
	constexpr T	loc(size_t lnum, size_t cnum) const;//���ʾ���Ԫ�أ���C���Զ�ά������ʹ���һ�£���0~row-1 , 0~col-1
	Matrix	alg_complem(size_t lnum, size_t cnum) const;//��ָ�������µĴ�������ʽ
	Matrix	adjoint()const; //��ȡ������
	Matrix	reverse()const;	//������
	T		det() const;	//��ȡ����ʽ��ֵ
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
	friend Matrix operator* (const T& pnum, const Matrix &pmat) {//�˴�����Ҫʹ����Ԫ
		return pmat*pnum;										 //���򵱳��� int���͵�������double���͵ľ���ʱ���ᱨ��
	}
	friend Matrix operator/(const T& pnum, const Matrix &pmat) {
		return pmat.reverse()*pnum;
	}
	~Matrix();
};




//���巨����������ʽ��ֵ
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
	trans = false;
}

template<typename T>
inline Matrix<T>::Matrix(size_t line_num, size_t column_num) {
	row_num = line_num;
	col_num = column_num;
	paddr = new T[row_num*col_num]();
	trans = false;
}

template<typename T>
inline Matrix<T>::Matrix(size_t line_num, size_t column_num, T * pfirst, bool auto_alloc) {
	row_num = line_num;
	col_num = column_num;
	trans = false;
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
	trans = false;
}

template<typename T>
inline Matrix<T>::Matrix(const Matrix & pmat) {
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	paddr = new T[row_num*col_num];
	trans = pmat.trans;
	for (size_t i = 0; i < row_num*col_num; i++) {
		paddr[i] = pmat.paddr[i];
	}
}

template<typename T>
inline Matrix<T>::Matrix(Matrix && pmat) {
	row_num = pmat.row_num;
	col_num = pmat.col_num;
	trans = pmat.trans;
	paddr = pmat.paddr;
	pmat.paddr = nullptr;
}

template<typename T>
constexpr inline size_t Matrix<T>::row() const {
	return trans ? col_num : row_num;
}

template<typename T>
constexpr inline size_t Matrix<T>::col() const {
	return trans ? row_num : col_num;
}

//��C���Զ�ά������ʹ���һ�£���0~line-1 , 0~colum-1
template<typename T>
constexpr inline T Matrix<T>::loc(size_t lnum, size_t cnum) const {
	if (lnum >= row() || cnum >= col()) {
		throw inter_error("����Ԫ�ط��ʴ���");
	}
	return paddr[(trans? cnum :lnum)*col_num + (trans ? lnum : cnum)];
}

//��ָ�������µĴ�������ʽ
template<typename T>
inline Matrix<T> Matrix<T>::alg_complem(size_t lnum, size_t cnum) const {
	T* pbuf;
	T sflag = (lnum + cnum) % 2 ? -1 : 1;
	size_t prow_num = row();
	size_t pcol_num = col();
	if (prow_num == 0 || pcol_num == 0)
		return Matrix(0, 0);
	pbuf = new T[(prow_num - 1)*(pcol_num - 1)];
	for (size_t i = 0, k = 0; i < prow_num; i++) {
		for (size_t j = 0; j < pcol_num; j++) {
			if (i != lnum&&j != cnum) {
				pbuf[k] = sflag*loc(i, j);
				k++;
			}
		}
	}
	return Matrix(prow_num - 1, pcol_num - 1, pbuf, false);
}

template<typename T>
inline Matrix<T> Matrix<T>::adjoint() const {
	T* pbuf;
	size_t prow = row();
	size_t pcol = col();
	if (prow == 0 || col_num == 0)
		return Matrix(0, 0);
	if (row_num != col_num) {
		throw inter_error("�Ƿ����޷���ȡ������");
	}
	pbuf = new T[prow*pcol];
	for (size_t i = 0; i < prow; i++) {
		for (size_t j = 0; j < pcol; j++) {
			pbuf[col()*i + j] = alg_complem(j, i).det();
		}
	}
	return Matrix(prow, pcol, pbuf, false);
}

//������
template<typename T>
inline Matrix<T> Matrix<T>::reverse() const {
	T&& res_det = det();
	if (res_det == T(0)) {
		throw inter_error("����Ϊ�������޷���ȡ����");
	}
	return adjoint()*(T(1) / res_det);
}

//����ת��
template<typename T>
inline void Matrix<T>::transpos() {
	trans = !trans;
}

template<typename T>
constexpr inline T * Matrix<T>::data() const{
	return paddr;
}

//���巨��ȡ����ʽ��ֵ
template<typename T>
inline T Matrix<T>::det() const {
	size_t* pbuf;
	T res(0);
	if (row_num != col_num) {
		throw inter_error("�Ƿ����޷���ȡ����ʽ��ֵ");
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
	trans = pmat.trans;
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
	trans = pmat.trans;
	paddr = pmat.paddr;
	pmat.paddr = nullptr;
	return *this;
}

template<typename T>
inline bool  Matrix<T>::operator==(const Matrix<T> &pmat) const {
	if (row() != pmat.row() || col() != pmat.col()) {
		throw inter_error("��ͬ�;����޷��Ƚ�");
	}
	for (size_t i = 0; i < row(); i++) {
		for (size_t j = 0; j < col(); j++) {
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
	size_t lrow = row();
	size_t lcol = col();
	size_t rrow = pmat.row();
	size_t rcol = pmat.col();
	if (lcol != rcol || lrow != rrow) {
		throw inter_error("��ͬ�;����޷����");
	}
	pFirst = new T[lcol*lrow];
	for (size_t i = 0; i < lrow; i++) {
		for (size_t j = 0; j < lcol; j++) {
			pFirst[lcol*i + j] = loc(i, j) + pmat.loc(i, j);
		}
	}
	return Matrix(lrow, lcol, pFirst, false);
}
template<typename T>
inline Matrix<T> Matrix<T>::operator-(const Matrix<T> &pmat)const {
	T* pFirst;
	size_t lrow = row();
	size_t lcol = col();
	size_t rrow = pmat.row();
	size_t rcol = pmat.col();
	if (lcol != rcol || lrow != rrow) {
		throw inter_error("��ͬ�;����޷����");
	}
	pFirst = new T[lcol*lrow];
	for (size_t i = 0; i < lrow; i++) {
		for (size_t j = 0; j < lcol; j++) {
			pFirst[lcol*i + j] = loc(i, j) - pmat.loc(i, j);
		}
	}
	return Matrix(lrow, lcol, pFirst, false);
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*(const T &pnum)const {
	T* pFirst;
	size_t lrow = row();
	size_t lcol = col();
	pFirst = new T[lrow*lcol];
	for (size_t i = 0; i < lrow; i++) {
		for (size_t j = 0; j < lcol; j++) {
			pFirst[lcol*i + j] = loc(i, j)*pnum;
		}
	}
	return Matrix(lrow, lcol, pFirst, false);
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T> &pmat)const {
	T* pFirst;
	size_t lrow = row();
	size_t lcol = col();
	size_t rrow = pmat.row();
	size_t rcol = pmat.col();
	if (lcol != rrow) {
		throw inter_error("��������������ͱ������������������");
	}
	pFirst = new T[lrow*rcol];
	if (pFirst == nullptr) {
		throw inter_error();
	}
	for (size_t i = 0; i < lrow; i++) {
		for (size_t j = 0; j < rcol; j++) {
			pFirst[rcol*i + j] = loc(i, 0)*pmat.loc(0, j);
			for (size_t m = 1; m < lcol; m++) {
				pFirst[rcol*i + j] += loc(i, m)*pmat.loc(m, j);
			}
		}
	}
	return Matrix(lrow, rcol, pFirst, false);
}

//�������
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

