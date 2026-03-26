#include <iostream>
using namespace std;
#include <list>

class Student
{
public:

	Student(const char* name)
	{
		m_name = new char[strlen(name) + 1];
		strcpy(m_name, name);

	}
	~Student()
	{
		if (m_name != NULL)
		{
			// 释放使用 new 关键字分配的内存
			delete[] m_name;
			// 释放内存后的指针置空 避免野指针
			m_name = NULL;
			// 将年龄字段设置为默认值
		}
	}


	Student(const Student& obj)
	{
		// 为 m_name 指针分配内存
		m_name = new char[strlen(obj.m_name) + 1];
		// 拷贝字符串数据
		// 需添加 #define _CRT_SECURE_NO_WARNINGS 声明
		strcpy(m_name, obj.m_name);
		// 设置年龄

	}


	Student& operator=(const Student& obj)
	{
		//先释放 调用者 本身的 m_name 指针指向的内存
		if (m_name != NULL)
		{
			// 使用 new 分配的内存需要使用 delete 释放
			delete[] m_name;
			// 释放内存后指针置空避免野指针
			m_name = NULL;
			// 年龄也设置为默认值

		}

		// 重新分配新的 字符串 内存
		m_name = new char[strlen(obj.m_name) + 1];
		// 拷贝字符串数据
		// 需添加 #define _CRT_SECURE_NO_WARNINGS 声明
		strcpy(m_name, obj.m_name);
		// 拷贝年龄

		// 返回调用者本身, 以便进行链式调用
		return *this;
	}

private:
	// 姓名
	char* m_name;


};

int main1() {

	Student s("Tom");
	list<Student> vec;
	vec.insert(vec.begin(),s);
	

	return 0;
};
