/********************************************************************************
* @File name: interpreter_math.cpp
* @Author: wzh
* @Version: 3.1
* @Date: 2020-5-12
* @Description: ����֧��Base_Data�������͵���ѧ����
********************************************************************************/
#include "interpreter.h"


using namespace interpret;
namespace {
typedef interpret::Interpreter::Base_Data Use_Data;
static inline Cmatrix_Type Num_To_Complex(const Matrix_Type& a) {
	size_t row_num = a.row();
	size_t col_num = a.col();
	Cmatrix_Type pbuf(row_num, col_num);
	for (size_t i = 0; i < row_num; i++) {
		for (size_t j = 0; j < col_num; j++) {
			pbuf.at(i,j) = a.at(i, j);
		}
	}
	return pbuf;
}
//��������֧�����͵ļӷ�
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
	throw show_err("���������޷����");
}
static inline Matrix_Type raddition(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type raddition(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type raddition(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type raddition(const Matrix_Type* a, const bool& b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type raddition(const bool& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const Cmatrix_Type* a, const bool& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type raddition(const bool& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
//��������֧�����͵ļ���
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
	throw show_err("���������޷����");
}
static inline Matrix_Type rsubstration(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type rsubstration(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type rsubstration(const Matrix_Type* a, const bool& b) {
	throw show_err("���������޷����");
}
static inline Matrix_Type rsubstration(const bool& a, const Matrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const Cmatrix_Type* a, const bool& b) {
	throw show_err("���������޷����");
}
static inline Cmatrix_Type rsubstration(const bool& a, const Cmatrix_Type* b) {
	throw show_err("���������޷����");
}
//��������֧�����͵ĳ˷�
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
//��������֧�����͵ĳ���
static inline Num_Type	rdivision(const Num_Type& a, const Num_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Num_Type& a, const Complex_Type& b) {
	return a / b;
}
static inline Num_Type	rdivision(const Num_Type& a, const bool& b) {
	throw show_err("�벻Ҫ���߼�������Ϊ��������Ϊ��ܴ���ܻ���ֳ������");
}
static inline Complex_Type	rdivision(const Complex_Type& a, const Num_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Complex_Type& a, const Complex_Type& b) {
	return a / b;
}
static inline Complex_Type	rdivision(const Complex_Type& a, const bool& b) {
	throw show_err("�벻Ҫ���߼�������Ϊ��������Ϊ��ܴ���ܻ���ֳ������");
}
static inline Num_Type	rdivision(const bool& a, const Num_Type& b) {
	return Num_Type(a) /b;
}
static inline Complex_Type	rdivision(const bool& a, const Complex_Type& b) {
	return Num_Type(a) / b;
}
static inline Num_Type	rdivision(const bool& a, const bool& b) {
	throw show_err("�벻Ҫ���߼�������Ϊ��������Ϊ��ܴ���ܻ���ֳ������");
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
//��������֧�����͵ĳ˷�
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
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Matrix_Type rpow(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Matrix_Type rpow(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Matrix_Type rpow(const Matrix_Type* a, const bool& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Matrix_Type rpow(const bool& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const Cmatrix_Type* a, const bool& b) {
	throw show_err("�ݲ�֧�־���������");
}
static inline Cmatrix_Type rpow(const bool& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־���������");
}
//��������֧�����͵�����
static inline Num_Type	rmod(const Num_Type& a, const Num_Type& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const Num_Type& a, const Complex_Type& b) {
	throw show_err("δ���帴��ȡ��");
}
static inline Num_Type	rmod(const Num_Type& a, const bool& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const Complex_Type& a, const Num_Type& b) {
	return Complex_Type(fmod(a.real(), b), fmod(a.imag(), b));
}
static inline Complex_Type	rmod(const Complex_Type& a, const Complex_Type& b) {
	throw show_err("δ���帴��ȡ��");
}
static inline Complex_Type	rmod(const Complex_Type& a, const bool& b) {
	throw show_err("δ���帴��ȡ��");
}
static inline Num_Type	rmod(const bool& a, const Num_Type& b) {
	return fmod(a, b);
}
static inline Complex_Type	rmod(const bool& a, const Complex_Type& b) {
	throw show_err("δ���帴��ȡ��");
}
static inline Num_Type	rmod(const bool& a, const bool& b) {
	return fmod(a, b);
}
static inline Matrix_Type rmod(const Matrix_Type* a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Matrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Matrix_Type rmod(const Matrix_Type* a, const Num_Type& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Matrix_Type rmod(const Num_Type& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Matrix_Type* a, const Complex_Type& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Complex_Type& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Matrix_Type rmod(const Matrix_Type* a, const bool& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Matrix_Type rmod(const bool& a, const Matrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Num_Type& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Num_Type& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const Complex_Type& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Complex_Type& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const Cmatrix_Type* a, const bool& b) {
	throw show_err("�ݲ�֧�־�����������");
}
static inline Cmatrix_Type rmod(const bool& a, const Cmatrix_Type* b) {
	throw show_err("�ݲ�֧�־�����������");
}
Use_Data urgreater(const Use_Data& a, const Use_Data& b) {
	return Use_Data(!(a < b));
}
Use_Data urless(const Use_Data& a, const Use_Data& b) {
	return Use_Data(a <b);
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
Use_Data urtgamma(const Use_Data&a);
static inline Interpreter::Array_Data rtgamma(const Interpreter::Array_Data* a) {
	Interpreter::Array_Data pvec;
	for (size_t i = 0; i < a->size(); i++) {
		pvec.push_back(urtgamma((*a)[i]));
	}
	return pvec;
}
static inline Matrix_Type rtgamma(const Matrix_Type* a1) {
	size_t row_num = a1->row();
	size_t col_num = a1->col();
	Matrix_Type pbuf(row_num, col_num);
	for (size_t i = 0; i<row_num; i++) {
		for (size_t j = 0; j<col_num; j++) {
			pbuf.at(i,j) = tgamma(a1->at(i, j));
		}
	}
	return pbuf;
}
static inline Cmatrix_Type rtgamma(const Cmatrix_Type* a1){
	throw show_err("��֧�ָ����Ľ׳�");
}
static inline Num_Type rtgamma(const Num_Type& a) {
	return tgamma(a);
}
static inline Num_Type rtgamma(const bool& a) {
	return tgamma(Num_Type(a));
}
static inline Complex_Type rtgamma(const Complex_Type& a) {
	throw show_err("��֧�ָ����Ľ׳�");
}

static inline Num_Type rdet(const Matrix_Type* a) {
	return a->det();
}
static inline Complex_Type rdet(const Cmatrix_Type* a) {
	return a->det();
}
static inline Matrix_Type rreverse(const Matrix_Type* a) {
	return a->inv();
}
static inline Cmatrix_Type rreverse(const Cmatrix_Type* a) {
	return a->inv();
}
static inline Num_Type rrow(const Matrix_Type* a) {
	return Num_Type(a->row());
}
static inline Num_Type rrow(const Cmatrix_Type* a) {
	return Num_Type(a->row());
}
static inline Num_Type rcol(const Matrix_Type* a) {
	return Num_Type(a->col());
}
static inline Num_Type rcol(const Cmatrix_Type* a) {
	return Num_Type(a->col());
}
static inline Matrix_Type rtranse(const Matrix_Type* a) {
	return a->rev();
}
static inline Cmatrix_Type rtranse(const Cmatrix_Type* a) {
	return a->rev();
}
static inline Num_Type rsize(const Interpreter::Array_Data* pvec) {
	return Num_Type(pvec->size());
}
static inline Interpreter::Array_Data rsort(const Interpreter::Array_Data* pvec) {
	Interpreter::Array_Data buf_pvec(*pvec);
	sort(buf_pvec.begin(), buf_pvec.end());
	return buf_pvec;
}

//ǿ��ת��Ϊbool
static inline bool rbool(const bool a) {
	return a;
}
static inline bool rbool(const Num_Type a) {
	return a==0 ?false:true;
}
static inline bool rbool(const Complex_Type a) {
	return a == Complex_Type(0) ? false : true;
}
static inline bool rbool(const Matrix_Type* a) {
	return *a == Matrix_Type(a->row(), a->col()) ? false : true;
}
static inline bool rbool(const Cmatrix_Type* a) {
	return *a == Cmatrix_Type(a->row(), a->col()) ? false : true;
}
bool rbool(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rbool(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rbool(a1);
	}
	case Interpreter::DATA_CMATRIX: {
		const Matrix_Type* a1;
		a.get_data(a1);
		return rbool(a1);
	}
	case Interpreter::DATA_COMPLEX: {
		Complex_Type a1;
		a.get_data(a1);
		return rbool(a1);
	}
	case Interpreter::DATA_MATRIX: {
		const Matrix_Type* a1;
		a.get_data(a1);
		return rbool(a1);
	}
	case Interpreter::DATA_ARRAY:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊboolֵ");
	default: 
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}

}
static inline Interpreter::Array_Data rbool(const Interpreter::Array_Data* a) {
	Interpreter::Array_Data parray;
	for (size_t i = 0; i < a->size(); i++) {
		parray.push_back(Use_Data(rbool(a->at(i))));
	}
	return parray;
}
Use_Data urbool(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return Use_Data(rbool(a1));
	}
	else
		return Use_Data(rbool(a));
}

//ǿ��ת��Ϊnum
static inline Num_Type rnum(const bool a) {
	return Num_Type(a);
}
static inline Num_Type rnum(const Num_Type a) {
	return a;
}
Num_Type rnum(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rnum(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rnum(a1);
	}
	case Interpreter::DATA_CMATRIX:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����");
	case Interpreter::DATA_COMPLEX:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊʵ��");
	case Interpreter::DATA_MATRIX:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����");
	case Interpreter::DATA_ARRAY:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����Ԫ��");
	default:
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}
}
static inline Interpreter::Array_Data rnum(const Interpreter::Array_Data* a) {
	Interpreter::Array_Data parray;
	for (size_t i = 0; i < a->size(); i++) {
		parray.push_back(Use_Data(rnum(a->at(i))));
	}
	return parray;
}
Use_Data urnum(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return Use_Data(rnum(a1));
	}
	else
		return Use_Data(rnum(a));
}

//ǿ��ת��Ϊ����
static inline Complex_Type rcomp(const bool a) {
	return Complex_Type(a);
}
static inline Complex_Type rcomp(const Num_Type a) {
	return Complex_Type(a);
}
static inline Complex_Type rcomp(const Complex_Type a) {
	return Complex_Type(a);
}

Complex_Type rcomp(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rcomp(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rcomp(a1);
	}
	case Interpreter::DATA_CMATRIX: {
		throw show_err("ת��ʧ�ܣ����������޷�ת��Ϊ����");
	}
	case Interpreter::DATA_COMPLEX: {
		Complex_Type a1;
		a.get_data(a1);
		return rcomp(a1);
	}
	case Interpreter::DATA_MATRIX: {
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����");
	}
	case Interpreter::DATA_ARRAY:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����");
	default:
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}
}
static inline Interpreter::Array_Data rcomp(const Interpreter::Array_Data* a) {
	Interpreter::Array_Data parray;
	for (size_t i = 0; i < a->size(); i++) {
		parray.push_back(Use_Data(rcomp(a->at(i))));
	}
	return parray;
}
Use_Data urcomp(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return Use_Data(rcomp(a1));
	}
	else
		return Use_Data(rcomp(a));
}
//ǿ��ת��Ϊ����
static inline Matrix_Type rmatrix(const bool a) {
	Matrix_Type pbuf(1, 1);
	pbuf.data()[0] = a;
	return pbuf;
}
static inline Matrix_Type rmatrix(const Num_Type a) {
	Matrix_Type pbuf(1, 1);
	pbuf.data()[0] = a;
	return pbuf;
}
static inline Matrix_Type rmatrix(const Matrix_Type* a) {
	return *a;
}
Matrix_Type rmatrix(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rmatrix(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rmatrix(a1);
	}
	case Interpreter::DATA_MATRIX: {
		const Matrix_Type* a1;
		a.get_data(a1);
		return rmatrix(a1);
	}
	case Interpreter::DATA_CMATRIX:
		throw show_err("ת��ʧ�ܣ����������޷�ת��Ϊʵ������");
	case Interpreter::DATA_COMPLEX:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊʵ������");
	case Interpreter::DATA_ARRAY:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����Ԫ��");
	default:
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}
}
static inline Matrix_Type rmatrix(const Interpreter::Array_Data* a) {
	const size_t& data_size = a->size();
	Matrix_Type pbuf(1, data_size);
	for (size_t i = 0; i < data_size; i++) {
		pbuf.at(0,i)= rnum(a->at(i));
	}
	return pbuf;
}
Use_Data urmatrix(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return Use_Data(rmatrix(a1));
	}
	else
		return Use_Data(rmatrix(a));
}

//ǿ�Ƹ�������
static inline Cmatrix_Type rcmatrix(const bool a) {
	Cmatrix_Type pbuf(1, 1);
	pbuf.data()[0] = a;
	return pbuf;
}
static inline Cmatrix_Type rcmatrix(const Num_Type a) {
	Cmatrix_Type pbuf(1, 1);
	pbuf.data()[0] = a;
	return pbuf;
}
static inline Cmatrix_Type rcmatrix(const Complex_Type a) {
	Cmatrix_Type pbuf(1, 1);
	pbuf.data()[0] = a;
	return pbuf;
}
static inline Cmatrix_Type rcmatrix(const Matrix_Type* a) {
	return Num_To_Complex(*a);
}
static inline Cmatrix_Type rcmatrix(const Cmatrix_Type* a) {
	return *a;
}
Cmatrix_Type rcmatrix(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rcmatrix(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rcmatrix(a1);
	}
	case Interpreter::DATA_MATRIX: {
		const Matrix_Type* a1;
		a.get_data(a1);
		return rcmatrix(a1);
	}
	case Interpreter::DATA_CMATRIX: {
		const Cmatrix_Type* a1;
		a.get_data(a1);
		return rcmatrix(a1);
	}
	case Interpreter::DATA_COMPLEX: {
		Complex_Type a1;
		a.get_data(a1);
		return rcmatrix(a1);
	}
	case Interpreter::DATA_ARRAY:
		throw show_err("ת��ʧ�ܣ������޷�ת��Ϊ����Ԫ��");
	default:
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}
}
static inline Cmatrix_Type rcmatrix(const Interpreter::Array_Data* a) {
	Cmatrix_Type pbuf (1,a->size());
	for (size_t i = 0; i < a->size(); i++) {
		pbuf.at(0,i) = rcomp(a->at(i));
	}
	return pbuf;
}
Use_Data urcmatrix(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return Use_Data(rcmatrix(a1));
	}
	else
		return Use_Data(rcmatrix(a));
}

//ǿ��ת��Ϊ����
static inline Interpreter::Array_Data rarray(const bool a) {
	Interpreter::Array_Data pvec;
	pvec.push_back(Use_Data(a));
	return pvec;
}
static inline Interpreter::Array_Data rarray(const Num_Type a) {
	Interpreter::Array_Data pvec;
	pvec.push_back(Use_Data(a));
	return pvec;
}
static inline Interpreter::Array_Data rarray(const Complex_Type a) {
	Interpreter::Array_Data pvec;
	pvec.push_back(Use_Data(a));
	return pvec;
}
static inline Interpreter::Array_Data rarray(const Matrix_Type* a) {
	Interpreter::Array_Data pvec;
	for (size_t i = 0; i < a->row(); i++) {
		for (size_t j = 0; j < a->col(); j++) {
			pvec.push_back(Use_Data(a->at(i, j)));
		}
	}
	return pvec;
}
static inline Interpreter::Array_Data rarray(const Cmatrix_Type* a) {
	Interpreter::Array_Data pvec;
	for (size_t i = 0; i < a->row(); i++) {
		for (size_t j = 0; j < a->col(); j++) {
			pvec.push_back(Use_Data(a->at(i, j)));
		}
	}
	return pvec;
}
static inline Interpreter::Array_Data rarray(const Interpreter::Array_Data* a) {
	return *a;
}

Interpreter::Array_Data rarray(const Use_Data& a) {
	switch (a.get_type()) {
	case Interpreter::DATA_BOOL: {
		bool a1;
		a.get_data(a1);
		return rarray(a1);
	}
	case Interpreter::DATA_DOUBLE: {
		Num_Type a1;
		a.get_data(a1);
		return rarray(a1);
	}
	case Interpreter::DATA_CMATRIX: {
		const Cmatrix_Type* a1;
		a.get_data(a1);
		return rarray(a1);
	}
	case Interpreter::DATA_COMPLEX: {
		Complex_Type a1;
		a.get_data(a1);
		return rarray(a1);
	}
	case Interpreter::DATA_MATRIX: {
		const Matrix_Type* a1;
		a.get_data(a1);
		return rarray(a1);
	}
	case Interpreter::DATA_ARRAY: {
		const Interpreter::Array_Data* a1;
		a.get_data(a1);
		return rarray(a1);
	}
	default: {
		throw show_err("ת��ʧ�ܣ����ʹ���");
	}
	}
}
Use_Data urarry(const Use_Data& a) {
	return Use_Data(rarray(a));
}

Use_Data rzero(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* pvec;
		a.get_data(pvec);
		if (pvec->size() == 2) {
			Num_Type row_num, col_num;
			pvec->at(0).get_data(row_num);
			pvec->at(1).get_data(col_num);
			return Use_Data(Matrix_Type(size_t(row_num), size_t(col_num)));
		}else
			throw show_err("������������");
	}else
		throw show_err("������������");
}

Use_Data rdim(const Use_Data& a) {
	if (a.get_type() == Interpreter::DATA_ARRAY) {
		const Interpreter::Array_Data* pvec;
		a.get_data(pvec);
		if (pvec->size() == 2) {
			Num_Type row_num, col_num;
			pvec->at(0).get_data(row_num);
			pvec->at(1).get_data(col_num);
			Matrix_Type pbuf(static_cast<size_t>(row_num), static_cast<size_t>(col_num));
			for (size_t i = 0; i < size_t(row_num); i++) {
				for (size_t j = 0; j < size_t(col_num); j++) {
					if (i == j) {
						pbuf.at(i, j) = 1;
					}
					else
						pbuf.at(i, j) = 0;
				}
			}
			return Use_Data(pbuf);
		}
		else
			throw show_err("������������");
	}
	else
		throw show_err("������������");
}
#define MATRIX_FUNCTION_DECLARE(name)	\
Use_Data m##name##(const Use_Data& a){\
	switch(a.get_type()){\
	case Interpreter::DATA_MATRIX:{\
		const Matrix_Type* a1;\
		a.get_data(a1); \
		return Use_Data(name##(a1));\
		} \
	case Interpreter::DATA_CMATRIX:{\
		const Cmatrix_Type* a1;\
		a.get_data(a1); \
		return Use_Data(name##(a1));\
		} \
	default:\
		throw show_err("������֧��������"); \
	}\
}
#define ARRAY_FUNCTION_DECLARE(name)    \
Use_Data a##name##(const Use_Data& a){\
	switch(a.get_type()){\
	case Interpreter::DATA_ARRAY:{\
		const Interpreter::Array_Data* a1;\
		a.get_data(a1);\
		return Use_Data(name##(a1));\
	}\
	default:\
		throw show_err("������֧��������"); \
	}\
}
#define SINGLEVAR_FUNCTION_DECLARE(name) \
Use_Data u##name##(const Use_Data& a) {\
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
	const Interpreter::Array_Data* a1;\
	a.get_data(a1);\
	return Use_Data(name##(a1));\
}\
case Interpreter::DATA_MATRIX:{\
	const Matrix_Type* a1;\
	a.get_data(a1);\
	return Use_Data(name##(a1));\
}\
case Interpreter::DATA_CMATRIX:{\
	const Cmatrix_Type* a1;\
	a.get_data(a1);\
	return Use_Data(name##(a1));\
}\
default:\
    throw show_err("������֧��������");\
}\
}
#define BINARY_FUNCTION_DECLARE(name)	\
Use_Data u##name##(const Use_Data& a, const Use_Data& b){\
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
			const Matrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_CMATRIX:{\
			const Cmatrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_ARRAY: {\
			const Interpreter::Array_Data *b1;\
			Interpreter::Array_Data pvec;\
			b.get_data(b1);\
			for (size_t i = 0; i < b1->size(); i++) {\
				pvec.push_back(u##name##(a, (*b1)[i]));\
			}\
			return Use_Data(pvec);\
			}\
		default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",b.get_type());\
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
			const Matrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_CMATRIX:{\
			const Cmatrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_ARRAY: {\
			const Interpreter::Array_Data *b1;\
			Interpreter::Array_Data pvec;\
			b.get_data(b1);\
			for (size_t i = 0; i < b1->size(); i++) {\
				pvec.push_back(u##name##(a, (*b1)[i]));\
			}\
			return Use_Data(pvec);\
			}\
		default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",b.get_type());\
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
			const Matrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_CMATRIX:{\
			const Cmatrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_ARRAY: {\
			const Interpreter::Array_Data *b1;\
			Interpreter::Array_Data pvec;\
			b.get_data(b1);\
			for (size_t i = 0; i < b1->size(); i++) {\
				pvec.push_back(u##name##(a, (*b1)[i]));\
			}\
			return Use_Data(pvec);\
			}\
		default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",b.get_type());\
		}\
	}\
	case Interpreter::DATA_MATRIX:{\
		const Matrix_Type* a1;\
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
			const Matrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_CMATRIX:{\
			const Cmatrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_ARRAY: {\
			const Interpreter::Array_Data *b1;\
			Interpreter::Array_Data pvec;\
			b.get_data(b1);\
			for (size_t i = 0; i < b1->size(); i++) {\
				pvec.push_back(u##name##(a, (*b1)[i]));\
			}\
			return Use_Data(pvec);\
			}\
		default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",b.get_type());\
		}\
	}\
	case Interpreter::DATA_CMATRIX:{\
		const Cmatrix_Type* a1;\
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
			const Matrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_CMATRIX:{\
			const Cmatrix_Type* b1;\
			b.get_data(b1); \
			return Use_Data(name##(a1, b1)); \
			}\
		case Interpreter::DATA_ARRAY: {\
			const Interpreter::Array_Data *b1;\
			Interpreter::Array_Data pvec;\
			b.get_data(b1);\
			for (size_t i = 0; i < b1->size(); i++) {\
				pvec.push_back(u##name##(a, (*b1)[i]));\
			}\
			return Use_Data(pvec);\
			}\
		default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",b.get_type());\
		}\
	}\
	case Interpreter::DATA_ARRAY: {\
		const Interpreter::Array_Data *a1;\
		Interpreter::Array_Data pvec;\
		a.get_data(a1);\
		if (b.get_type() == Interpreter::DATA_ARRAY) {\
			const Interpreter::Array_Data *b1;\
			b.get_data(b1);\
			if (a1->size() != b1->size())\
				throw show_err("��������ά����ƥ��");\
			for (size_t i = 0; i < a1->size(); i++) {\
				pvec.push_back(u##name##((*a1)[i], (*b1)[i]));\
			}\
			return Use_Data(pvec);\
		}\
		else {\
			for (size_t i = 0; i < a1->size(); i++) {\
				pvec.push_back(u##name##((*a1)[i], b));\
			}\
			return Use_Data(pvec);\
		}\
		}\
	default:\
			throw show_err("�˺�����֧�ִ����ͣ��������ͣ�",a.get_type());\
}\
}
 

#define SINGLEVAR_FUNC_NEWDECLARE(name)		\
Use_Data ur##name##(const Use_Data& a);\
static inline Interpreter::Array_Data r##name##(const Interpreter::Array_Data* a){\
	Interpreter::Array_Data pvec;\
	for (size_t i = 0; i < a->size(); i++) {\
		pvec.push_back(ur##name##((*a)[i]));\
	}\
	return pvec;\
}\
static inline Matrix_Type r##name##(const Matrix_Type* a1){\
	size_t row_num = a1->row();\
	size_t col_num = a1->col();\
	Matrix_Type pbuf(row_num,col_num);\
	for (size_t i = 0; i<row_num; i++) {\
		for (size_t j = 0; j<col_num; j++) {\
			pbuf.at(i,j) = name##(a1->at(i, j));\
		}\
	}\
	return pbuf;\
}\
static inline Cmatrix_Type r##name##(const Cmatrix_Type* a1){\
	size_t row_num = a1->row();\
	size_t col_num = a1->col();\
	Cmatrix_Type pbuf(row_num,col_num);\
	for (size_t i = 0; i<row_num; i++) {\
		for (size_t j = 0; j<col_num; j++) {\
			pbuf.at(i,j) = name##(a1->at(i, j));\
		}\
	}\
	return pbuf;\
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
SINGLEVAR_FUNCTION_DECLARE(rtgamma)

MATRIX_FUNCTION_DECLARE(rdet)
MATRIX_FUNCTION_DECLARE(rreverse)
MATRIX_FUNCTION_DECLARE(rrow)
MATRIX_FUNCTION_DECLARE(rtranse)
MATRIX_FUNCTION_DECLARE(rcol)

ARRAY_FUNCTION_DECLARE(rsize)
ARRAY_FUNCTION_DECLARE(rsort)

static inline Use_Data rsum(const Interpreter::Array_Data* pvec) {
	Use_Data pdata(Num_Type(0));
	for (size_t i = 0; i < pvec->size(); i++) {
		pdata = uraddition(pdata, pvec->at(i));
	}
	return pdata;
}
ARRAY_FUNCTION_DECLARE(rsum)
const Interpreter::Binary_Func_Pair sbinary_func[] = {
	'+',4,uraddition,
	'-',4,ursubstration,
	'*',5,urmultiply,
	'/',5,urdivision,
	'%',5,urmod,
	'^',6,urpow,
	'>',3,urgreater,
	'<',3,urless,
};
const Interpreter::Unary_Func_Pair sunary_func[] = {
	'!',8,urtgamma,
	'`',8,mrtranse,
};
const Interpreter::SingleVar_Func_Pair ssinglevar_func[] = {
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
	"imag"	,urimag,
	"det"	,mrdet,
	"inv" ,mrreverse,
	"row"	,mrrow,
	"trans" ,mrtranse,
	"col"	,mrcol,
	"size"	,arsize,
	"sort"	,arsort,
	"sum"	,arsum,
	"bool"	,urbool,
	"double",urnum,
	"complex",urcomp,
	"array"	,urarry,
	"matrix",urmatrix,
	"cmatrix",urcmatrix,
	"zero",rzero,
	"dim",rdim
};
const Interpreter::Const_Num_Pair sconst_BaseData[] = {
	"pi",Use_Data(Num_Type(3.14159265358979323846)),
	"e",Use_Data(Num_Type(2.71828182845904523536)),
	"i",Use_Data(Complex_Type(0,1)),
	"j",Use_Data(Complex_Type(0,1)),
	"true",Use_Data(bool(true)),
	"false",Use_Data(bool(false)),
};

}
const Interpreter::StaticVar_Func Interpreter::singlevar_func\
(ssinglevar_func, sizeof(ssinglevar_func)/sizeof(ssinglevar_func[0]));
const Interpreter::Binary_Func Interpreter::binary_func\
(sbinary_func, sizeof(sbinary_func) / sizeof(sbinary_func[0]));
const Interpreter::Unary_Func Interpreter::unary_func\
(sunary_func, sizeof(sunary_func) / sizeof(sunary_func[0]));
const Interpreter::Const_Num Interpreter::const_BaseData\
(sconst_BaseData, sizeof(sconst_BaseData) / sizeof(sconst_BaseData[0]));

const Str_Type Interpreter::result_string = "ans";
std::ostream* Interpreter::os_err = &std::cerr;
