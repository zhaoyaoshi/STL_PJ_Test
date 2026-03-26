#include<iostream>
#include"deque.h"
using namespace std;
int main()
{
	deque<int> que;

	que.push_back(1);

	que.push_back(2);
	que.push_back(3);
	que.push_back(4);


	que.print();

}

