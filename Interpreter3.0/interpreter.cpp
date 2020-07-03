#include "interpreter.h"


//分配一块由pData初始化的内存
inline void inter::Interpreter::Base_Data::_Malloc(const Base_Data & pData) {
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
		data = new _Data_Array(*reinterpret_cast<_Data_Array*>(pData.data));
		for (size_t i = 0; i < reinterpret_cast<_Data_Array*>(data)->size(); i++) {
			if (reinterpret_cast<_Data_Array*>(data)->at(i).is_shadow()) {
				reinterpret_cast<_Data_Array*>(data)->at(i) = reinterpret_cast<_Data_Array*>(data)->at(i).get_real();
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
		throw show_err("数据申请错误");
	}
}


//释放所分配的数据
inline void inter::Interpreter::Base_Data::_Free(void) {
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
		delete reinterpret_cast<_Data_Array*>(data);
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

inter::Interpreter::Base_Data::Base_Data() {
	data = nullptr;
	is_own = false;
	is_init = false;
}

inter::Interpreter::Base_Data::Base_Data(const Num_Type & res) {
	data = new Num_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_DOUBLE;
}

inter::Interpreter::Base_Data::Base_Data(const Complex_Type & res) {
	data = new Complex_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_COMPLEX;
}

inter::Interpreter::Base_Data::Base_Data(const bool & res) {
	data = new bool(res);
	is_own = true;
	is_init = true;
	d_type = DATA_BOOL;
}

inter::Interpreter::Base_Data::Base_Data(const _Data_Array & res) {
	data = new _Data_Array(res);
	is_own = true;
	is_init = true;
	d_type = DATA_ARRAY;
}

inter::Interpreter::Base_Data::Base_Data(const Matrix_Type &res){
	data = new Matrix_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_MATRIX;
}

inter::Interpreter::Base_Data::Base_Data(const Cmatrix_Type &res){
	data = new Cmatrix_Type(res);
	is_own = true;
	is_init = true;
	d_type = DATA_CMATRIX;
}

inter::Interpreter::Base_Data::Base_Data(Matrix_Type &&res){
	data = new Matrix_Type;
	(*reinterpret_cast<Matrix_Type*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_MATRIX;
}

inter::Interpreter::Base_Data::Base_Data(Cmatrix_Type && res){
	data = new Cmatrix_Type;
	(*reinterpret_cast<Cmatrix_Type*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_CMATRIX;
}

inter::Interpreter::Base_Data::Base_Data(_Data_Array && res){
	data = new _Data_Array;
	(*reinterpret_cast<_Data_Array*>(data)) = std::move(res);
	is_own = true;
	is_init = true;
	d_type = DATA_ARRAY;
}



/**
* @brief 获得本数据的一个影子
* @param 无
* @return 返回一个属性为影子的Base_Data
*/
inter::Interpreter::Base_Data inter::Interpreter::Base_Data::get_shadow() const {
	Base_Data pData;
	pData.data = data;
	pData.d_type = d_type;
	pData.is_init = is_init;
	pData.is_own = false;
	return pData;
}


/**
* @brief 获得本数据的本体
注意获得的本体为一个全新的本体，与原有的本体仅数据存储的值相同
其余再无任何关系
* @param 无
* @return 返回一个属性为本体的Base_Data
*/
inter::Interpreter::Base_Data inter::Interpreter::Base_Data::get_real() const {
	Base_Data pData;
	if (is_init) {
		pData.d_type = d_type;
		pData.is_init = true;
		pData.is_own = true;
		pData._Malloc(*this);
	}
	return pData;
}

inter::Interpreter::Base_Data::Base_Data(const Base_Data & pData) {
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

bool inter::Interpreter::Base_Data::operator<(const Base_Data &pdata)const{
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
			throw show_err("数据类型不支持比较大小");
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
			throw show_err("数据类型不支持比较大小");
		}
	}
	default:
		throw show_err("数据类型不支持比较大小");
	}
	}
}

inter::Interpreter::Base_Data::Base_Data(Base_Data && pData) {
	d_type = pData.d_type;
	is_own = pData.is_own;
	is_init = pData.is_init;
	data = pData.data;
	pData.is_own = false;
}

inter::Interpreter::Base_Data& inter::Interpreter::Base_Data::operator=(const Base_Data& pData) {
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

inter::Interpreter::Base_Data& inter::Interpreter::Base_Data::operator=(Base_Data&& pData) {
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
* @brief 判断是否为影子
* @param 无
* @return bool
*		-<em>false</em> 不是影子
*		-<em>true</em>	是影子
*/
bool inter::Interpreter::Base_Data::is_shadow() const {
	if (is_init)
		return !is_own;
	else
		throw show_err("试图查询未初始化的数值类型");
}

/**
* @brief 获取真实存储的数据类型
主要在编写数学函数时使用，属于底层操作
* @param 无
* @return Date_Type
*		DATA_DOUBLE		实数
*		DATA_COMPLEX	复数
*/
inter::Interpreter::Data_Type inter::Interpreter::Base_Data::get_type() const {
	if (is_init)
		return d_type;
	else
		throw show_err("试图查询未初始化的数值类型");
}

//获取内部存储的数据源地址
void inter::Interpreter::Base_Data::get_data(void *& p) const {	//此处获取源地址是为了让我们可以通过
	if (is_init)				//影子可以寻找到本体，影子与本体的唯一
		p = data;				//联系只有这个地址。
	else
		throw show_err("试图查询未初始化的存储地址");
}

void inter::Interpreter::Base_Data::get_data(Num_Type & res) const {
	if (is_init) {
		if (DATA_DOUBLE == d_type) {
			res = *reinterpret_cast<Num_Type*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}

void inter::Interpreter::Base_Data::get_data(Complex_Type & res) const {
	if (is_init) {
		if (DATA_COMPLEX == d_type) {
			res = *reinterpret_cast<Complex_Type*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}

void inter::Interpreter::Base_Data::get_data(bool & res) const {
	if (is_init) {
		if (DATA_BOOL == d_type) {
			res = *reinterpret_cast<bool*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}

void inter::Interpreter::Base_Data::get_data(_Data_Array *& res) const {
	if (is_init) {
		if (DATA_ARRAY == d_type) {
			res = reinterpret_cast<_Data_Array*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}

void inter::Interpreter::Base_Data::get_data(Matrix_Type *& res) const{
	if (is_init) {
		if (DATA_MATRIX == d_type) {
			res = reinterpret_cast<Matrix_Type*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}

void inter::Interpreter::Base_Data::get_data(Cmatrix_Type *& res) const{
	if (is_init) {
		if (DATA_CMATRIX == d_type) {
			res = reinterpret_cast<Cmatrix_Type*>(data);
		}
		else
			throw show_err("数据类型错误，格式不匹配");
	}
	else
		throw show_err("试图查询未初始化的数值类型");
}



inter::Interpreter::Base_Data::~Base_Data() {
	if (is_init&&is_own)
		_Free();
}

inter::Ostr_Type& inter::operator<<(inter::Ostr_Type&os, const inter::Interpreter::Base_Data& pData) {
	switch (pData.get_type()) {
	case inter::Interpreter::DATA_DOUBLE: {
		inter::Num_Type a;
		pData.get_data(a);
		os << a;
		break;
	}
	case inter::Interpreter::DATA_COMPLEX: {
		inter::Complex_Type a;
		pData.get_data(a);
		os << a.real();
		if (a.imag() >= 0)
			os << '+';
		os << a.imag() << "*i";
		break;
	}
	case inter::Interpreter::DATA_BOOL: {
		bool a;
		pData.get_data(a);
		a ? os << "true" : os << "false";
		break;
	}
	case inter::Interpreter::DATA_ARRAY: {
		inter::Interpreter::_Data_Array* a;
		pData.get_data(a);
		os << '(';
		for (auto const& iter : *a) {
			os << iter << ' ';
		}
		os << ')';
		break;
	}
	case inter::Interpreter::DATA_MATRIX: {
		inter::Matrix_Type* a;
		pData.get_data(a);
		for (size_t i = 0; i < a->row(); i++) {
			for (size_t j = 0; j < a->col(); j++) {
				os << a->loc(i, j) << ' ';
			}
			os << std::endl;
		}
		break;
	}
	case inter::Interpreter::DATA_CMATRIX: {
		inter::Cmatrix_Type* a;
		pData.get_data(a);
		for (size_t i = 0; i < a->row(); i++) {
			for (size_t j = 0; j < a->col(); j++) {
				Complex_Type pbuf = a->loc(i, j);
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
		throw inter::show_err("数据类型不支持显示");
	}
	return os;
}

//给出从属的根链表级
inter::Interpreter::Base_Item::Base_Item(_My_List * baselist, Base_Type btype, Item_Type itype, int prio) {
	b_type = btype;
	i_type = itype;
	priority = prio;
	base_list = baselist;
	baselist->push_back(this);
	base_Iter = --baselist->end();
	have_master = true;
}

//无从属链表
inter::Interpreter::Base_Item::Base_Item(Base_Type btype, Item_Type itype, int prio) {
	b_type = btype;
	i_type = itype;
	priority = prio;
	have_master = false;
}

//获取基础数据类型
inter::Interpreter::Base_Type inter::Interpreter::Base_Item::get_base() {
	return b_type;
}

//获取具体的节点类型
inter::Interpreter::Item_Type inter::Interpreter::Base_Item::get_item() {
	return i_type;
}

//获取节点自己的迭代器
inter::Interpreter::_My_List_Iter inter::Interpreter::Base_Item::get_iter() {
	if (have_master)
		return base_Iter;
	else
		throw show_err("获取的迭代器无效");
}

inter::Interpreter::_My_List * inter::Interpreter::Base_Item::get_master() {
	if (have_master)
		return base_list;
	else
		throw show_err("获取主链表失败");
}


//留待符合条件的节点重定义虚函数
void inter::Interpreter::Base_Item::get_data(Base_Data & num_data) {
	throw show_err("数值获取错误，错误节点属性：", i_type);
}

void inter::Interpreter::Base_Item::get_data(size_t & num) {
	throw show_err("偏移量获取错误，错误节点属性：", i_type);
}

void inter::Interpreter::Base_Item::get_data(Str_Type & usestr) {
	throw show_err("字符串获取错误，错误节点属性：", i_type);
}

void inter::Interpreter::Base_Item::get_data(_My_List *& uselist) {
	throw show_err("链表指针获取错误，错误节点属性：", i_type);
}

void inter::Interpreter::Base_Item::get_data(_Data_Array *& usevector) {
	throw show_err("向量指针获取错误，错误节点属性：", i_type);
}


int inter::Interpreter::Base_Item::get_prio() {
	return priority;
}

//更改节点的链表主人，会插入到新链表的尾部
inter::Interpreter::_My_List_Iter inter::Interpreter::Base_Item::change_master(_My_List * uselist) {
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
* @brief 节点更换主链表
* @param uselist:要更换的链表指针
*		 _My_List_Iter:更换的位置，插入pIter的后面
* @return 新位置的迭代器
*/
inter::Interpreter::_My_List_Iter inter::Interpreter::Base_Item::change_master(_My_List * uselist, _My_List_Iter pIter) {
	if (have_master)
		base_list->erase(base_Iter);
	else
		have_master = true;
	base_list = uselist;
	base_Iter = uselist->insert(pIter, this);
	return base_Iter;
}

/**
* @brief 用一个节点，将自己从原来的链表处替换下来，自己变为无主节点
* @param data_item:要将自己替换下来的节点指针
* @return 替换完后原位置新节点的迭代器
*/
inter::Interpreter::_My_List_Iter inter::Interpreter::Base_Item::instead(Base_Item * data_item) {//链表元素替换
	if (have_master == false)
		throw show_err("无主节点不可替换");
	if (data_item->have_master)
		data_item->base_list->erase(data_item->base_Iter);
	data_item->base_list = base_list;
	data_item->base_Iter = base_list->insert(base_list->erase(base_Iter), data_item);
	have_master = false;
	data_item->have_master = true;
	return data_item->base_Iter;
}

inter::Interpreter::Base_Item::~Base_Item() {
	if (have_master)
		base_list->erase(base_Iter);
}

/**
* @brief 对一个链表节点优先级从bracket_prority到1依次化简
* @param uselist:要化简的链表
* @return 无
*/
void inter::Interpreter::simply_express(_My_List & uselist) {
	for (int i = bracket_prority; i > 0; i--) {
		Base_Item* pbuf;
		for (_My_List_Iter &&use_iter = uselist.begin(); use_iter != uselist.end();) {
			pbuf = *use_iter;
			if (pbuf->get_prio() == i) {
				use_iter = pbuf->operation();
				delete pbuf;//执行操作后变为无主节点，可以删除，以后考虑要改进
			}
			else {
				use_iter++;
			}
		}
	}
}

/**
* @brief 递归删除链表中的元素并释放内存
主要用作错误处理的内存释放
* @param uselist:要清空的链表
* @return 无
*/
void inter::Interpreter::clear_list(_My_List & uselist) {
	while (!uselist.empty()) {
		delete uselist.front();
	}
}

//联系上下文判断是否为数字或名字
inline bool inter::Interpreter::is_num(int c, size_t name_offset) {
	return name_offset ? false : isdigit(c) || c == '.';
}

inline bool inter::Interpreter::is_name(int c, size_t num_offset) {
	return isalpha(c) || c == '_' || !isascii(c) || (num_offset ? false : isdigit(c));
}

/**
* @brief 将字符串符号化，转变为链表
* @param str:要分析的字符串
buf_list:用来缓存的链表，用于存储分析后的链表
* @return 无
*/
size_t inter::Interpreter::symbol_string(_My_List & buf_list, const Char_Type * str) {
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
	size_t length = strlen(str) + 1;
	size_t offset = 0;
	_My_List* main_list = &buf_list;
	
	std::stack<Braket_Status>bracket_stack;
	bracket_stack.push(Braket_Status(main_list, Braket_Status::Normal));
	for (i = 0; i < length; i++) {
		if (!isascii(str[i])) {
			throw show_err("暂不支持非ascii字符输入");
		}
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

			if ((offset = is_in_store(const_BaseData, use_name)) != const_BaseData_num) {
				new Noraml_Num(main_list, const_BaseData[offset].num);
			}
			else if (use_name == result_string) {
				new Noraml_Num(main_list, final_result.get_shadow());
			}
			else if (main_var.find_in(use_name, pnum)) {
				new Varible_Num(main_list, pnum);
			}
			else if ((offset = is_in_store(singlevar_func, use_name)) != singlevar_func_num) {
				new Static_Function(main_list, offset);
			}
			else {
				new Undef_Name(main_list, use_name);
			}
			name_offset = 0;
		}
		//这里要改进
		if ((offset = is_in_store(binary_func, str[i])) != binary_func_num) {
			new Binary_Operator(main_list, offset);
			continue;
		}
		if ((offset = is_in_store(unary_func, str[i])) != unary_func_num) {
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
				throw show_err("括号混淆：'[]'中')'过多");
			bracket_stack.pop();
			if (bracket_stack.empty())
				throw show_err("括号个数不匹配，右括号太多了");
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
				throw show_err("括号混淆：'[]'中'('过多");
			}
			bracket_stack.pop();
			if (bracket_stack.empty())
				throw show_err("括号个数不匹配，右括号太多了");
			main_list = bracket_stack.top().pdata;
			continue;
		}
		if (str[i] == '=') {
			new Assignment_Operator(main_list, &main_var);
			continue;
		}
		if (str[i] == ',') {
			if(bracket_stack.top().state == Braket_Status::Normal)
				new Comma_Operator(main_list);
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
				continue;
			}
			else
				continue;
		}
		if (str[i] == ' ')
			continue;
		throw show_err(str[i], "为未定义字符");
	}
	if (main_list != &buf_list)//主链表与基础的链表不一致，显然括号不匹配
		throw show_err("括号个数不匹配，左括号太多了");
	return i;
}

/**
* @brief 分析一段字符串
* @param str:要分析的字符串
buf_list:用来缓存的链表，用于发生异常时
在函数外释放数据
* @return 无
*/
void inter::Interpreter::analyze_string(const Str_Type & str, _My_List & buf_list) {
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
			final_result = pres.get_real();         //final_result可能只获得了变量表中的影子
													//因此为防止变量表清除影响结果，强制变为本体
			delete buf_list.front();
		}
		else
			throw show_err("主表达式内节点个数异常,剩余节点的个数：", buf_list.size());
	} while (offset<str.size());
}

inter::Interpreter::Interpreter() {
	is_ok = false;
	final_result = Base_Data(Num_Type(0));
}

inter::Interpreter::Interpreter(const Interpreter & pInter) {//其实没什么用，默认的就ok
	final_result = pInter.final_result.get_real();
	is_ok = pInter.is_ok;
	main_var = pInter.main_var;
}


bool inter::Interpreter::slove(const Str_Type & str) {
	_My_List buf_list;
	try {
		analyze_string(str, buf_list);
		is_ok = true;
	}
	catch (std::logic_error&err) {
		*os_err << err.what() << std::endl;
		clear_list(buf_list);
		final_result = Base_Data(std::numeric_limits<Num_Type>::quiet_NaN());
		is_ok = true;
	}
	catch (inter_error& err) {
		*os_err << err.what() << std::endl;
		clear_list(buf_list);
		is_ok = false;
	}
	catch (std::exception& err) {
		*os_err << err.what() << std::endl;
		is_ok = false;
	}
	return is_ok;
}

/**
* @brief 清除存储的变量
* @param 无
* @return 无
*/
void inter::Interpreter::clear() {
	main_var.clear();
}

inter::Str_Type inter::Interpreter::get_const_num() {
	Str_Type buf;
	buf += result_string;
	buf += '\n';
	for (const auto &pIter : const_BaseData) {
		buf += pIter.name;
		buf += '\n';
	}
	return buf;
}

/**
* @brief 获取支持的函数
* @param 无
* @return Str_Type:由\n间隔的函数名组成的字符串
*/

inter::Str_Type inter::Interpreter::get_const_func() {
	Str_Type buf;
	for (const auto &pIter : singlevar_func) {
		buf += pIter.name;
		buf += '\n';
	}
	return buf;
}

/**
* @brief 获取支持的运算符
* @param 无
* @return Str_Type:由\n间隔的运算符组成的字符串
*/

inter::Str_Type inter::Interpreter::get_const_opre() {
	Str_Type buf;
	for (const auto &pIter : binary_func) {
		buf += pIter.name;
		buf += '\n';
	}
	for (const auto &pIter : unary_func) {
		buf += pIter.name;
		buf += '\n';
	}
	return buf;
}

/**
* @brief 重置错误输出流
* @param err：新的错误流
* @return 无
*/

inter::Interpreter::_My_List_Iter inter::Interpreter::Binary_Operator::operation(void) {
	Base_Data a, b;
	_My_List_Iter use_iter;
	if (this == get_master()->back()) //右端没有节点
		throw show_err("运算符'", binary_func[num].name, "'使用错误！");
	Base_Item* node_next = *(++get_iter());		//获取右端节点指针
	node_next->get_data(b);						//获取数据（不是num节点会自动抛出异常)
	if (this == get_master()->front()) {		//左端没有节点
		if (binary_func[num].name == '+' || binary_func[num].name == '-') {//可能是正负号
			auto&& res = binary_func[num].func(Base_Data(Num_Type(0)), b);
			use_iter = instead(new Noraml_Num(res));
			delete node_next;
			return use_iter;
		}
		throw show_err("运算符'", binary_func[num].name, "'使用错误！");
	}
	Base_Item* node_priv = *(--get_iter());		//获取左端的节点指针
	node_priv->get_data(a);						//获取数据（不是num节点会自动抛出异常)
	auto&& res = binary_func[num].func(a, b);	//对数据进行对应的运算
	use_iter = instead(new Noraml_Num(res));		//用计算结果的num节点替换掉自己
	delete node_priv;							//删除左端的节点
	delete node_next;							//删除右端的节点
	return use_iter;							//返回新的迭代器
}

inter::Interpreter::_My_List_Iter inter::Interpreter::Bracket_Operator::operation(void) {
	_My_List_Iter use_iter;
	if (use_list.empty()) {
		return ++get_iter();
	}
	simply_express(use_list);//化简自身的节点
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
					_Data_Array* pvec;
					Base_Item* pItem;
					Num_Type poffset;
					pbuf.get_data(pvec);
					pself.get_data(poffset);
					if (size_t(poffset) >= pvec->size()) {
						throw show_err("数组元素访问错误，可访问范围：0~", pvec->size()-1, ",试图访问的坐标：", size_t(poffset));
					}
					if (pbuf.is_shadow())
						pItem = new Node_Elem_Num(pvec, Node_Elem_Num::ARRAY_1, size_t(poffset));
					else
						pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::ARRAY_1, size_t(poffset));
					delete priv;
					return instead(pItem);
				}
				case DATA_MATRIX: {
					Matrix_Type* pmat;
					Base_Item* pItem;
					pbuf.get_data(pmat);
					switch (pself.get_type()) {
					case DATA_DOUBLE: {
						Num_Type poffset;
						pself.get_data(poffset);
						if (size_t(poffset) >= pmat->row()) {
							throw show_err("矩阵行向量访问错误，可访问范围：0~", pmat->row()-1, ",试图访问的坐标：", size_t(poffset));
						}
						if(pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::MATRIX_1, size_t(poffset));
						else
							pItem = new Node_Elem_Num(std::move(pbuf),Node_Elem_Num::MATRIX_1, size_t(poffset));
						delete priv;
						return instead(pItem);
					}
					case DATA_ARRAY: {
						_Data_Array* pvec;
						pself.get_data(pvec);
						if(pvec->size()!=2)
							throw show_err("访问矩阵元素的下标个数错误！");
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
						throw show_err("下标里混进了奇怪的东西，无法解析为下标");
					}
				}
				case DATA_CMATRIX: {
					Cmatrix_Type* pmat;
					Base_Item* pItem;
					pbuf.get_data(pmat);
					switch (pself.get_type()) {
					case DATA_DOUBLE: {
						Num_Type poffset;
						pself.get_data(poffset);
						if (size_t(poffset) >= pmat->row()) {
							throw show_err("矩阵行向量访问错误，可访问范围：0~", pmat->row() - 1, ",试图访问的坐标：", size_t(poffset));
						}
						if (pbuf.is_shadow())
							pItem = new Node_Elem_Num(pmat, Node_Elem_Num::CMATRIX_1, size_t(poffset));
						else
							pItem = new Node_Elem_Num(std::move(pbuf), Node_Elem_Num::CMATRIX_1, size_t(poffset));
						delete priv;
						return instead(pItem);
					}
					case DATA_ARRAY: {
						_Data_Array* pvec;
						pself.get_data(pvec);
						if (pvec->size() != 2)
							throw show_err("访问矩阵元素的下标个数错误！");
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
						throw show_err("下标里混进了奇怪的东西，无法解析为下标");
					}
				}
				default:
					throw show_err("试图使用坐标访问非数组和非矩阵节点！");
				}
			}
		}
		return use_list.front()->change_master(get_master(), get_iter());
	}
	else
		throw show_err("括号内节点个数异常,剩余节点个数：", use_list.size());
}


inter::Interpreter::_My_List_Iter inter::Interpreter::Comma_Operator::operation(void) {
	_Data_Array pPackage;
	_My_List_Iter comma_iter = get_iter();
	auto next = [](_My_List_Iter pIter) {return ++pIter; };
	auto priv = [](_My_List_Iter pIter) {return --pIter; };
	auto move_to_package = [&pPackage](Base_Item* pItem) {
		Base_Data pdata;
		pItem->get_data(pdata);
		pPackage.push_back(pdata);
		delete pItem;
	};
	move_to_package(*priv(comma_iter));//逗号前的元素移到包内
	if (next(comma_iter) == get_master()->end()) {
		throw show_err("逗号使用错误！");
	}
	move_to_package(*next(comma_iter));//逗号后的元素移到包内
	if (next(comma_iter) == get_master()->end()) {
		return instead(new Noraml_Num(Base_Data(pPackage)));
	}
	comma_iter++;//尝试看看后面是否还有逗号
	while ((*comma_iter)->get_item() == COMMA_OPRERATOR) {
		if (next(comma_iter) == get_master()->end()) {//逗号做结尾的情况
			throw show_err("逗号使用错误！");
		}
		move_to_package(*next(comma_iter));
		if (next(comma_iter) == get_master()->end()) {//移除完元素后面没有逗号了
			delete *comma_iter;//把自己删掉，只保留一个用来替换元素，其他的成分都删掉
			break;
		}
		comma_iter++;
		delete *priv(comma_iter);
	}
	return instead(new Noraml_Num(Base_Data(pPackage)));
}

inter::Interpreter::_My_List_Iter inter::Interpreter::Static_Function::operation(void) {
	Base_Data a;
	_My_List_Iter use_iter;
	if (this == get_master()->back()) {
		throw show_err("'", singlevar_func[num].name, "' 函数后面缺少参数");
	}
	Base_Item* node_next = *(++get_iter());
	node_next->get_data(a);
	auto&& res = singlevar_func[num].func(a);
	use_iter = instead(new Noraml_Num(res));
	delete node_next;
	return use_iter;
}

inter::Interpreter::_My_List_Iter inter::Interpreter::Unary_Operator::operation(void) {
	Base_Data a;
	Complex_Type coma;
	_My_List_Iter use_iter;
	if (this == get_master()->front()) {
		throw show_err("运算符'", unary_func[num].name, "'使用错误！");
	}
	Base_Item* node_priv = *(--get_iter());
	node_priv->get_data(a);
	auto&& res = unary_func[num].func(a);
	use_iter = instead(new Noraml_Num(res));
	delete node_priv;
	return use_iter;
}

/**
* @brief 向变量表添加一个新的元素
* @param pstr:新元素的名称
pdata:新元素的值
* @return 新元素生成的一个影子
*/

inter::Interpreter::Base_Data inter::Interpreter::Variable_Map::push_new(const Str_Type & pstr, const Base_Data & pdata) {
	auto&& ret = var_map.insert(std::make_pair(pstr, pdata.get_real())).first;//必须要存储真实值
	void* use_data;
	ret->second.get_data(use_data);
	var_p_map.insert(std::make_pair(use_data, ret));
	return ret->second.get_shadow();
}

/**
* @brief 更改变量表的某一个元素的值
* @param psrc:要更改的元素的地址标签
pdata:要更改的值
* @return 元素更改后生成的影子
*/

inter::Interpreter::Base_Data inter::Interpreter::Variable_Map::data_change(void * psrc, const Base_Data & pdata) {
	auto&& ret = var_p_map.find(psrc);
	if (ret != var_p_map.end()) {
		void* pnew;
		_My_Map_Iter pIter = ret->second;
		pIter->second = std::move(pdata.get_real());
		pIter->second.get_data(pnew);
		var_p_map.erase(psrc);
		var_p_map.insert(std::make_pair(pnew, pIter));
		return pIter->second.get_shadow();
	}
	else
		throw show_err("未在变量表中寻找到对应项，更改失败");
}

/**
* @brief 在变量表中寻找指定的键值
* @param pstr:要寻找的键
pdata:若成功寻找到会将寻找的对应值的影子传递过来
* @return true:变量表中寻找到了键值
false:变量表中未寻找到了键值
*/

bool inter::Interpreter::Variable_Map::find_in(const Str_Type & pstr, Base_Data & pdata) {
	auto&& ret = var_map.find(pstr);
	if (ret != var_map.end()) {
		pdata = ret->second.get_shadow();
		return true;
	}
	else
		return false;
}

bool inter::Interpreter::Variable_Map::find_in(void * paddr, _My_Map_Iter & pIter)
{
	auto &&ret = var_p_map.find(paddr);
	if (ret != var_p_map.end()) {
		pIter = ret->second;
		return true;
	}
	else
		return false;
}



/**
* @brief 清除变量表中的数据
* @param 无
* @return 无
*/

inline void inter::Interpreter::Variable_Map::clear(void) {
	var_map.clear();
	var_p_map.clear();
}

inter::Interpreter::_My_List_Iter inter::Interpreter::Assignment_Operator::operation(void) {
	if (this == get_master()->front() || this == get_master()->back())//保证两边有元素
		throw show_err("赋值运算符使用错误！");
	Base_Item* node_priv = *(--get_iter());
	Base_Item* node_next = *(++get_iter());
	Base_Data pnext_num;
	node_next->get_data(pnext_num);
	if (node_priv->get_item() == UNDEF_NAME) {//创建新的变量
		Str_Type pstr;
		node_priv->get_data(pstr);
		base_map->push_new(pstr, pnext_num);//新增一个节点
	}
	else if (node_priv->get_item() == NUM_ELEME) {
		reinterpret_cast<Node_Elem_Num*>(node_priv)->assign(pnext_num, base_map);
	}
	else {//对变量赋值
		Base_Data priv_num;
		void* priv_data;
		node_priv->get_data(priv_num);				//获取左值的变量
		priv_num.get_data(priv_data);				//获取左值的变量的源地址
		base_map->data_change(priv_data, pnext_num);
	}
	delete node_priv;
	return ++get_iter();
}

inter::Interpreter& inter::Interpreter::operator=(const inter::Interpreter& pInter) {//其实没什么用，默认的就ok
	if (&pInter == this) {
		return *this;
	}
	final_result = pInter.final_result.get_real();
	is_ok = pInter.is_ok;
	main_var = pInter.main_var;
	return *this;
}

inter::Interpreter::_My_List_Iter inter::Interpreter::SQ_Bracket_Operator::operation(void){
	size_t row_num = 0;
	size_t col_num = 0;
	size_t col_finalnum = 0;
	std::vector<Num_Type> num_buf;
	std::vector<Complex_Type> complex_buf;
	_My_List_Iter use_iter;
	bool is_num = true;
	if (use_list.empty()) {
		return ++get_iter();
	}
	simply_express(use_list);//化简自身的节点
	for (_My_List_Iter use_iter = use_list.begin(); use_iter != use_list.end(); use_iter++) {
		if ((*use_iter)->get_item() == DIVIDE_OPERATOR) {
			if (col_num != 0) {
				row_num++;
				if (col_finalnum == 0) {
					col_finalnum = col_num;
				}
				else if (col_finalnum != col_num) {
					throw show_err("矩阵列数不统一，无法初始化");
				}
				col_num = 0;
			}
		}
		else {
			Base_Data pdata;
			(*use_iter)->get_data(pdata);
			col_num++;
			switch (pdata.get_type()) {
			case DATA_DOUBLE: {
				Num_Type pres;
				pdata.get_data(pres);
				if (is_num) {
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
				if (is_num) {
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
				if (is_num) {
					complex_buf.assign(num_buf.begin(), num_buf.end());
					is_num = false;
				}
				complex_buf.push_back(pres);
				break;
			}
			default:
				throw show_err("矩阵内不支持此种数据类型：", pdata.get_type());
			}
		}
	}
	
	if (col_finalnum!=0 &&col_num != 0 && col_num != col_finalnum) {
		throw show_err("矩阵列数不统一，无法初始化");
	}
	if (col_finalnum == 0) {
		col_finalnum = col_num;
	}
	if (col_num != 0) {
		row_num++;
	}
	if (is_num) {
		size_t buf_size = num_buf.size();
		Num_Type* pbuf = new Num_Type[buf_size];
		for (size_t i = 0; i < buf_size; i++) {
			pbuf[i] = num_buf[i];
		}
		use_iter = instead(new Noraml_Num(Base_Data(Matrix_Type(row_num, col_finalnum, pbuf, false))));
	}
	else {
		size_t buf_size = complex_buf.size();
		Complex_Type* pbuf = new Complex_Type[buf_size];
		for (size_t i = 0; i < buf_size; i++) {
			pbuf[i] = complex_buf[i];
		}
		use_iter = instead(new Noraml_Num(Base_Data(Cmatrix_Type(row_num, col_finalnum, pbuf, false))));
	}
	return use_iter;
}

void inter::Interpreter::Node_Elem_Num::assign(Base_Data & num_data, Variable_Map * pmap) {
	switch (type) {
	case ARRAY_1:
		reinterpret_cast<_Data_Array*>(paddr)->at(xpos) = num_data.get_real();
		break;
	case MATRIX_1: {
		const auto& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		if (num_data.get_type() == DATA_MATRIX) {
			Matrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("行向量赋值失败，维数不统一");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->loc(xpos, i) = rmat->loc(0, i);
			}
		}
		else {
			Cmatrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("行向量赋值失败，维数不统一");
			}
			Complex_Type* pbuf = new Complex_Type[lmat->col()*lmat->row()];
			for (size_t i = 0; i < lmat->col()*lmat->row(); i++) {
				pbuf[i] = lmat->data()[i];
			}
			for (size_t i = 0; i < lmat->col(); i++) {
				pbuf[xpos*lmat->col() + i] = rmat->loc(0, i);
			}
			pmap->data_change(paddr, Base_Data(Cmatrix_Type(lmat->row(), lmat->col(), pbuf, false)));

		}
		break;
	}
	case CMATRIX_1: {
		const auto& lmat = reinterpret_cast<Cmatrix_Type*>(paddr);
		if (num_data.get_type() == DATA_MATRIX) {
			Matrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("行向量赋值失败，维数不统一");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->loc(xpos, i) = rmat->loc(0, i);
			}
		}
		else {
			Cmatrix_Type* rmat;
			num_data.get_data(rmat);
			if (rmat->col() != lmat->col()) {
				throw show_err("行向量赋值失败，维数不统一");
			}
			for (size_t i = 0; i < rmat->col(); i++) {
				lmat->loc(xpos, i) = rmat->loc(0, i);
			}
		}
		break;
	}
	case MATRIX_2: {
		const auto& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		if (num_data.get_type() == DATA_DOUBLE) {
			Num_Type rnum;
			num_data.get_data(rnum);
			lmat->loc(xpos, ypos) = rnum;
		}
		else if (num_data.get_type() == DATA_BOOL) {
			bool rnum;
			num_data.get_data(rnum);
			lmat->loc(xpos, ypos) = Num_Type(rnum);
		}
		else {
			Complex_Type rnum;
			num_data.get_data(rnum);
			Complex_Type* pbuf = new Complex_Type[lmat->col()*lmat->row()];
			for (size_t i = 0; i < lmat->col()*lmat->row(); i++) {
				pbuf[i] = lmat->data()[i];
			}
			pbuf[xpos*lmat->col() + ypos] = rnum;
			pmap->data_change(paddr, Base_Data(Cmatrix_Type(lmat->row(), lmat->col(), pbuf, false)));
		}
		break;
	}
	case CMATRIX_2: {
		const auto& lmat = reinterpret_cast<Cmatrix_Type*>(paddr);
		if (num_data.get_type() == DATA_DOUBLE) {
			Num_Type rnum;
			num_data.get_data(rnum);
			lmat->loc(xpos, ypos) = rnum;
		}
		else if (num_data.get_type() == DATA_BOOL) {
			bool rnum;
			num_data.get_data(rnum);
			lmat->loc(xpos, ypos) = Num_Type(rnum);
		}
		else {
			Complex_Type rnum;
			num_data.get_data(rnum);
			lmat->loc(xpos, ypos) = rnum;
		}
		break;
	}
	default:
		throw show_err("未知错误");
		break;
	}
}

void inter::Interpreter::Node_Elem_Num::get_data(Base_Data & num_data) {
	switch (type) {
	case ARRAY_1: {
		num_data = reinterpret_cast<_Data_Array*>(paddr)->at(xpos);
		break;
	}
	case MATRIX_1: {
		const auto& lmat = reinterpret_cast<Matrix_Type*>(paddr);
		size_t lcol = lmat->col();
		Num_Type* pbuf = new Num_Type[lcol];
		for (size_t i = 0; i < lcol; i++) {
			pbuf[i] = lmat->loc(xpos, i);
		}
		num_data = Base_Data(Matrix_Type(1, lcol, pbuf, false));
		break;
	}
	case CMATRIX_1: {
		Complex_Type* pbuf = new Complex_Type[reinterpret_cast<Cmatrix_Type*>(paddr)->col()];
		for (size_t i = 0; i < reinterpret_cast<Cmatrix_Type*>(paddr)->col(); i++) {
			pbuf[i] = reinterpret_cast<Cmatrix_Type*>(paddr)->loc(xpos, i);
		}
		num_data = Base_Data(Cmatrix_Type(1, reinterpret_cast<Cmatrix_Type*>(paddr)->col(), pbuf, false));
		break;
	}
	case MATRIX_2: {
		num_data = Base_Data(reinterpret_cast<Matrix_Type*>(paddr)->loc(xpos, ypos));
		break;
	}
	case CMATRIX_2: {
		num_data = Base_Data(reinterpret_cast<Cmatrix_Type*>(paddr)->loc(xpos, ypos));
		break;
	}
	default:
		throw show_err("未知错误");
	}
}
