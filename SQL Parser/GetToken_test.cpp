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
	//���ϴ���߻�ȡ���ַ���
	while (GetSubStringFromLeft(s1, substr, terminchar�� &offset))
	{
		s1 += offset;	//�������ҵ������ַ���
		s1++;	//����һ���ַ���ʼ����

		/*
		tmpList.push_back(offset);	//�����ҵ������ַ���ƫ��
		Count++;	//����ͳ�Ƽ���
		*/
		ResultVector.push_back(offset);
	}

	return &ResultVector;
}