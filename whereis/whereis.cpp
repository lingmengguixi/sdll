// whereis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <io.h>


using namespace std;
char * nextPath(char *&paths) {
	while (*paths == ';') paths++;
	int len=0;
	while (*paths != ';'&&*paths!='\0') {
		paths++;
		len++;
	}
	if (len == 0) return NULL;

	char *path = new char[len + 1];
	memcpy(path, paths - len, sizeof(char)*len);
	path[len] = 0;
	if (path[len - 1] == '\\' || path[len - 1] == '/') {
		path[len - 1] = 0;
	}
	return path;
}
char temp[10000];
int count1 = 0;
void find(char *dir,char *key) {
	sprintf(temp, "%s\\*%s*", dir, key);
	_finddata_t data;
	long handle=_findfirst(temp,&data);
	if (handle != -1l) {
		do {
			 count1++;
			 sprintf(temp, "%s\\%s", dir,data.name);
			 cout << count1 << ":" << temp << endl;
		} while (_findnext(handle, &data) == 0);
	}
}
int main(int len,char *argv[])
{
	char key[10000];
	memset(key, 0, sizeof(key));
	if (len == 1) {
		cout << "输入关键字" << endl;
		cin >> key;
	}else if (len != 2) {
		cout << "语法:whereis [key]" << endl;
		cout << "key:查询的关键字" << endl;
		return -1;
	}else {
		memcpy(key, argv[1], sizeof(char)*strlen(argv[1]));
	}
	cout << "key is:[" << key << "]" << endl;
	char *paths = getenv("path");
	char *dir;
	while ((dir=nextPath(paths))!=NULL) {
		find(dir, key);
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
