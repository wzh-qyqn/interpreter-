/********************************************************************************
* @File name: interpreter.h
* @Author: wzh
* @Version: 4.0
* @Date: 2020-7-13
* @Description: 对由数学表达式组成的字符串进行解析的类，支持多种数学函数与种运算符，
*				支持复数运算与变量过程中定义，支持矩阵运算。
********************************************************************************/
#pragma once
#ifndef __INTERPRETER__H__
#define __INTERPRETER__H__
#include <list>
#include <complex>
#include <map>
#include <array>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "interpreter_type.h"
#include "interpreter_err.h"
/**
* @brief 本程序的主要名字空间
*/
namespace inter {

//-------------------------------------------------------------------
// 主体类的实现
//-------------------------------------------------------------------

/**
* @brief 对由数学表达式组成的字符串进行解析的类
*	1、构造函数只有一种为默认构造函数
*	2、通过solve()方法或重载的运算符‘<<’输入字符串到解释器中
*	3、通过get_result()方法获得Base_Data类型的数据或通过重载的运算符
*		‘<<’输出到标准的输出流
*	4、通过方法good()判断是否解析成功
*		-<em>false</em> 解析失败，向错误流输出错误信息（默认错误流为cerr）
*		-<em>true</em>	解析成功，可以正常输出数据
*	5、通过get_var_map()方法获得过程中定义的变量表，变量表为
*		<string,Base_Data>类型的map容器
*	6、通过clear()方法清除定义的变量
*	7、通过get_const_func()方法获得支持函数表，函数表形式为由'\n'分隔
*		的字符串
*	8、通过get_const_opre()方法获得支持的操作符表，形式同上
*	9、通过set_err()方法重定向错误流
*	10、优先级排序 number ,',', '=' , '+' , '-' , '*' , '/' , '%' , '^' , function , '!' , '()'
*/
class Interpreter {
public:
	class Base_Data;
	typedef std::vector<Base_Data> _Data_Array;

    /**
    * @brief 运行过程支持的基础数据类型枚举
    */
    enum Data_Type {
        DATA_DOUBLE,			//实数
        DATA_BOOL,              //逻辑类型
        DATA_COMPLEX,			//复数
		DATA_ARRAY,				//数组
		DATA_MATRIX,			//矩阵
		DATA_CMATRIX,			//复数矩阵
    };
    /**
    * @brief 通用的数据类
	*	
    */
    class Base_Data {//自定义的基础数据类型兼容所有数据类型的基础类
    private:
        void* data;
        bool is_own;
        bool is_init;
        Data_Type d_type;
		void _Malloc(const Base_Data& pData);	//分配一块由pData初始化的内存
		void _Free(void);						//释放所分配的数据
    public:
		explicit Base_Data();
		explicit Base_Data(const Num_Type&);
		explicit Base_Data(const Complex_Type&);
		explicit Base_Data(const bool&);
		explicit Base_Data(const _Data_Array&);
		explicit Base_Data(const Matrix_Type&);
		explicit Base_Data(const Cmatrix_Type&);
		explicit Base_Data(Matrix_Type&&);
		explicit Base_Data(Cmatrix_Type&&);
		explicit Base_Data(_Data_Array&&);
		explicit Base_Data(const Base_Data&);	
		bool operator<(const Base_Data&)const;
		Base_Data& operator=(const Base_Data&);
		Base_Data(Base_Data&&);
		Base_Data& operator=(Base_Data&&);
		friend Ostr_Type& operator<<(Ostr_Type&, const Base_Data&);
		Base_Data get_shadow() const;			//获取标签值
		Base_Data get_real() const;				//获取真实值
		bool is_shadow()const;					//对是否是标签进行判断
		Data_Type get_type() const;				//获取存储的数据类型
		void get_data(const void* &)const;			//获取标签地址
		void get_data(Num_Type&) const;			//获取数据
		void get_data(Complex_Type&)const;
		void get_data(bool&)const;
		void get_data(const _Data_Array*&)const;
		void get_data(const Matrix_Type*&)const;
		void get_data(const Cmatrix_Type*&)const;
		~Base_Data();
    };
private:
    class Base_Item;
    typedef std::list<Base_Item*> _My_List;
    typedef _My_List::iterator _My_List_Iter;
    typedef std::map<Str_Type, Base_Data> _My_Map;
    typedef _My_Map::iterator _My_Map_Iter;
    typedef _Data_Array::iterator _My_Vector_Iter;
    typedef std::map<const void*, _My_Map_Iter> _My_Point_Map;

    /**
    * @brief 查询指定的存储空间内是否有指定的值,若存在则返回偏移量，若不存在返回
            存储空间的最大存储量
            此函数的作用为判读一个字符串或字符在存储区的位置，主要是作为数学函数和
            运算符判断的函数
    * @param store:存储区
             name :指定的名字
    * @return int	偏移量，未寻找到返回存储空间的最大存储量
    */
    template<typename Tx, typename Ty>
    static inline size_t is_in_store(const Tx& store, const Ty& name) {
        size_t i = 0;
        for (; i < store.size(); i++) {
            if (store[i].name == name)
                return i;
        }
        return i;
    }
    //二元运算符键值对
    struct Binary_Func {
        Char_Type		name;	//运算符的字符值								
        int priority;			//运算优先级值越大越高
        Base_Data(*func)(const Base_Data&, const Base_Data&);//函数指针
    };
    //一元运算符键值对
    struct Unary_Func {
        Char_Type		name;	//运算符的字符值
        int priority;			//运算优先级值越大越高
        Base_Data(*func)(const Base_Data&);//函数指针
    };
    //单变量函数
    struct SingleVar_Func {
        Str_Type name;		//函数名
        Base_Data(*func)(const Base_Data&);//函数指针
    };
	//常量
    struct Const_Num {
        Str_Type name;
        Base_Data num;
    };
    //各种键值对的个数
    static const size_t binary_func_num		= 8;		//二元运算符个数
    static const size_t singlevar_func_num	= 34;		//单变量函数个数
    static const size_t unary_func_num		= 2;		//单目运算符个数
	static const size_t const_BaseData_num	= 6;		//常量个数

	//优先级若修改可能会导致语义异常，慎重
    static const size_t singlevar_priority	= 7;		//单变量函数的优先级
    static const size_t assignment_priority	= 2;		//赋值运算优先级
    static const size_t comma_priority		= 1;		//逗号优先级
    static const size_t bracket_prority		= 9;		//括号优先级
    
    static const Str_Type result_string;				//表示上一次运行结果的字符串，默认为ans
    static const std::array<Binary_Func, binary_func_num>binary_func;
    static const std::array<SingleVar_Func, singlevar_func_num>singlevar_func;
    static const std::array<Unary_Func, unary_func_num>unary_func;
    static const std::array<Const_Num, const_BaseData_num>const_BaseData;
    static std::ostream* os_err;
    //表示节点的基础数据类型,代表了节点附加的值
    enum Base_Type {
        BASE_NUM,			//Base_Data型
        BASE_STRING,		//string型
        BASE_INT,			//int型
        BASE_EMPTY,			//空型，无数据
        BASE_LIST,			//_My_list型
        BASE_VECTOR			//数组型
    };
    //表示节点的具体属性，代表了节点的种类
    enum Item_Type {
        BINARY_OPERATOR,	//二元运算符
        STATIC_FUNCTION,	//单变量数据
        UNARY_OPERATOR,		//一元运算符
        ASSIGNMENT_OPERATOR,//赋值运算符
        NUM,				//数值节点
		NUM_ELEME,
		NUM_VARIBLE,
        BRACKET_OPERATOR,	//括号运算符
		SQ_BEACKET_OPERATOR,//方括号
		DIVIDE_OPERATOR,	//分隔符
		SPACE_OPERATOR,
        COMMA_OPRERATOR,    //逗号运算符
        UNDEF_NAME,			//还未定义的名字	
    };
    /**
    * @brief 节点的基类
    *		所有的运行过程中的节点都由此节点派生而来，下面描述一下整个类工作的大致流程
    *		1、首先，将输入的字符串放入传入到函数analyze_string()中。
    *       2、在symbol_string()中对输入的字符串进行分离与最初步的翻译，将整个字符串
    *          翻译成一个由很多个Base_Item的派生类的指针组成的链表。
    *       3、调用simply_express()函数，函数会按照链表上每个节点的优先级依次从高到低执行
    *          对应节点指针的operation()方法，每个派生的节点都应该在这个方法中实现自己的
    *          化简与运算行为。
    *       4、执行完后，若表达式正确，链表中只会剩下一个数值节点，此数值节点即为最终结果
    *       5、将最终的结果存储在final_result中。
    *       6、输出final_result，等待下一次的字符串输入
    *          而Base_Item就是所有派生节点的基类，代表了公共的行为
    *
    */
    class Base_Item {
    private:
        Base_Type b_type;		//基础数据类型
        Item_Type i_type;		//具体数据类型
        _My_List* base_list;	//从属的链表
        _My_List_Iter base_Iter;//base_list中自己的迭代器
        int priority;			//优先级
        bool have_master;		//确定是否有从属链表
    public:
		Base_Item(_My_List* baselist, Base_Type btype, Item_Type itype, int prio);
		Base_Item(Base_Type btype, Item_Type itype, int prio);
        Base_Item(const Base_Item&) = delete;
        Base_Item& operator=(const Base_Item&) = delete;
		Base_Type get_base();
		Item_Type get_item();
		_My_List_Iter get_iter();
		_My_List* get_master();
		virtual void get_data(Base_Data& num_data);
		virtual void get_data(size_t& num);
		virtual void get_data(Str_Type &usestr);
		virtual void get_data(_My_List* &uselist);
		virtual void get_data(_Data_Array* &usevector);
		int get_prio();
		_My_List_Iter change_master(_My_List* uselist);
		_My_List_Iter change_master(_My_List* uselist, _My_List_Iter pIter);
		_My_List_Iter instead(Base_Item* data_item);
        virtual _My_List_Iter operation(void) = 0;
		virtual ~Base_Item();
    };
	static void simply_express(_My_List &uselist);
	static void clear_list(_My_List& uselist);
    /**
    * @brief 附加数据为 Base_Data的Base_Item 派生类
    */
	class Node_Num :public Base_Item {
	protected:
		Base_Data store_num;
	public:
		Node_Num(_My_List* baselist, Item_Type itype, Base_Data&& usenum) :
			Base_Item(baselist,BASE_NUM,itype,0), store_num(std::move(usenum)){}
		Node_Num(Item_Type itype,Base_Data&&usenum) :
			Base_Item(BASE_NUM, itype, 0), store_num(std::move(usenum)) {}
		Node_Num(_My_List* baselist, Item_Type itype, const Base_Data& usenum) :
			Base_Item(baselist, BASE_NUM, itype, 0), store_num(usenum) {};
		Node_Num(Item_Type itype,const Base_Data& usenum) :
			Base_Item(BASE_NUM, itype, 0), store_num(usenum) {};
		virtual void get_data(Base_Data& num_data)override {
			num_data = store_num;
		}
		virtual _My_List_Iter operation(void)override = 0;
	};
    class Noraml_Num :public Node_Num {
    public:
        Noraml_Num(_My_List* baselist,Base_Data&& usenum) :
			Node_Num(baselist, NUM, usenum){};
        Noraml_Num(Base_Data&&usenum):
			Node_Num(NUM, usenum){}
        Noraml_Num(_My_List* baselist, const Base_Data& usenum) :
			Node_Num(baselist, NUM, usenum) {};
		Noraml_Num(const Base_Data& usenum) :
			Node_Num(NUM, usenum) {};
        virtual _My_List_Iter operation(void)override {
            return ++get_iter();
        }
    };
	class Varible_Num :public Node_Num {
	public:
		Varible_Num(_My_List* baselist, Base_Data&& usenum) :
			Node_Num(baselist, NUM_VARIBLE, usenum) {};
		Varible_Num(Base_Data&&usenum) :
			Node_Num(NUM_VARIBLE, usenum) {}
		Varible_Num(_My_List* baselist, const Base_Data& usenum) :
			Node_Num(baselist, NUM_VARIBLE, usenum) {};
		Varible_Num(const Base_Data& usenum) :
			Node_Num(NUM_VARIBLE, usenum) {};
		virtual _My_List_Iter operation(void)override {
			return ++get_iter();
		}
	};
	
    /**
    * @brief 附加数据为 string的Base_Item 派生类
    */
    class Node_String :public Base_Item {//字符串类型
    protected:
        Str_Type mystring;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(Str_Type &usestr)override {
            usestr = mystring;
        }
        Node_String(_My_List* baselist, Item_Type itype, const Str_Type& name) :
            Base_Item(baselist, BASE_STRING, itype,0), mystring(name) {}
        Node_String(Item_Type itype, const Str_Type& name) :
            Base_Item(BASE_STRING, itype,0), mystring(name) {}
        virtual ~Node_String() {}
    };
    /**
    * @brief 附加数据为 int的Base_Item 派生类
    */
    class Node_Int :public Base_Item {
    protected:
        size_t num;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(size_t& use_num)override {
            use_num = num;
        }
        Node_Int(_My_List* baselist, Item_Type itype, size_t usenum, int prio) :
            Base_Item(baselist, BASE_INT, itype, prio), num(usenum) {}
        Node_Int(Item_Type itype, size_t usenum, int prio) :
            Base_Item(BASE_INT, itype, prio), num(usenum) {}
        virtual ~Node_Int() {}
    };
    /**
    * @brief 附加数据为空的Base_Item 派生类
    */
    class Node_Empty :public Base_Item {
    public:
        virtual _My_List_Iter operation(void)override = 0;
        Node_Empty(_My_List* baselist, Item_Type itype, int prio) :
            Base_Item(baselist, BASE_EMPTY, itype, prio) {}
        Node_Empty(Item_Type itype, int prio) :
            Base_Item(BASE_EMPTY, itype, prio) {}
        virtual ~Node_Empty() {}
    };
	
    /**
    * @brief 附加数据为 _My_List的Base_Item 派生类
    */
    class Node_List :public Base_Item {
    protected:
        _My_List use_list;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(_My_List* &uselist)override {
            uselist = &use_list;
        }
        Node_List(Item_Type itype, int prio) ://不初始化
            Base_Item(BASE_LIST, itype, prio) {}
        Node_List(_My_List* baselist, Item_Type itype, int prio) :
            Base_Item(baselist, BASE_LIST, itype, prio) {}
        virtual ~Node_List() {
            clear_list(use_list);
        }
    };
	/**
	* @brief 附加数据为 _Data_Array的Base_Item 派生类
	*/
    class Node_Vector :public Base_Item {
    protected:
        _Data_Array use_vector;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(_Data_Array* &usevector)override {
            usevector = &use_vector;
        }
        Node_Vector(Item_Type itype, int prio) ://不初始化
            Base_Item(BASE_VECTOR, itype, prio) {}
        Node_Vector(_My_List* baselist,Item_Type itype, int prio) :
            Base_Item(baselist,BASE_VECTOR, itype, prio) {}
        virtual ~Node_Vector() {}
    };


    /**
    * @brief 二元运算符的节点
    */
    class Binary_Operator :public Node_Int {
    public:
        Binary_Operator(_My_List* baselist, size_t offset) :
            Node_Int(baselist, BINARY_OPERATOR, offset, binary_func[offset].priority) {}
        Binary_Operator(size_t offset) :
            Node_Int(BINARY_OPERATOR, offset, binary_func[offset].priority) {}
        virtual ~Binary_Operator() {}
		virtual _My_List_Iter operation(void)override;
    };
    /**
    * @brief 括号运算符的节点，过程与上面相似
    */
    class Bracket_Operator :public Node_List {
    public:
        Bracket_Operator(_My_List* baselist) :
            Node_List(baselist, BRACKET_OPERATOR, bracket_prority) {}
		virtual _My_List_Iter operation(void)override;
    };

	class SQ_Bracket_Operator :public Node_List {
	public:
		SQ_Bracket_Operator(_My_List* baselist) :
			Node_List(baselist, BRACKET_OPERATOR, bracket_prority) {}
		virtual _My_List_Iter operation(void)override;
	};

	class Divide_Operator : public Node_Empty {
	public:
		Divide_Operator(_My_List* baselist) :
			Node_Empty(baselist, DIVIDE_OPERATOR, 0){}
		virtual _My_List_Iter operation(void)override {
			return ++get_iter();
		}
	};

	class Space_Opretator : public Node_Empty {
	public:
		Space_Opretator(_My_List* baselist) :
			Node_Empty(baselist, SPACE_OPERATOR, 0) {}
		virtual _My_List_Iter operation(void)override {
			return ++get_iter();
		}
	};
    /**
    * @brief 逗号运算符
    */
    class Comma_Operator :public Node_Empty {
    public:
        Comma_Operator(_My_List* baselist) :
            Node_Empty(baselist, COMMA_OPRERATOR,comma_priority) {};
		virtual _My_List_Iter operation(void)override;
    };
    /**
    * @brief 单变量函数的节点，过程与上面相似
    */
    class Static_Function :public Node_Int {
    public:
        Static_Function(_My_List* baselist, size_t offset) :
            Node_Int(baselist, STATIC_FUNCTION, offset, singlevar_priority) {}
        Static_Function(int offset) :
            Node_Int(STATIC_FUNCTION, offset, singlevar_priority) {}
		virtual _My_List_Iter operation(void)override;
    };
    /**
    * @brief 一元运算符的节点，过程与上面相似
    */
    class Unary_Operator :public Node_Int {
    public:
        Unary_Operator(_My_List* baselist, size_t offset) :
            Node_Int(baselist, UNARY_OPERATOR, offset, unary_func[offset].priority) {}
        Unary_Operator(size_t offset) :
            Node_Int(UNARY_OPERATOR, offset, unary_func[offset].priority) {}
		virtual _My_List_Iter operation(void)override;
    };
    /**
    * @brief 待定义名字的节点，过程与上面相似
    */
    class Undef_Name :public Node_String {
    public:
        Undef_Name(_My_List* uselist, const Str_Type& usestr) :
            Node_String(uselist, UNDEF_NAME, usestr) {}
        Undef_Name(const Str_Type& usestr) :
            Node_String(UNDEF_NAME, usestr) {}
        virtual _My_List_Iter operation(void)override {
            return ++get_iter();
        }
    };
    /**
    * @brief 变量存储表，严格的一一对应的关系
        这里使用了两个map容器来实现这种关系，关系图如下
                    _My_Iter--->\->---string
                      |			 \	   |
             var_p_map|			  \    |var_map<string,Base_Data>
                      |  get_data()\   |
                   void* <--------- Base_Data
        通过这样的设计，就建立起了string->Base_Data->void*->_My_Iter->string的对应关系
        只要有这四种量的任意一个都可以寻找到全体键值对，通过这样可以让我们方便的赋值与
        取值。
    */
    class Variable_Map {
    private:
        _My_Map var_map;
        _My_Point_Map var_p_map;
    public:
		Base_Data push_new(const Str_Type&pstr, const Base_Data &pdata);//填入新变量
		Base_Data data_change(const void* psrc, const Base_Data& pdata);		//改变变量值
		bool find_in(const Str_Type&pstr, Base_Data &pdata);			//根据字符串寻找变量
		bool find_in(const void* paddr, _My_Map_Iter& pIter);
		_My_Map get_main_var(void) {										//获取变量表
			return var_map;
		}
		void clear(void);												//清空变量表
    };
	class Node_Elem_Num :public Node_Num {
	public:
		enum Elem_Type {
			ARRAY_1,
			MATRIX_1,
			MATRIX_2,
			CMATRIX_1,
			CMATRIX_2
		};
	private:
		const size_t xpos;
		const size_t ypos;
		void* paddr;
		const Elem_Type type;
	public:
		Node_Elem_Num(const void* addr, Elem_Type ptype, size_t xupos, size_t yupos = 0) :
			Node_Num(NUM_ELEME,Base_Data()), xpos(xupos), ypos(yupos), type(ptype), paddr(const_cast<void*>(addr)) {}
		Node_Elem_Num(_My_List* baselist, const void* addr, Elem_Type ptype, size_t xupos, size_t yupos = 0) :
			Node_Num(baselist, NUM_ELEME, Base_Data()), xpos(xupos), ypos(yupos), type(ptype), paddr(const_cast<void*>(addr)) {};
		Node_Elem_Num(Base_Data&& pdata, Elem_Type ptype, size_t xupos, size_t yupos = 0) :
			Node_Num(NUM_ELEME, std::move(pdata)), xpos(xupos), ypos(yupos), type(ptype){
			const void* addr;
			store_num.get_data(addr);
			paddr = const_cast<void*>(addr);
		}
		Node_Elem_Num(_My_List* baselist, Base_Data&& pdata, Elem_Type ptype, size_t xupos, size_t yupos = 0) :
			Node_Num(baselist, NUM_ELEME, std::move(pdata)), xpos(xupos), ypos(yupos), type(ptype){
			const void* addr;
			store_num.get_data(addr);
			paddr = const_cast<void*>(addr);
		};
		void assign(Base_Data& num_data, Variable_Map* pmap);
		virtual _My_List_Iter operation(void)override {
			return ++get_iter();
		}
		virtual void get_data(Base_Data& num_data)override;
	};
    /**
    * @brief 赋值运算符的节点
    */
    class Assignment_Operator :public Node_Empty {
    private:
        Variable_Map* base_map;
    public:
        Assignment_Operator(_My_List* uselist, Variable_Map* use_map) :
            Node_Empty(uselist, ASSIGNMENT_OPERATOR, assignment_priority), base_map(use_map) {}
        Assignment_Operator(Variable_Map* use_map) :
            Node_Empty(ASSIGNMENT_OPERATOR, assignment_priority), base_map(use_map) {}
		virtual _My_List_Iter operation(void)override;
    };
    //联系上下文判断是否为数字或名字
	static bool is_num(int c, size_t name_offset);
	static bool is_name(int c, size_t num_offset);
    Base_Data final_result;	//上一次的运算结果
    bool is_ok;				//上一次运行状态
    Variable_Map main_var;	//变量表
	size_t symbol_string(_My_List& buf_list, const Char_Type* str);
	void analyze_string(const Str_Type&str, _My_List& buf_list);
public:
	Interpreter();
	/**
	* @brief 获取上一次的运行状态
	* @param 无
	* @return true:无错误
	false:有错误
	*/
	bool good() {
		return is_ok;
	}
	/**
	* @brief 获取上一次的结果
	* @param 无
	* @return Base_Data:运行结果
	*/
	Base_Data get_result() {
		return final_result.get_real();
	}
	Interpreter(const Interpreter& pInter);
	Interpreter& operator=(const Interpreter& pInter);

	/**
	* @brief 获取变量表
	* @param 无
	* @return _My_Map:变量表
	*/
	_My_Map get_var_map(void) {
		return main_var.get_main_var();
	}
    /**
    * @brief 分析多个表达式的字符串
    * @param str:要分析的字符串
    * @return true:无错误
    *		  false:有错误
    */
	bool slove(const Str_Type& str);
    /**
    * @brief 清除存储的变量
    * @param 无
    * @return 无
    */
	void clear();
	static std::vector<Str_Type> get_const_num();
    /**
    * @brief 获取支持的函数
    * @param 无
    * @return Str_Type:由函数名组成的动态数组
    */
	static std::vector<Str_Type> get_const_func();
    /**
    * @brief 获取支持的运算符
    * @param 无
    * @return Str_Type:由运算符组成的动态数组
    */
	static std::vector<Char_Type> get_const_opre();
    /**
    * @brief 重置错误输出流
    * @param err：新的错误流
    * @return 无
    */
	static void set_err(std::ostream& err) {
		os_err = &err;
	}
    void operator<<(const Str_Type& str) {
        slove(str);
    }
    friend Ostr_Type& operator<< (Ostr_Type& os, const Interpreter& pInter) {
        if (pInter.is_ok) {
            os << pInter.final_result;
        }
        else {
            os << "error";
        }
        return os;
    }
};

}

#endif
