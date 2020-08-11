/********************************************************************************
* @File name: main.cpp
* @Author: wzh
* @Version: 3.1
* @Date: 2020-5-12
* @Description: ��Interpreter�Ĳ�������
*		1������ʼ����һ�� ��>>�����ţ��˷��ű�ʾ���Խ�������
*		2��������������һ����ѧ���ʽ�����س���ʾ���������֧��18����ѧ������7���������
*			֧�ָ�����������������ж��塣
*		3��ͨ���ֺſ���ʹ������ʽ��˳�����
*		4��֧�����¼�����������
*			==================================
*			|clc		|�������            |
*			----------------------------------
*			|show val	|��ʾ�Ѷ������      |
*			----------------------------------
*			|clear		|�������            |
*			----------------------------------
*			|test		|�������ģʽ        |
*			----------------------------------
*			|read		|��ȡ��һ�εĲ����ļ�|
*			----------------------------------
*			|show func	|��ʾ֧�ֵĺ���      |
*			----------------------------------
*			|show opre	|��ʾ֧�ֵ������	 |
*			==================================
********************************************************************************/
#include "interpreter.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <Windows.h>
#include <sstream>
#define MAIN_TEST
using namespace std;
#ifdef MAIN_TEST
typedef interpret::Str_Type use_string;
bool is_cmd(const use_string& str, const use_string& cmd, const use_string& val = "\n") {
    size_t offset;
    if ((offset = str.find(cmd, 0)) != use_string::npos) {
        if (val == "\n")
            return true;
        offset += cmd.size();
        if (offset >= str.size())
            return false;
        if (!isspace(str[offset]))
            return false;
        if (str.find(val, offset) != use_string::npos)
            return true;
        else
            return false;
    }
    else
        return false;
}
static inline unsigned int rand_range(unsigned int min, unsigned int max) {
    return rand()*time(0) % (max - min + 1) + min;
}
void test_random(int num, use_string&str) {
    const array<interpret::Char_Type, 6>test_opre = { '+','-','*','/','^','%'};
    const array<const use_string, 16>test_func = {
        "sin","cos","tan",
        "asin","acos","atan",
        "abs","log","ln",
        "sinh","cosh","tanh",
        "asinh","acosh","atanh","exp"
    };
    ofstream fout("random_express.txt");
    stringstream ss;
    stack<int>test_end;

    srand(unsigned int(time(0)));
    str.clear();
    ss.clear();
    ss << rand();
    for (int i = 0; i < num; i++) {
        ss << test_opre[rand() % test_opre.size()];
        if (rand() % 10 == 5) {
            ss << test_func[rand() % test_func.size()];
            ss << '(';
            test_end.push(rand_range(i, test_end.size() ? (test_end.top()) : (num - 1)));
        }
        ss << double(rand() / 3.0);
        if (test_end.size() != 0) {
            while (test_end.top() == i) {
                ss << ')';
                test_end.pop();
                if (test_end.size() == 0)
                    break;
            }
        }
    }

    ss >> str;
    fout << str;
    fout.close();
}
int main(int argc, char* argv[])
{
	interpret::Interpreter test;
    use_string str;
    SetConsoleTitle("������(with C++)");
    std::system("color F0");
    while (true)
    {
        cout << ">>";
        getline(cin, str);
        if (is_cmd(str, "help")) {
            cout<< "        1������ʼ����һ�� ��>>�����ţ��˷��ű�ʾ���Խ������롣" << endl
                << "        2��������������һ����ѧ���ʽ�����س���ʾ���������֧��18����ѧ������7���������" << endl
                << "           ֧�ָ�����������������ж��塣����������������C����һ�£���ʱ��֧�����ġ�����" << endl
                << "           ,�Ѿ�������Ϊ�������ַ���ͬ��������Ϊ������" << endl
                << "        3��ͨ���ֺſ���ʹ������ʽ��˳����㡣" << endl
                << "        4��֧�����¼�����������" << endl
                << "            ==================================" << endl
                << "            |clc        |�������            |" << endl
                << "            ----------------------------------" << endl
                << "            |show val   |��ʾ�Ѷ������      |" << endl
                << "            ----------------------------------" << endl
                << "            |clear      |�������            |" << endl
                << "            ----------------------------------" << endl
                << "            |test       |�������ģʽ        |" << endl
                << "            ----------------------------------" << endl
                << "            |read       |��ȡ��һ�εĲ����ļ�|" << endl
                << "            ----------------------------------" << endl
                << "            |show func  |��ʾ֧�ֵĺ���      |" << endl
                << "            ----------------------------------" << endl
                << "            |show opre  |��ʾ֧�ֵ������    |" << endl
                << "            ----------------------------------" << endl
                << "            |show const |��ʾ֧�ֵĳ���      |" << endl
                << "            ==================================" << endl;
        }
        else if (is_cmd(str, "show", "func")) {
            auto&& pIter= interpret::Interpreter::get_const_func();
			for (const auto& use_iter : pIter) {
				cout << use_iter << std::endl;
			}
        }
        else if (is_cmd(str, "show", "opre")) {
			auto&& pIter = interpret::Interpreter::get_const_opre();
			for (const auto& use_iter : pIter) {
				cout << use_iter << ' ';
			}
			cout << std::endl;
        }
        else if (is_cmd(str, "show", "const")) {
			auto&& pIter = interpret::Interpreter::get_const_num();
			for (const auto& use_iter : pIter) {
				cout << use_iter << std::endl;
			}
        }
        else if (is_cmd(str, "show","val")) {
            str.clear();
            auto use_map = test.get_var_map();
            cout << left;
            for (const auto&pmap : use_map) {
                cout.width(20);
                cout << pmap.first;
                cout << pmap.second << endl;
            }
        }
        else if (is_cmd(str, "clear")) {
            test.clear();
        }
        else if (is_cmd(str, "clc")) {
            std::system("cls");
        }
        else if (is_cmd(str, "test")) {
            int num;
            time_t start;
            time_t end;
            cout << "������Ի�����";
            cin >> num;
            cin.get();
            cout << "���ɲ��Ա��ʽ������" << endl;
            test_random(num, str);
            cout << "���ɳɹ���" << endl;
            cout << str << endl;
            cout << "��ʼ���㡣����" << endl;
            start = clock();
            test << str;
            end = clock();
            cout << "ans=\n" << test << endl;
            cout << "����ʱ��: " << (end - start) << "ms" << endl;
        }
		else if (is_cmd(str, "quit")) {
			break;
		}
        else if (is_cmd(str, "read")) {
			ifstream fin;
            time_t start;
            time_t end;
			str.clear();
            cout << "��ʼ�����ļ�������" << endl;
			fin.open("random_express.txt");
			while (!fin.eof())
				str += fin.get();
            fin.close();
            cout << str << endl;
            cout << "��ʼ���㡣����" << endl;
            start = clock();
            test << str;
            end = clock();
            cout << "ans=\n" << test << endl;
            cout << "����ʱ��: " << (end - start) << "ms" << endl;
        }
        else {
            test << str;
            cout << "ans=\n"<<test << endl;
        }

    }
    std::system("pause");
    return 0;
}
#else
//������Ĳ���
#include "matix.h"
template<typename T>
std::ostream& operator<< (std::ostream& os, const interpret::Matrix<T>& pmat) {
	for (size_t i = 0; i < pmat.row(); i++) {
		for (size_t j = 0; j < pmat.col(); j++) {
			os << pmat.at(i, j) << ' ';
		}
		os << std::endl;
	}
	return os;
}
const unsigned int test_row = 8;
const unsigned int test_col = 8;
int main(void) {
	double * ptest = new double[test_col * test_row];
	for (unsigned int i = 0; i < test_row * test_col; i++) {
		ptest[i] = rand() % 10;
	}
	interpret::Matrix<double> test(test_row, test_col, ptest);
	cout << test;
	
	cout << "trans test:" << endl;
	test = test.rev();
	for (unsigned int i = 0; i < test.row(); i++) {
		for (unsigned int j = 0; j < test.col(); j++) {
			cout << test.at(i, j) << ' ';
		}
		cout << endl;
	}
	cout << "det test:" << endl;
	try {
		cout << test.det() << endl;
		cout << "reverse test:" << endl;
		cout << test.inv();
		cout << test*test.inv();
		test /= 6;
		cout << (6 * test);
	}
	catch (std::exception&err) {
		cout << err.what() << endl;;
	}
	system("pause");
}
#endif