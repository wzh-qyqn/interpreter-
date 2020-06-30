#pragma once
#ifndef __INTERPRETER_ERR_H__
#define __INTERPRETER_ERR_H__
#include <exception>
#include <string>
namespace inter {
	class inter_error :public std::exception {
	public:
		inter_error(const std::string& str = "interpreter error!") :
			std::exception(str.c_str()) {}
	};
	template<typename T>
	static inline void print_err(std::string &err_str, const T& args) {
		std::stringstream ss;
		std::string buf;
		ss << args;
		std::getline(ss, buf);
		err_str += buf;
	}
	template<typename...Args>
	static inline inter_error show_err(const Args&...args) {
		std::string str_buf;
		int arr[] = { (print_err(str_buf,args),0)... };
		return str_buf;
	}
}

#endif

