/********************************************************************************
* @File name: interpreter.h
* @Author: wzh
* @Version: 4.0
* @Date: 2020-7-13
* @Description: ������ѧ���ʽ��ɵ��ַ������н������֧࣬�ֶ�����ѧ���������������
*				֧�ָ�����������������ж��壬֧�־������㡣
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
* @brief ���������Ҫ���ֿռ�
*/
namespace inter {

//-------------------------------------------------------------------
// �������ʵ��
//-------------------------------------------------------------------

/**
* @brief ������ѧ���ʽ��ɵ��ַ������н�������
*	1�����캯��ֻ��һ��ΪĬ�Ϲ��캯��
*	2��ͨ��solve()���������ص��������<<�������ַ�������������
*	3��ͨ��get_result()�������Base_Data���͵����ݻ�ͨ�����ص������
*		��<<���������׼�������
*	4��ͨ������good()�ж��Ƿ�����ɹ�
*		-<em>false</em> ����ʧ�ܣ�����������������Ϣ��Ĭ�ϴ�����Ϊcerr��
*		-<em>true</em>	�����ɹ������������������
*	5��ͨ��get_var_map()������ù����ж���ı�����������Ϊ
*		<string,Base_Data>���͵�map����
*	6��ͨ��clear()�����������ı���
*	7��ͨ��get_const_func()�������֧�ֺ�������������ʽΪ��'\n'�ָ�
*		���ַ���
*	8��ͨ��get_const_opre()�������֧�ֵĲ���������ʽͬ��
*	9��ͨ��set_err()�����ض��������
*	10�����ȼ����� number ,',', '=' , '+' , '-' , '*' , '/' , '%' , '^' , function , '!' , '()'
*/
class Interpreter {
public:
	class Base_Data;
	typedef std::vector<Base_Data> _Data_Array;

    /**
    * @brief ���й���֧�ֵĻ�����������ö��
    */
    enum Data_Type {
        DATA_DOUBLE,			//ʵ��
        DATA_BOOL,              //�߼�����
        DATA_COMPLEX,			//����
		DATA_ARRAY,				//����
		DATA_MATRIX,			//����
		DATA_CMATRIX,			//��������
    };
    /**
    * @brief ͨ�õ�������
	*	
    */
    class Base_Data {//�Զ���Ļ����������ͼ��������������͵Ļ�����
    private:
        void* data;
        bool is_own;
        bool is_init;
        Data_Type d_type;
		void _Malloc(const Base_Data& pData);	//����һ����pData��ʼ�����ڴ�
		void _Free(void);						//�ͷ������������
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
		Base_Data get_shadow() const;			//��ȡ��ǩֵ
		Base_Data get_real() const;				//��ȡ��ʵֵ
		bool is_shadow()const;					//���Ƿ��Ǳ�ǩ�����ж�
		Data_Type get_type() const;				//��ȡ�洢����������
		void get_data(const void* &)const;			//��ȡ��ǩ��ַ
		void get_data(Num_Type&) const;			//��ȡ����
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
    * @brief ��ѯָ���Ĵ洢�ռ����Ƿ���ָ����ֵ,�������򷵻�ƫ�������������ڷ���
            �洢�ռ�����洢��
            �˺���������Ϊ�ж�һ���ַ������ַ��ڴ洢����λ�ã���Ҫ����Ϊ��ѧ������
            ������жϵĺ���
    * @param store:�洢��
             name :ָ��������
    * @return int	ƫ������δѰ�ҵ����ش洢�ռ�����洢��
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
    //��Ԫ�������ֵ��
    struct Binary_Func {
        Char_Type		name;	//��������ַ�ֵ								
        int priority;			//�������ȼ�ֵԽ��Խ��
        Base_Data(*func)(const Base_Data&, const Base_Data&);//����ָ��
    };
    //һԪ�������ֵ��
    struct Unary_Func {
        Char_Type		name;	//��������ַ�ֵ
        int priority;			//�������ȼ�ֵԽ��Խ��
        Base_Data(*func)(const Base_Data&);//����ָ��
    };
    //����������
    struct SingleVar_Func {
        Str_Type name;		//������
        Base_Data(*func)(const Base_Data&);//����ָ��
    };
	//����
    struct Const_Num {
        Str_Type name;
        Base_Data num;
    };
    //���ּ�ֵ�Եĸ���
    static const size_t binary_func_num		= 8;		//��Ԫ���������
    static const size_t singlevar_func_num	= 34;		//��������������
    static const size_t unary_func_num		= 2;		//��Ŀ���������
	static const size_t const_BaseData_num	= 6;		//��������

	//���ȼ����޸Ŀ��ܻᵼ�������쳣������
    static const size_t singlevar_priority	= 7;		//���������������ȼ�
    static const size_t assignment_priority	= 2;		//��ֵ�������ȼ�
    static const size_t comma_priority		= 1;		//�������ȼ�
    static const size_t bracket_prority		= 9;		//�������ȼ�
    
    static const Str_Type result_string;				//��ʾ��һ�����н�����ַ�����Ĭ��Ϊans
    static const std::array<Binary_Func, binary_func_num>binary_func;
    static const std::array<SingleVar_Func, singlevar_func_num>singlevar_func;
    static const std::array<Unary_Func, unary_func_num>unary_func;
    static const std::array<Const_Num, const_BaseData_num>const_BaseData;
    static std::ostream* os_err;
    //��ʾ�ڵ�Ļ�����������,�����˽ڵ㸽�ӵ�ֵ
    enum Base_Type {
        BASE_NUM,			//Base_Data��
        BASE_STRING,		//string��
        BASE_INT,			//int��
        BASE_EMPTY,			//���ͣ�������
        BASE_LIST,			//_My_list��
        BASE_VECTOR			//������
    };
    //��ʾ�ڵ�ľ������ԣ������˽ڵ������
    enum Item_Type {
        BINARY_OPERATOR,	//��Ԫ�����
        STATIC_FUNCTION,	//����������
        UNARY_OPERATOR,		//һԪ�����
        ASSIGNMENT_OPERATOR,//��ֵ�����
        NUM,				//��ֵ�ڵ�
		NUM_ELEME,
		NUM_VARIBLE,
        BRACKET_OPERATOR,	//���������
		SQ_BEACKET_OPERATOR,//������
		DIVIDE_OPERATOR,	//�ָ���
		SPACE_OPERATOR,
        COMMA_OPRERATOR,    //���������
        UNDEF_NAME,			//��δ���������	
    };
    /**
    * @brief �ڵ�Ļ���
    *		���е����й����еĽڵ㶼�ɴ˽ڵ�������������������һ�������๤���Ĵ�������
    *		1�����ȣ���������ַ������봫�뵽����analyze_string()�С�
    *       2����symbol_string()�ж�������ַ������з�����������ķ��룬�������ַ���
    *          �����һ���ɺܶ��Base_Item���������ָ����ɵ�����
    *       3������simply_express()�����������ᰴ��������ÿ���ڵ�����ȼ����δӸߵ���ִ��
    *          ��Ӧ�ڵ�ָ���operation()������ÿ�������Ľڵ㶼Ӧ�������������ʵ���Լ���
    *          ������������Ϊ��
    *       4��ִ����������ʽ��ȷ��������ֻ��ʣ��һ����ֵ�ڵ㣬����ֵ�ڵ㼴Ϊ���ս��
    *       5�������յĽ���洢��final_result�С�
    *       6�����final_result���ȴ���һ�ε��ַ�������
    *          ��Base_Item�������������ڵ�Ļ��࣬�����˹�������Ϊ
    *
    */
    class Base_Item {
    private:
        Base_Type b_type;		//������������
        Item_Type i_type;		//������������
        _My_List* base_list;	//����������
        _My_List_Iter base_Iter;//base_list���Լ��ĵ�����
        int priority;			//���ȼ�
        bool have_master;		//ȷ���Ƿ��д�������
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
    * @brief ��������Ϊ Base_Data��Base_Item ������
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
    * @brief ��������Ϊ string��Base_Item ������
    */
    class Node_String :public Base_Item {//�ַ�������
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
    * @brief ��������Ϊ int��Base_Item ������
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
    * @brief ��������Ϊ�յ�Base_Item ������
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
    * @brief ��������Ϊ _My_List��Base_Item ������
    */
    class Node_List :public Base_Item {
    protected:
        _My_List use_list;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(_My_List* &uselist)override {
            uselist = &use_list;
        }
        Node_List(Item_Type itype, int prio) ://����ʼ��
            Base_Item(BASE_LIST, itype, prio) {}
        Node_List(_My_List* baselist, Item_Type itype, int prio) :
            Base_Item(baselist, BASE_LIST, itype, prio) {}
        virtual ~Node_List() {
            clear_list(use_list);
        }
    };
	/**
	* @brief ��������Ϊ _Data_Array��Base_Item ������
	*/
    class Node_Vector :public Base_Item {
    protected:
        _Data_Array use_vector;
    public:
        virtual _My_List_Iter operation(void)override = 0;
        virtual void get_data(_Data_Array* &usevector)override {
            usevector = &use_vector;
        }
        Node_Vector(Item_Type itype, int prio) ://����ʼ��
            Base_Item(BASE_VECTOR, itype, prio) {}
        Node_Vector(_My_List* baselist,Item_Type itype, int prio) :
            Base_Item(baselist,BASE_VECTOR, itype, prio) {}
        virtual ~Node_Vector() {}
    };


    /**
    * @brief ��Ԫ������Ľڵ�
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
    * @brief ����������Ľڵ㣬��������������
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
    * @brief ���������
    */
    class Comma_Operator :public Node_Empty {
    public:
        Comma_Operator(_My_List* baselist) :
            Node_Empty(baselist, COMMA_OPRERATOR,comma_priority) {};
		virtual _My_List_Iter operation(void)override;
    };
    /**
    * @brief �����������Ľڵ㣬��������������
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
    * @brief һԪ������Ľڵ㣬��������������
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
    * @brief ���������ֵĽڵ㣬��������������
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
    * @brief �����洢���ϸ��һһ��Ӧ�Ĺ�ϵ
        ����ʹ��������map������ʵ�����ֹ�ϵ����ϵͼ����
                    _My_Iter--->\->---string
                      |			 \	   |
             var_p_map|			  \    |var_map<string,Base_Data>
                      |  get_data()\   |
                   void* <--------- Base_Data
        ͨ����������ƣ��ͽ�������string->Base_Data->void*->_My_Iter->string�Ķ�Ӧ��ϵ
        ֻҪ����������������һ��������Ѱ�ҵ�ȫ���ֵ�ԣ�ͨ���������������Ƿ���ĸ�ֵ��
        ȡֵ��
    */
    class Variable_Map {
    private:
        _My_Map var_map;
        _My_Point_Map var_p_map;
    public:
		Base_Data push_new(const Str_Type&pstr, const Base_Data &pdata);//�����±���
		Base_Data data_change(const void* psrc, const Base_Data& pdata);		//�ı����ֵ
		bool find_in(const Str_Type&pstr, Base_Data &pdata);			//�����ַ���Ѱ�ұ���
		bool find_in(const void* paddr, _My_Map_Iter& pIter);
		_My_Map get_main_var(void) {										//��ȡ������
			return var_map;
		}
		void clear(void);												//��ձ�����
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
    * @brief ��ֵ������Ľڵ�
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
    //��ϵ�������ж��Ƿ�Ϊ���ֻ�����
	static bool is_num(int c, size_t name_offset);
	static bool is_name(int c, size_t num_offset);
    Base_Data final_result;	//��һ�ε�������
    bool is_ok;				//��һ������״̬
    Variable_Map main_var;	//������
	size_t symbol_string(_My_List& buf_list, const Char_Type* str);
	void analyze_string(const Str_Type&str, _My_List& buf_list);
public:
	Interpreter();
	/**
	* @brief ��ȡ��һ�ε�����״̬
	* @param ��
	* @return true:�޴���
	false:�д���
	*/
	bool good() {
		return is_ok;
	}
	/**
	* @brief ��ȡ��һ�εĽ��
	* @param ��
	* @return Base_Data:���н��
	*/
	Base_Data get_result() {
		return final_result.get_real();
	}
	Interpreter(const Interpreter& pInter);
	Interpreter& operator=(const Interpreter& pInter);

	/**
	* @brief ��ȡ������
	* @param ��
	* @return _My_Map:������
	*/
	_My_Map get_var_map(void) {
		return main_var.get_main_var();
	}
    /**
    * @brief ����������ʽ���ַ���
    * @param str:Ҫ�������ַ���
    * @return true:�޴���
    *		  false:�д���
    */
	bool slove(const Str_Type& str);
    /**
    * @brief ����洢�ı���
    * @param ��
    * @return ��
    */
	void clear();
	static std::vector<Str_Type> get_const_num();
    /**
    * @brief ��ȡ֧�ֵĺ���
    * @param ��
    * @return Str_Type:�ɺ�������ɵĶ�̬����
    */
	static std::vector<Str_Type> get_const_func();
    /**
    * @brief ��ȡ֧�ֵ������
    * @param ��
    * @return Str_Type:���������ɵĶ�̬����
    */
	static std::vector<Char_Type> get_const_opre();
    /**
    * @brief ���ô��������
    * @param err���µĴ�����
    * @return ��
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
