#include <iostream>
#include <fstream>
#include <list>

using namespace std;

int main(int argc, char *argv[])
{
	cout<<"Hello, World!"<<endl;
	char *test = (char *)new fstream();
	fstream file("c:\\test.txt", ios::app | ios::in | ios::out, _SH_DENYWR);
	char *Buf;
	Buf = new char[1024];
	strcpy(Buf, "unread");
	cout<<Buf<<endl;
	if (file.fail())
	//if (true)
	{
		cout<<"open file failed!"<<endl;
		return -1;
	}
	else
	{
		cout<<"open file succeeded!"<<endl;
		file<<"helloworld1234567890"<<endl;
		cout<<"written"<<endl;
		cout<<"get pointer is "<<file.tellg()<<endl;
		file.seekg(0);
		cout<<"get pointer is "<<file.tellg()<<endl;
		file.getline(Buf, 1024, '\0');
		//file>>Buf;
		file.close();
	}
	
	cout<<"Buf is "<<Buf<<endl;
	return 0;
}

Vector<DWORD> *GetAllSubString(char *str, const char * const substr, char terminchar)
{
	if ((str == NULL) || (substr == NULL))
	{
		return NULL;
	}

	/*
	DWORD Count;
	Count = 0;
	List<DWORD> tmpList;
	tmpList = new List<DWORD>();
	*/

	Vector<DWORD> ResultVector;
	ResultVector = new Vector<DWORD>

	DWORD offset;
	char *s1;
	s1 = str;
	//不断从左边获取子字符串
	while (GetSubStringFromLeft(s1, substr, terminchar， &offset))
	{
		s1 += offset;	//跳到已找到的子字符串
		s1++;	//从下一个字符开始搜索

		/*
		tmpList.push_back(offset);	//保存找到的子字符串偏移
		Count++;	//更新统计计数
		*/
		ResultVector.push_back(offset);
	}

	return &ResultVector;
}