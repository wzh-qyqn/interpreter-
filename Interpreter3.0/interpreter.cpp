#include "interpreter.h"


//����һ����pData��ʼ�����ڴ�
inline void interpret::Interpreter::Base_Data::_Malloc(const Base_Data & pData) {
	switch (pData.d_type) {
	case DATA_DOUBLE:
		data = new Num_Type(*reinterpret_cast<Num_Type*>(pData.data));
		break;
	case DATA_COMPLEX:
		data = new Complex_Type(*reinterpret_cast<Complex_Type*>(pData.data));
		break;
	case DATA_BOOL:
		data = new bool(*reinterpret_cast<bool*>(pData.data));
		break;
	case DATA_ARRAY:
		data = new Array_Data(*reinterpret_cast<Array_Data*>(pData.data));
		for (size_t i = 0; i < reinterpret_cast<Array_Data*>(data)->size(); i++) {
			if (reinterpret_cast<Array_Data*>(data)->at(i).is_shadow()) {
				reinterpret_cast<Array_Data*>(data)->at(i) = reinterpret_cast<Array_Data*>(data)->at(i).get_real();
			}
		}
		break;
	case DATA_MATRIX:
		data = new Matrix_Type(*reinterpret_cast<Matrix_Type*>(pData.data));
		break;
	case DATA_CMATRIX:
		data = new Cmatrix_Type(*reinterpret_cast<Cmatrix_Type*>(pData.data));
		break;
	default:
		throw show_err("�����������");
	}
}


//�ͷ������������
inline void interpret::Interpreter::Base_Data::_Free(void) {
	switch (d_type) {
	case DATA_DOUBLE:
		delete reinterpret_cast<Num_Type*>(data);
		break;
	case DATA_COMPLEX:
		delete reinterpret_cast<Complex_Type*>(data);
		break;
	case DATA_BOOL:
		delete reinterpret_cast<bool*>(data);
		break;
	case DATA_ARRAY:
		delete reinterpret_cast<Array_Data*>(data);
		break;
	case DATA_MATRIX:
		delete reinterpret_cast<Matrix_Type*>(data);
		break;
	case DATA_CMATRIX:
		delete reinterpret_cast<Cmatrix_Type*>(data);
		break;
	default:
		delete data;
	}
}

interpret::Interpreter::Base_Data::Base_Data() {
	data = nullptr;
	is_own = false;
	is_init = false;
}

interpret::Interpreter::Base_Data::Base_Data(const Num_Type & res) {
	data = new Num_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_DOUBLE;
}

interpret::Interpreter::Base_Data::Base_Data(const Complex_Type & res) {
	data = new Complex_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_COMPLEX;
}

interpret::Interpreter::Base_Data::Base_Data(const bool & res) {
	data = new bool(res);
	is_own = true;
	is_init = true;
	d_type = DATA_BOOL;
}

interpret::Interpreter::Base_Data::Base_Data(const Array_Data & res) {
	data = new Array_Data(res);
	is_own = true;
	is_init = true;
	d_type = DATA_ARRAY;
}

interpret::Interpreter::Base_Data::Base_Data(const Matrix_Type &res){
	data = new Matrix_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_MATRIX;
}

interpret::Interpreter::Base_Data::Base_Data(const Cmatrix_Type &res){
	data = new Cmatrix_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_CMATRIX;
}

interpret::Interpreter::Base_Data::Base_Data(Matrix_Type &&res){
	data = new Matrix_Type;
	(*reinterpret_cast<Matrix_Type*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_MATRIX;
}

interpret::Interpreter::Base_Data::Base_Data(Cmatrix_Type && res){
	data = new Cmatrix_Type;
	(*reinterpret_cast<Cmatrix_Type*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_CMATRIX;
}

interpret::Interpreter::Base_Data::Base_Data(Array_Data && res){
	data = new Array_Data;
	(*reinterpret_cast<Array_Data*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_ARRAY;
}



/**
* @brief ��ñ����ݵ�һ��Ӱ��
* @param ��
* @return ����һ������ΪӰ�ӵ�Base_Data
*/
interpret::Interpreter::Base_Data interpret::Interpreter::Base_Data::get_shadow() const {
	Base_Data pData;
	pData.data = data;
	pData.d_type = d_type;
	pData.is_init = is_init;
	pData.is_own = false;
	return pData;
}


/**
* @brief ��ñ����ݵı���
ע���õı���Ϊһ��ȫ�µı��壬��ԭ�еı�������ݴ洢��ֵ��ͬ
���������κι�ϵ
* @param ��
* @return ����һ������Ϊ�����Base_Data
*/
interpret::Interpreter::Base_Data interpret::Interpreter::Base_Data::get_real() const {
	Base_Data pData;
	if (is_init) {
		pData.d_type = d_type;
		pData.is_init = true;
		pData.is_own = true;
		pData._Malloc(*this);
	}
	else
		show_err("��ȡ���ݱ������������δ��ʼ��������");
	return pData;
}

interpret::Interpreter::Base_Data::Base_Data(const Base_Data & pData) {
	d_type = pData.d_type;
	is_own = pData.is_own;
	is_init = pData.is_init;
	if (is_init&&is_own) {
		_Malloc(pData);
	}
	else {
		data = pData.data;
	}
}

bool interpret::Interpreter::Base_Data::operator<(const Base_Data &pdata)const{
	switch (get_type()) {
	case Interpreter::DATA_DOUBLE: {
		Num_Type ldata;
		get_data(ldata);
		switch (pdata.get_type()) {
		case Interpreter::DATA_DOUBLE: {
			Num_Type rdata;
			pdata.get_data(rdata);
			return ldata < rdata;
		}
		case Interpreter::DATA_BOOL: {
			bool rdata;
			pdata.get_data(rdata);
			return ldata < Num_Type(rdata);
		}
		default:
			throw show_err("�������Ͳ�֧�ֱȽϴ�С");
		}
	case Interpreter::DATA_BOOL: {
		bool ldata;
		get_data(ldata);
		switch (pdata.get_type()) {
		case Interpreter::DATA_DOUBLE: {
			Num_Type rdata;
			pdata.get_data(rdata);
			return Num_Type(ldata) < rdata;
		}
		case Interpreter::DATA_BOOL: {
			bool rdata;
			pdata.get_data(rdata);
			return Num_Type(ldata) < Num_Type(rdata);
		}
		default:
			throw show_err("�������Ͳ�֧�ֱȽϴ�С");
		}
	}
	default:
		throw show_err("�������Ͳ�֧�ֱȽϴ�С");
	}
	}
}

bool interpret::Interpreter::Base_Data::operator>(const Base_Data &pdata) const{
	switch (get_type()) {
	case Interpreter::DATA_DOUBLE: {
		Num_Type ldata;
		get_data(ldata);
		switch (pdata.get_type()) {
		case Interpreter::DATA_DOUBLE: {
			Num_Type rdata;
			pdata.get_data(rdata);
			return ldata > rdata;
		}
		case Interpreter::DATA_BOOL: {
			bool rdata;
			pdata.get_data(rdata);
			return ldata > Num_Type(rdata);
		}
		default:
			throw show_err("�������Ͳ�֧�ֱȽϴ�С");
		}
	case Interpreter::DATA_BOOL: {
		bool ldata;
		get_data(ldata);
		switch (pdata.get_type()) {
		case Interpreter::DATA_DOUBLE: {
			Num_Type rdata;
			pdata.get_data(rdata);
			return Num_Type(ldata) > rdata;
		}
		case Interpreter::DATA_BOOL: {
			bool rdata;
			pdata.get_data(rdata);
			return Num_Type(ldata) > Num_Type(rdata);
		}
		default:
			throw show_err("�������Ͳ�֧�ֱȽϴ�С");
		}
	}
	default:
		throw show_err("�������Ͳ�֧�ֱȽϴ�С");
	}
	}
}

interpret::Interpreter::Base_Data::Base_Data(Base_Data && pData) {
	d_type = pData.d_type;
	is_own = pData.is_own;
	is_init = pData.is_init;
	data = pData.data;
	pData.is_own = false;
}

interpret::Interpreter::Base_Data& interpret::Interpreter::Base_Data::operator=(const Base_Data& pData) {
	if (&pData == this)
		return *this;
	if (is_init&&is_own)
		_Free();
	d_type = pData.d_type;
	is_own = pData.is_own;
	is_init = pData.is_init;
	if (is_init&&is_own) {
		_Malloc(pData);
	}
	else {
		data = pData.data;
	}
	return *this;
}

interpret::Interpreter::Base_Data& interpret::Interpreter::Base_Data::operator=(Base_Data&& pData) {
	if (&pData == this)
		return *this;
	if (is_init&&is_own)
		_Free();
	is_init = pData.is_init;
	d_type = pData.d_type;
	is_own = pData.is_own;
	data = pData.data;
	pData.is_own = false;
	return *this;
}

/**
* @brief �ж��Ƿ�ΪӰ��
* @param ��
* @return bool
*		-<em>false</em> ����Ӱ��
*		-<em>true</em>	��Ӱ��
*/
bool interpret::Interpreter::Base_Data::is_shadow() const {
	if (is_init)
		return !is_own;
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

/**
* @brief ��ȡ��ʵ�洢����������
��Ҫ�ڱ�д��ѧ����ʱʹ�ã����ڵײ����
* @param ��
* @return Date_Type
*		DATA_DOUBLE		ʵ��
*		DATA_COMPLEX	����
*/
interpret::Interpreter::Data_Type interpret::Interpreter::Base_Data::get_type() const {
	if (is_init)
		return d_type;
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

//��ȡ�ڲ��洢������Դ��ַ
void interpret::Interpreter::Base_Data::get_data(const void *& p) const {	//�˴���ȡԴ��ַ��Ϊ�������ǿ���ͨ��
	if (is_init)				//Ӱ�ӿ���Ѱ�ҵ����壬Ӱ���뱾���Ψһ
		p = data;				//��ϵֻ�������ַ��
	else
		throw show_err("��ͼ��ѯδ��ʼ���Ĵ洢��ַ");
}

void interpret::Interpreter::Base_Data::get_data(Num_Type & res) const {
	if (is_init) {
		if (DATA_DOUBLE == d_type) {
			res = *reinterpret_cast<Num_Type*>(data);
		}
		else
			throw show_err("�������ͣ�",data_to_str(d_type),"��ͼ��ȡʵ������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

void interpret::Interpreter::Base_Data::get_data(Complex_Type & res) const {
	if (is_init) {
		if (DATA_COMPLEX == d_type) {
			res = *reinterpret_cast<Complex_Type*>(data);
		}
		else
			throw show_err("�������ͣ�", data_to_str(d_type), "��ͼ��ȡ��������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

void interpret::Interpreter::Base_Data::get_data(bool & res) const {
	if (is_init) {
		if (DATA_BOOL == d_type) {
			res = *reinterpret_cast<bool*>(data);
		}
		else
			throw show_err("�������ͣ�", data_to_str(d_type), "��ͼ��ȡ�߼���������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

void interpret::Interpreter::Base_Data::get_data(const Array_Data *& res) const {
	if (is_init) {
		if (DATA_ARRAY == d_type) {
			res = reinterpret_cast<const Array_Data *>(data);
		}
		else
			throw show_err("�������ͣ�", data_to_str(d_type), "��ͼ��ȡ��������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

void interpret::Interpreter::Base_Data::get_data(const Matrix_Type *& res) const{
	if (is_init) {
		if (DATA_MATRIX == d_type) {
			res = reinterpret_cast<const Matrix_Type*>(data);
		}
		else
			throw show_err("�������ͣ�", data_to_str(d_type), "��ͼ��ȡ��������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}

void interpret::Interpreter::Base_Data::get_data(const Cmatrix_Type *& res) const{
	if (is_init) {
		if (DATA_CMATRIX == d_type) {
			res = reinterpret_cast<const Cmatrix_Type*>(data);
		}
		else
			throw show_err("�������ͣ�", data_to_str(d_type), "��ͼ��ȡ������������");
	}
	else
		throw show_err("��ͼ��ѯδ��ʼ������ֵ����");
}



interpret::Interpreter::Base_Data::~Base_Data() {
	if (is_init&&is_own)
		_Free();
}

interpret::Ostr_Type& interpret::operator<<(interpret::Ostr_Type&os, const interpret::Interpreter::Base_Data& pData) {
	switch (pData.get_type()) {
	case interpret::Interpreter::DATA_DOUBLE: {
		interpret::Num_Type a;
		pData.get_data(a);
		os << a;
		break;
	}
	case interpret::Interpreter::DATA_COMPLEX: {
		interpret::Complex_Type a;
		pData.get_data(a);
		os << a.real();
		if (a.imag() >= 0)
			os << '+';
		os << a.imag() << "*i";
		break;
	}
	case interpret::Interpreter::DATA_BOOL: {
		bool a;
		pData.get_data(a);
		a ? os << "true" : os << "false";
		break;
	}
	case interpret::Interpreter::DATA_ARRAY: {
		const interpret::Interpreter::Array_Data* a;
		pData.get_data(a);
		os << '(';
		for (auto const& iter : *a) {
			os << iter << ' ';
		}
		os << ')';
		break;
	}
	case interpret::Interpreter::DATA_MATRIX: {
		const interpret::Matrix_Type* a;
		pData.get_data(a);
		for (size_t i = 0; i < a->row(); i++) {
			for (size_t j = 0; j < a->col(); j++) {
				os << a->at(i, j) << ' ';
			}
			os << std::endl;
		}
		break;
	}
	case interpret::Interpreter::DATA_CMATRIX: {
		const interpret::Cmatrix_Type* a;
		pData.get_data(a);
		for (size_t i = 0; i < a->row(); i++) {
			for (size_t j = 0; j < a->col(); j++) {
				Complex_Type pbuf = a->at(i, j);
				os << pbuf.real();
				if (pbuf.imag() >= 0)
					os << '+';
				os << pbuf.imag() << "*i  ";
			}
			os << std::endl;
		}
		break;
	}
	default:
		throw interpret::show_err("�������Ͳ�֧����ʾ");
	}
	return os;
}

//���������ĸ�����
interpret::Interpreter::Base_Item::Base_Item(_My_List * baselist, Base_Type btype, Item_Type itype, int prio) {
	b_type = btype;
	i_type = itype;
	priority = prio;
	base_list = baselist;
	baselist->push_back(this);
	base_Iter = --baselist->end();
	have_master = true;
}

//�޴�������
interpret::Interpreter::Base_Item::Base_Item(Base_Type btype, Item_Type itype, int prio) {
	b_type = btype;
	i_type = itype;
	priority = prio;
	have_master = false;
}

//��ȡ������������
interpret::Interpreter::Base_Type interpret::Interpreter::Base_Item::get_base() {
	return b_type;
}

//��ȡ����Ľڵ�����
interpret::Interpreter::Item_Type interpret::Interpreter::Base_Item::get_item() {
	return i_type;
}

//��ȡ�ڵ��Լ��ĵ�����
interpret::Interpreter::_My_List_Iter interpret::Interpreter::Base_Item::get_iter() {
	if (have_master)
		return base_Iter;
	else
		throw show_err("��ȡ�ĵ�������Ч");
}

interpret::Interpreter::_My_List * interpret::Interpreter::Base_Item::get_master() {
	if (have_master)
		return base_list;
	else
		throw show_err("��ȡ������ʧ��");
}


//�������������Ľڵ��ض����麯��
void interpret::Interpreter::Base_Item::get_data(Base_Data & num_data) {
	throw show_err("��ֵ��ȡ���󣬴���ڵ����ԣ�", item_to_str(i_type));
}

void interpret::Interpreter::Base_Item::get_data(size_t & num) {
	throw show_err("ƫ������ȡ���󣬴���ڵ����ԣ�", item_to_str(i_type));
}

void interpret::Interpreter::Base_Item::get_data(Str_Type & usestr) {
	throw show_err("�ַ�����ȡ���󣬴���ڵ����ԣ�", item_to_str(i_type));
}

void interpret::Interpreter::Base_Item::get_data(_My_List *& uselist) {
	throw show_err("����ָ���ȡ���󣬴���ڵ����ԣ�", item_to_str(i_type));
}

void interpret::Interpreter::Base_Item::get_data(Array_Data *& usevector) {
	throw show_err("����ָ���ȡ���󣬴���ڵ����ԣ�", item_to_str(i_type));
}


int interpret::Interpreter::Base_Item::get_prio() {
	return priority;
}

//���Ľڵ���������ˣ�����뵽�������β��
interpret::Interpreter::_My_List_Iter interpret::Interpreter::Base_Item::change_master(_My_List * uselist) {
	if (have_master)
		base_list->erase(base_Iter);
	else
		have_master = true;
	uselist->push_back(this);
	base_list = uselist;
	base_Iter = --base_list->end();
	return base_Iter;
}

/**
* @brief �ڵ����������
* @param uselist:Ҫ����������ָ��
*		 _My_List_Iter:������λ�ã�����pIter�ĺ���
* @return ��λ�õĵ�����
*/
interpret::Interpreter::_My_List_Iter interpret::Interpreter::Base_Item::change_master(_My_List * uselist, _My_List_Iter pIter) {
	if (have_master)
		base_list->erase(base_Iter);
	else
		have_master = true;
	base_list = uselist;
	base_Iter = uselist->insert(pIter, this);
	return base_Iter;
}

/**
* @brief ��һ���ڵ㣬���Լ���ԭ���������滻�������Լ���Ϊ�����ڵ�
* @param data_item:Ҫ���Լ��滻�����Ľڵ�ָ��
* @return �滻���ԭλ���½ڵ�ĵ�����
*/
interpret::Interpreter::_My_List_Iter interpret::Interpreter::Base_Item::instead(Base_Item * data_item) {//����Ԫ���滻
	if (have_master == false)
		throw show_err("�����ڵ㲻���滻");
	if (data_item->have_master)
		data_item->base_list->erase(data_item->base_Iter);
	data_item->base_list = base_list;
	data_item->base_Iter = base_list->insert(base_list->erase(base_Iter), data_item);
	have_master = false;
	data_item->have_master = true;
	return data_item->base_Iter;
}

interpret::Interpreter::Base_Item::~Base_Item() {
	if (have_master)
		base_list->erase(base_Iter);
}


inline const char * interpret::Interpreter::base_to_str(Base_Type b_type) {
	switch (b_type) {
	case BASE_NUM:
		return "������";
	case BASE_STRING:
		return "�ַ�����";
	case BASE_INT:
		return "������";
	case BASE_EMPTY:
		return "����";
	case BASE_LIST:
		return "������";
	case BASE_VECTOR:
		return "������";
	default:
		return "δ֪����";
	}
}
inline const char * interpret::Interpreter::item_to_str(Item_Type i_type) {
	switch (i_type) {
	case BINARY_OPERATOR:
		return "��Ԫ������";
	case STATIC_FUNCTION:
		return "��̬����";
	case UNARY_OPERATOR:
		return "��Ŀ�����";
	case ASSIGNMENT_OPERATOR:
		return "��ֵ�����";
	case NUM:
		return "��ֵ�ڵ�";
	case NUM_ELEME:
		return "��ǩ��ֵ�ڵ�";
	case NUM_VARIBLE:
		return "����";
	case BRACKET_OPERATOR:
		return "���������";
	case SQ_BEACKET_OPERATOR:
		return "�����������";
	case DIVIDE_OPERATOR:
		return "�����зָ���";
	case SPACE_OPERATOR:
		return "����Ԫ�طָ���";
	case COMMA_OPRERATOR:
		return "���������";
	case UNDEF_NAME:
		return "δ�������ֽڵ�";
	default:
		return "δ֪�ڵ�";
	}
}
inline const char * interpret::Interpreter::data_to_str(Data_Type d_type) {
	switch (d_type) {
	case DATA_ARRAY:
		return "Base_Data����";
	case DATA_BOOL:
		return "bool�߼�����";
	case DATA_CMATRIX:
		return "������������";
	case DATA_COMPLEX:
		return "��������";
	case DATA_DOUBLE:
		return "ʵ������";
	case DATA_MATRIX:
		return "ʵ����������";
	default:
		return "δ֪����";
	}
}

/**
* @brief ��һ������ڵ����ȼ���bracket_prority��1���λ���
* @param uselist:Ҫ���������
* @return ��
*/
void interpret::Interpreter::simply_express(_My_List & uselist) {
	for (int i = bracket_prority; i > 0; i--) {
		Base_Item* pbuf;
		for (_My_List_Iter &&use_iter = uselist.begin(); use_iter != uselist.end();) {
			pbuf = *use_iter;
			if (pbuf->get_prio() == i) {
				use_iter = pbuf->operation();
				delete pbuf;//ִ�в������Ϊ�����ڵ㣬����ɾ�����Ժ���Ҫ�Ľ�
			}
			else {
				use_iter++;
			}
		}
	}
}

/**
* @brief �ݹ�ɾ�������е�Ԫ�ز��ͷ��ڴ�
��Ҫ������������ڴ��ͷ�
* @param uselist:Ҫ��յ�����
* @return ��
*/
void interpret::Interpreter::clear_list(_My_List & uselist) {
	while (!uselist.empty()) {
		delete uselist.front();
	}
}

//��ϵ�������ж��Ƿ�Ϊ���ֻ�����
inline bool interpret::Interpreter::is_num(int c, size_t name_offset) {
	return name_offset ? false : isdigit(c) || c == '.';
}

inline bool interpret::Interpreter::is_name(int c, size_t num_offset) {
	return isalpha(c) || c == '_' || !isascii(c) || (num_offset ? false : isdigit(c));
}

/**
* @brief ���ַ������Ż���ת��Ϊ����
* @param str:Ҫ�������ַ���
buf_list:����������������ڴ洢�����������
* @return ��
*/
size_t interpret::Interpreter::symbol_string(_My_List & buf_list, const Char_Type * str) {
	size_t num_offset = 0;
	size_t name_offset = 0;
	size_t i;
	struct Braket_Status {
		_My_List* pdata;
		enum Status {
			Normal,
			Square
		}state;
		Braket_Status(_My_List*p, Status s):pdata(p),state(s){}
	};
	bool is_space=false;
	size_t length = strlen(str) + 1;
	size_t offset = 0;
	_My_List* main_list = &buf_list;
	
	std::stack<Braket_Status>bracket_stack;
	bracket_stack.push(Braket_Status(main_list, Braket_Status::Normal));
	for (i = 0; i < length; i++) {
		if (!isascii(str[i])) {
			throw show_err("�ݲ�֧�ַ�ascii�ַ�����");
		}
		if (str[i] != ' ' && str[i] != ','&&str[i]!='\t')
			is_space = false;
		if (is_num(str[i], name_offset)) {
			num_offset++;
			continue;
		}
		else if (num_offset != 0) {
			new Noraml_Num(main_list, Base_Data(std::stold(&str[i - num_offset])));
			num_offset = 0;
		}

		if (is_name(str[i], num_offset)) {
			name_offset++;
			continue;
		}
		else if (name_offset != 0) {
			const Str_Type use_name(str, i - name_offset, name_offset);
			Base_Data pnum;

			if ((offset = is_in_store(const_BaseData, use_name)) != const_BaseData.size) {
				new Noraml_Num(main_list, const_BaseData.store[offset].num);
			}
			else if (use_name == result_string) {
				new Noraml_Num(main_list, final_result.get_shadow());
			}
			else if (main_var.find_in(use_name, pnum)) {
				new Varible_Num(main_list, pnum);
			}
			else if ((offset = is_in_store(singlevar_func, use_name)) != singlevar_func.size) {
				new Static_Function(main_list, offset);
			}
			else {
				new Undef_Name(main_list, use_name);
			}
			name_offset = 0;
		}
		//����Ҫ�Ľ�
		if ((offset = is_in_store(binary_func, str[i])) != binary_func.size) {
			new Binary_Operator(main_list, offset);
			continue;
		}
		if ((offset = is_in_store(unary_func, str[i])) != unary_func.size) {
			new Unary_Operator(main_list, offset);
			continue;
		}
		if (str[i] == '(') {
			(new Bracket_Operator(main_list))->get_data(main_list);
			bracket_stack.push(Braket_Status(main_list, Braket_Status::Normal));
			continue;
		}
		if (str[i] == ')') {
			if(bracket_stack.top().state != Braket_Status::Normal)
				throw show_err("���Ż�����'[]'��')'����");
			bracket_stack.pop();
			if (bracket_stack.empty())
				throw show_err("���Ÿ�����ƥ�䣬������̫����");
			main_list = bracket_stack.top().pdata;
			continue;
		}
		if (str[i] == '[') {
			(new SQ_Bracket_Operator(main_list))->get_data(main_list);
			bracket_stack.push(Braket_Status(main_list, Braket_Status::Square));
			continue;
		}
		if (str[i] == ']') {
			if (bracket_stack.top().state != Braket_Status::Square) {
				throw show_err("���Ż�����'[]'��'('����");
			}
			bracket_stack.pop();
			if (bracket_stack.empty())
				throw show_err("���Ÿ�����ƥ�䣬������̫����");
			main_list = bracket_stack.top().pdata;
			continue;
		}
		if (str[i] == '=') {
			new Assignment_Operator(main_list, &main_var);
			continue;
		}
		if (str[i] == ',') {
			if (bracket_stack.top().state == Braket_Status::Normal)
				new Comma_Operator(main_list);
			else {
				if (!is_space) {
					new Space_Opretator(main_list);
					is_space = true;
				}
			}
			continue;
		}
		if (str[i] == '\0') {
			break;
		}
		if (str[i] == ';') {
			if (bracket_stack.top().state == Braket_Status::Square) {
				new Divide_Operator(main_list);
				continue;
			}
			else
				break;
		}
		if (str[i] == '\n') {
			if (bracket_stack.top().state == Braket_Status::Square) {
				new Divide_Operator(main_list);
			}
			continue;
		}
		if (str[i] == ' '||str[i]=='\t') {
			if (bracket_stack.top().state == Braket_Status::Square) {
				if (!is_space) {
					new Space_Opretator(main_list);
					is_space = true;
				}
			}
			continue;
		}
		throw show_err(str[i], "Ϊδ�����ַ�");
	}
	if (main_list != &buf_list)//�����������������һ�£���Ȼ���Ų�ƥ��
		throw show_err("���Ÿ�����ƥ�䣬������̫����");
	return i;
}

/**
* @brief ����һ���ַ���
* @param str:Ҫ�������ַ���
buf_list:����������������ڷ����쳣ʱ
�ں������ͷ�����
* @return ��
*/
void interpret::Interpreter::analyze_string(const Str_Type & str, _My_List & buf_list) {
	const char* p_str = str.c_str();
	size_t offset = 0;
	do {
		offset+=symbol_string(buf_list, p_str + offset)+1;
		simply_express(buf_list);	
		if (buf_list.empty()) {
			continue;
		}
		if (buf_list.size() == 1) {
			Base_Data pres;
			buf_list.front()->get_data(pres);
			final_result = pres.get_real();         //final_result����ֻ����˱������е�Ӱ��
													//���Ϊ��ֹ���������Ӱ������ǿ�Ʊ�Ϊ����
			delete buf_list.front();
		}
		else
			throw show_err("�����ʽ�ڽڵ�����쳣,ʣ��ڵ�ĸ�����", buf_list.size());
	} while (offset<str.size());
}

interpret::Interpreter::Interpreter() {
	is_ok = false;
	final_result = Base_Data(Num_Type(0));
}

interpret::Interpreter::Interpreter(const Interpreter & pInter) {//��ʵûʲô�ã�Ĭ�ϵľ�ok
	final_result = pInter.final_result.get_real();
	is_ok = pInter.is_ok;
	main_var = pInter.main_var;
}


bool interpret::Interpreter::slove(const Str_Type & str) {
	_My_List buf_list;
	try {
		analyze_string(str, buf_list);
		is_ok = true;
	}
	catch (const inter_error& err) {
		*os_err << err.what() << std::endl;
		clear_list(buf_list);
		is_ok = false;
	}
	catch (const std::exception& err) {
		clear_list(buf_list);
		is_ok = false;
		throw err;
	}
	catch (...) {
		clear_list(buf_list);
		is_ok = false;
		throw std::exception("����������δ֪����!!!");
	}
	return is_ok;
}

/**
* @brief ����洢�ı���
* @param ��
* @return ��
*/
void interpret::Interpreter::clear() {
	main_var.clear();
}

std::vector<interpret::Str_Type> interpret::Interpreter::get_const_num() {
	std::vector<interpret::Str_Type> buf;
	buf.push_back(result_string);
	for (size_t i = 0; i < const_BaseData.size; i++) {
		buf.push_back(const_BaseData.store[i].name);
	}
	return buf;
}

/**
* @brief ��ȡ֧�ֵĺ���
* @param ��
* @return Str_Type:��\n����ĺ�������ɵ��ַ���
*/

std::vector<interpret::Str_Type> interpret::Interpreter::get_const_func() {
	std::vector<interpret::Str_Type> buf;
	for (size_t i = 0; i < singlevar_func.size; i++) {
		buf.push_back(singlevar_func.store[i].name);
	}
	return buf;
}

/**
* @brief ��ȡ֧�ֵ������
* @param ��
* @return Str_Type:��\n������������ɵ��ַ���
*/

std::vector<interpret::Char_Type> interpret::Interpreter::get_const_opre() {
	std::vector<interpret::Char_Type> buf;
	for (size_t i = 0; i < binary_func.size; i++) {
		buf.push_back(binary_func.store[i].name);
	}
	for (size_t i = 0; i < unary_func.size; i++) {
		buf.push_back(unary_func.store[i].name);
	}
	return buf;
}


interpret::Interpreter::_My_List_Iter interpret::Interpreter::Binary_Operator::operation(void) {
	Base_Data a, b;
	_My_List_Iter use_iter;
	bool signflag=false;
	if (this == get_master()->back()) //�Ҷ�û�нڵ�
		throw show_err("�����'", binary_func.store[num].name, "'ʹ�ô���");
	Base_Item* node_next = *(++get_iter());		//��ȡ�Ҷ˽ڵ�ָ��
	node_next->get_data(b);						//��ȡ���ݣ�����num�ڵ���Զ��׳��쳣)
	if (binary_func.store[num].name == '-') {
		signflag = true;
	}
	if (this == get_master()->front()) {		//���û�нڵ�
		if (signflag) {//�����Ǹ���
			auto&& res = binary_func.store[is_in_store(binary_func, '*')].func(Base_Data(Num_Type(-1)), b);//���ó˷�����
			use_iter = instead(new Noraml_Num(res));
			delete node_next;
			return use_iter;
		}
		throw show_err("�����'", binary_func.store[num].name, "'ʹ�ô���");
	}
	Base_Item* node_priv = *(--get_iter());		//��ȡ��˵Ľڵ�ָ��
	if (signflag && node_priv->get_base() != BASE_NUM) {//���ܸ���
		auto&& res = binary_func.store[is_in_store(binary_func,'*')].func(Base_Data(Num_Type(-1)), b);//���ó˷�����
		use_iter = instead(new Noraml_Num(res));
		delete node_next;
		return use_iter;
	}
	node_priv->get_data(a);						//��ȡ���ݣ�����num�ڵ���Զ��׳��쳣)
	auto&& res = binary_func.store[num].func(a, b);	//�����ݽ��ж�Ӧ������
	use_iter = instead(new Noraml_Num(res));	//�ü�������num�ڵ��滻���Լ�
	delete node_priv;							//ɾ����˵Ľڵ�
	delete node_next;							//ɾ���Ҷ˵Ľڵ�
	return use_iter;							//�����µĵ�����
}

interpret::Interpreter::_My_List_Iter interpret::Interpreter::Bracket_Operator::operation(void) {
	_My_List_Iter use_iter;
	if (use_list.empty()) {
		return ++get_iter();
	}
	simply_express(use_list);//��������Ľڵ�
	if (use_list.size() == 1) {
		if (this != get_master()->front()) {
			Base_Item* priv = *(--get_iter());
			if (priv->get_base() == BASE_NUM) {
				Base_Data pbuf;
				Base_Data pself;
				use_list.front()->get_data(pself);
				priv->get_data(pbuf);
				switch (pbuf.get_type()) {
				case DATA_ARRAY: {
					const Array_Data* pvec;
					Base_Item* pItem;
					Num_Type poffset;
					pbuf.get_data(pvec);
					pself.get_data(poffset);
					if (size_t(poffset) >= pvec->size()) {
						throw show_err("����Ԫ�ط��ʴ��󣬿ɷ��ʷ�Χ��0~", pvec->size()-1, ",��ͼ���ʵ����꣺", size_t(poffset));
					}
					if (pbuf.is_shadow())
						pItem = new Node_Elem_Num(pvec, Node_Elem_Num::ARRAY_1, size_t(poffset));
					else
						pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::ARRAY_1, size_t(poffset));
					delete priv;
					return instead(pItem);
				}
				case DATA_MATRIX: {
					const Matrix_Type* pmat;
					Base_Item* pItem;
					pbuf.get_data(pmat);
					switch (pself.get_type()) {
					case DATA_DOUBLE: {
						Num_Type poffset;
						pself.get_data(poffset);
						if (size_t(poffset) >= pmat->row()) {
							throw show_err("�������������ʴ��󣬿ɷ��ʷ�Χ��0~", pmat->row()-1, ",��ͼ���ʵ����꣺", size_t(poffset));
						}
						if(pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::MATRIX_1, size_t(poffset));
						else
							pItem = new Node_Elem_Num(std::move(pbuf),Node_Elem_Num::MATRIX_1, size_t(poffset));
						delete priv;
						return instead(pItem);
					}
					case DATA_ARRAY: {
						const Array_Data* pvec;
						pself.get_data(pvec);
						if(pvec->size()!=2)
							throw show_err("���ʾ���Ԫ�ص��±��������");
						Num_Type poffi;
						Num_Type poffj;
						pvec->at(0).get_data(poffi);
						pvec->at(1).get_data(poffj);
						if (pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::MATRIX_2, size_t(poffi), size_t(poffj));
						else
							pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::MATRIX_2, size_t(poffi), size_t(poffj));
						delete priv;
						return instead(pItem);
					}
					default:
						throw show_err("�±���������ֵĶ������޷�����Ϊ�±�");
					}
				}
				case DATA_CMATRIX: {
					const Cmatrix_Type* pmat;
					Base_Item* pItem;
					pbuf.get_data(pmat);
					switch (pself.get_type()) {
					case DATA_DOUBLE: {
						Num_Type poffset;
						pself.get_data(poffset);
						if (size_t(poffset) >= pmat->row()) {
							throw show_err("�������������ʴ��󣬿ɷ��ʷ�Χ��0~", pmat->row() - 1, ",��ͼ���ʵ����꣺", size_t(poffset));
						}
						if (pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::CMATRIX_1, size_t(poffset));
						else
							pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::CMATRIX_1, size_t(poffset));
						delete priv;
						return instead(pItem);
					}
					case DATA_ARRAY: {
						const Array_Data* pvec;
						pself.get_data(pvec);
						if (pvec->size() != 2)
							throw show_err("���ʾ���Ԫ�ص��±��������");
						Num_Type poffi;
						Num_Type poffj;
						pvec->at(0).get_data(poffi);
						pvec->at(1).get_data(poffj);
						if (pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::CMATRIX_2, size_t(poffi), size_t(poffj));
						else
							pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::CMATRIX_2, size_t(poffi), size_t(poffj));
						delete priv;
						return instead(pItem);
					}
					default:
						throw show_err("�±���������ֵĶ������޷�����Ϊ�±�");
					}
				}
				default:
					throw show_err("��ͼʹ��������ʷ�����ͷǾ���ڵ㣡");
				}
			}
		}
		return use_list.front()->change_master(get_master(), get_iter());
	}
	else
		throw show_err("�����ڽڵ�����쳣,ʣ��ڵ������", use_list.size());
}


interpret::Interpreter::_My_List_Iter interpret::Interpreter::Comma_Operator::operation(void) {
	Array_Data pPackage;
	_My_List_Iter comma_iter = get_iter();
	auto next = [](_My_List_Iter pIter) {return ++pIter; };
	auto priv = [](_My_List_Iter pIter) {return --pIter; };
	auto move_to_package = [&pPackage](Base_Item* pItem) {
		Base_Data pdata;
		pItem->get_data(pdata);
		pPackage.push_back(pdata);
		delete pItem;
	};
	move_to_package(*priv(comma_iter));//����ǰ��Ԫ���Ƶ�����
	if (next(comma_iter) == get_master()->end()) {
		throw show_err("����ʹ�ô���");
	}
	move_to_package(*next(comma_iter));//���ź��Ԫ���Ƶ�����
	if (next(comma_iter) == get_master()->end()) {
		return instead(new Noraml_Num(Base_Data(pPackage)));
	}
	comma_iter++;//���Կ��������Ƿ��ж���
	while ((*comma_iter)->get_item() == COMMA_OPRERATOR) {
		if (next(comma_iter) == get_master()->end()) {//��������β�����
			throw show_err("����ʹ�ô���");
		}
		move_to_package(*next(comma_iter));
		if (next(comma_iter) == get_master()->end()) {//�Ƴ���Ԫ�غ���û�ж�����
			delete *comma_iter;//���Լ�ɾ����ֻ����һ�������滻Ԫ�أ������ĳɷֶ�ɾ��
			break;
		}
		comma_iter++;
		delete *priv(comma_iter);
	}
	return instead(new Noraml_Num(Base_Data(pPackage)));
}

interpret::Interpreter::_My_List_Iter interpret::Interpreter::Static_Function::operation(void) {
	Base_Data a;
	_My_List_Iter use_iter;
	if (this == get_master()->back()) {
		throw show_err("'", singlevar_func.store[num].name, "' ��������ȱ�ٲ���");
	}
	Base_Item* node_next = *(++get_iter());
	node_next->get_data(a);
	auto&& res = singlevar_func.store[num].func(a);
	use_iter = instead(new Noraml_Num(res));
	delete node_next;
	return use_iter;
}

interpret::Interpreter::_My_List_Iter interpret::Interpreter::Unary_Operator::operation(void) {
	Base_Data a;
	Complex_Type coma;
	_My_List_Iter use_iter;
	if (this == get_master()->front()) {
		throw show_err("�����'", unary_func.store[num].name, "'ʹ�ô���");
	}
	Base_Item* node_priv = *(--get_iter());
	node_priv->get_data(a);
	auto&& res = unary_func.store[num].func(a);
	use_iter = instead(new Noraml_Num(res));
	delete node_priv;
	return use_iter;
}

/**
* @brief ����������һ���µ�Ԫ��
* @param pstr:��Ԫ�ص�����
pdata:��Ԫ�ص�ֵ
* @return ��Ԫ�����ɵ�һ��Ӱ��
*/

interpret::Interpreter::Base_Data interpret::Interpreter::Variable_Map::push_new(const Str_Type & pstr, const Base_Data & pdata) {
	auto&& ret = var_map.insert(std::make_pair(pstr, pdata.get_real())).first;//����Ҫ�洢��ʵֵ
	const void* use_data;
	ret->second.get_data(use_data);
	var_p_map.insert(std::make_pair(use_data, ret));
	return ret->second.get_shadow();
}

/**
* @brief ���ı������ĳһ��Ԫ�ص�ֵ
* @param psrc:Ҫ���ĵ�Ԫ�صĵ�ַ��ǩ
pdata:Ҫ���ĵ�ֵ
* @return Ԫ�ظ��ĺ����ɵ�Ӱ��
*/

interpret::Interpreter::Base_Data interpret::Interpreter::Variable_Map::data_change(const void * psrc, const Base_Data & pdata) {
	auto&& ret = var_p_map.find(psrc);
	if (ret != var_p_map.end()) {
		const void* pnew;
		_My_Map_Iter pIter = ret->second;
		pIter->second = pdata.get_real();
		pIter->second.get_data(pnew);
		var_p_map.erase(psrc);
		var_p_map.insert(std::make_pair(pnew, pIter));
		return pIter->second.get_shadow();
	}
	else
		throw show_err("δ�ڱ�������Ѱ�ҵ���Ӧ�����ʧ��");
}

/**
* @brief �ڱ�������Ѱ��ָ���ļ�ֵ
* @param pstr:ҪѰ�ҵļ�
pdata:���ɹ�Ѱ�ҵ��ὫѰ�ҵĶ�Ӧֵ��Ӱ�Ӵ��ݹ���
* @return true:��������Ѱ�ҵ��˼�ֵ
false:��������δѰ�ҵ��˼�ֵ
*/

bool interpret::Interpreter::Variable_Map::find_in(const Str_Type & pstr, Base_Data & pdata) {
	auto&& ret = var_map.find(pstr);
	if (ret != var_map.end()) {
		pdata = ret->second.get_shadow();
		return true;
	}
	else
		return false;
}

bool interpret::Interpreter::Variable_Map::find_in(const void * paddr, _My_Map_Iter & pIter){
	auto &&ret = var_p_map.find(paddr);
	if (ret != var_p_map.end()) {
		pIter = ret->second;
		return true;
	}
	else
		return false;
}



/**
* @brief ����������е�����
* @param ��
* @return ��
*/

inline void interpret::Interpreter::Variable_Map::clear(void) {
	var_map.clear();
	var_p_map.clear();
}

interpret::Interpreter::_My_List_Iter interpret::Interpreter::Assignment_Operator::operation(void) {
	if (this == get_master()->front() || this == get_master()->back())//��֤������Ԫ��
		throw show_err("��ֵ�����ʹ�ô���");
	Base_Item* node_priv = *(--get_iter());
	Base_Item* node_next = *(++get_iter());
	Base_Data pnext_num;
	node_next->get_data(pnext_num);
	if (node_priv->get_item() == UNDEF_NAME) {//�����µı���
		Str_Type pstr;
		node_priv->get_data(pstr);
		base_map->push_new(pstr, pnext_num);//����һ���ڵ�
	}
	else if (node_priv->get_item() == NUM_ELEME) {
		reinterpret_cast<Node_Elem_Num*>(node_priv)->assign(pnext_num, base_map);
	}
	else {//�Ա�����ֵ
		Base_Data priv_num;
		const void* priv_data;
		node_priv->get_data(priv_num);				//��ȡ��ֵ�ı���
		priv_num.get_data(priv_data);				//��ȡ��ֵ�ı�����Դ��ַ
		base_map->data_change(priv_data, pnext_num);
	}
	delete node_priv;
	return ++get_iter();
}

interpret::Interpreter& interpret::Interpreter::operator=(const interpret::Interpreter& pInter) {//��ʵûʲô�ã�Ĭ�ϵľ�ok
	if (&pInter == this) {
		return *this;
	}
	final_result = pInter.final_result.get_real();
	is_ok = pInter.is_ok;
	main_var = pInter.main_var;
	return *this;
}

interpret::Interpreter::_My_List_Iter interpret::Interpreter::SQ_Bracket_Operator::operation(void){
	size_t row_num = 0;
	size_t col_num = 0;
	size_t col_finalnum = 0;
	std::vector<Num_Type> num_buf;
	std::vector<Complex_Type> complex_buf;
	_My_List_Iter use_iter;
	bool is_double_num = true;
	if (use_list.empty()) {
		return ++get_iter();
	}
	simply_express(use_list);//��������Ľڵ�
	for (_My_List_Iter pIter = use_list.begin(); pIter != use_list.end(); pIter++) {
		if ((*pIter)->get_item() == DIVIDE_OPERATOR) {
			if (col_num != 0) {
				row_num++;
				if (col_finalnum == 0) {
					col_finalnum = col_num;
				}
				else if (col_finalnum != col_num) {
					throw show_err("����������ͳһ���޷���ʼ��");
				}
				col_num = 0;
			}
		}
		else if ((*pIter)->get_item() == SPACE_OPERATOR) {
			continue;
		}
		else {
			Base_Data pdata;
			(*pIter)->get_data(pdata);
			col_num++;
			switch (pdata.get_type()) {
			case DATA_DOUBLE: {
				Num_Type pres;
				pdata.get_data(pres);
				if (is_double_num) {
					num_buf.push_back(pres);
				}
				else {
					complex_buf.push_back(pres);
				}
				break;
			}
			case DATA_BOOL: {
				bool pres;
				pdata.get_data(pres);
				if (is_double_num) {
					num_buf.push_back(pres);
				}
				else {
					complex_buf.push_back(pres);
				}
				break;
			}
			case DATA_COMPLEX: {
				Complex_Type pres;
				pdata.get_data(pres);
				if (is_double_num) {
					complex_buf.assign(num_buf.begin(), num_buf.end());
					is_double_num = false;
				}
				complex_buf.push_back(pres);
				break;
			}
			default:
				throw show_err("�����ڲ�֧�ִ����������ͣ�", pdata.get_type());
			}
		}
	}
	
	if (col_finalnum!=0 &&col_num != 0 && col_num != col_finalnum) {
		throw show_err("����������ͳһ���޷���ʼ��");
	}
	if (col_finalnum == 0) {
		col_finalnum = col_num;
	}
	if (col_num != 0) {
		row_num++;
	}
	if (is_double_num) {
		size_t buf_size = num_buf.size();
		Matrix_Type pbuf(row_num, col_finalnum);
		for (size_t i = 0; i < buf_size; i++) {
			pbuf.data()[i] = num_buf[i];
		}
		use_iter = instead(new Noraml_Num(Base_Data(pbuf)));
	}
	else {
		size_t buf_size = complex_buf.size();
		Cmatrix_Type pbuf(row_num, col_finalnum);
		for (size_t i = 0; i < buf_size; i++) {
			pbuf.data()[i] = complex_buf[i];
		}
		use_iter = instead(new Noraml_Num(Base_Data(pbuf)));
	}
	return use_iter;
}

void interpret::Interpreter::Node_Elem_Num::assign(Base_Data & num_data, Variable_Map * pmap) {
	switch (type) {
	case ARRAY_1:
		reinterpret_cast<Array_Data*>(paddr)->at(xpos) = num_data.get_real();
		break;
	case MATRIX_1: {
		const auto&& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		if (num_data.get_type() == DATA_MATRIX) {
			const Matrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("��������ֵʧ�ܣ�ά����ͳһ");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->at(xpos, i) = rmat->at(0, i);
			}
		}
		else {
			const Cmatrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("��������ֵʧ�ܣ�ά����ͳһ");
			}
			Cmatrix_Type pbuf(lmat->row(),lmat->col());
			for (size_t i = 0; i < lmat->col()*lmat->row(); i++) {
				pbuf.data()[i] = lmat->data()[i];
			}
			for (size_t i = 0; i < lmat->col(); i++) {
				pbuf.at(xpos,i) = rmat->at(0, i);
			}
			pmap->data_change(paddr, Base_Data(pbuf));
		}
		break;
	}
	case CMATRIX_1: {
		const auto&& lmat = reinterpret_cast<Cmatrix_Type*>(paddr);
		if (num_data.get_type() == DATA_MATRIX) {
			const Matrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("��������ֵʧ�ܣ�ά����ͳһ");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->at(xpos, i) = rmat->at(0, i);
			}
		}
		else {
			const Cmatrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("��������ֵʧ�ܣ�ά����ͳһ");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->at(xpos, i) = rmat->at(0, i);
			}
		}
		break;
	}
	case MATRIX_2: {
		const auto&& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		if (num_data.get_type() == DATA_DOUBLE) {
			Num_Type rnum;
			num_data.get_data(rnum);
			lmat->at(xpos, ypos) = rnum;
		}
		else if (num_data.get_type() == DATA_BOOL) {
			bool rnum;
			num_data.get_data(rnum);
			lmat->at(xpos, ypos) = Num_Type(rnum);
		}
		else {
			Complex_Type rnum;
			num_data.get_data(rnum);
			Cmatrix_Type pbuf(lmat->row(),lmat->col());
			for (size_t i = 0; i < lmat->col()*lmat->row(); i++) {
				pbuf.data()[i] = lmat->data()[i];
			}
			pbuf.at(xpos,ypos) = rnum;
			pmap->data_change(paddr, Base_Data(pbuf));
		}
		break;
	}
	case CMATRIX_2: {
		const auto&& lmat = reinterpret_cast<Cmatrix_Type*>(paddr);
		if (num_data.get_type() == DATA_DOUBLE) {
			Num_Type rnum;
			num_data.get_data(rnum);
			lmat->at(xpos, ypos) = rnum;
		}
		else if (num_data.get_type() == DATA_BOOL) {
			bool rnum;
			num_data.get_data(rnum);
			lmat->at(xpos, ypos) = Num_Type(rnum);
		}
		else {
			Complex_Type rnum;
			num_data.get_data(rnum);
			lmat->at(xpos, ypos) = rnum;
		}
		break;
	}
	default:
		throw show_err("δ֪����", __LINE__);
		break;
	}
}

void interpret::Interpreter::Node_Elem_Num::get_data(Base_Data & num_data) {
	switch (type) {
	case ARRAY_1: {
		num_data = reinterpret_cast<Array_Data*>(paddr)->at(xpos);
		break;
	}
	case MATRIX_1: {
		const auto&& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		size_t lcol = lmat->col();
		Matrix_Type pbuf(1, lcol);
		for (size_t i = 0; i < lcol; i++) {
			pbuf.at(0,i) = lmat->at(xpos, i);
		}
		num_data = Base_Data(pbuf);
		break;
	}
	case CMATRIX_1: {
		const auto&& lmat = reinterpret_cast<Cmatrix_Type*>(paddr);
		size_t lcol = lmat->col();
		Cmatrix_Type pbuf(1, lcol);
		for (size_t i = 0; i < lcol; i++) {
			pbuf.at(0,i) = lmat->at(xpos, i);
		}
		num_data = Base_Data(pbuf);
		break;
	}
	case MATRIX_2: {
		num_data = Base_Data(reinterpret_cast<Matrix_Type*>(paddr)->at(xpos, ypos));
		break;
	}
	case CMATRIX_2: {
		num_data = Base_Data(reinterpret_cast<Cmatrix_Type*>(paddr)->at(xpos, ypos));
		break;
	}
	default:
		throw show_err("δ֪����",__LINE__);
	}
}
