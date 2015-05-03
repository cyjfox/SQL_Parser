#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

const int STRCMP_SAME = 0;
const int STRCMP_NOTSAME = -1;

bool GetAllSubString(char *str, const char * const substr, vector<unsigned int> *presult);
bool GetSubStringFromLeft(char *str, const char * const substr, unsigned int *poffset);
int cyj_strcmp(char *str, const char * const substr, unsigned int len);

int main(int argc, char *argv[])
{
	cout<<"Hello, World!"<<endl;
	char *test = (char *)new fstream();
	fstream file("c:\\test.txt", ios::app | ios::in | ios::out, _SH_DENYWR);
	char *Buf;
	Buf = new char[1024];
	strcpy(Buf, "unread");
	cout<<Buf<<endl;

	vector<unsigned int> *pSubStrings;
	pSubStrings = new vector<unsigned int>();

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
		file.close();

		

		GetAllSubString(Buf, "test", pSubStrings);

		vector<unsigned int>::iterator i, iend;
		iend = pSubStrings->end();
		unsigned int index;
		index = 0;
		for (i = pSubStrings->begin(); i < iend; i++)
		{
			cout<<"��"<<index<<"���ִ���ƫ��Ϊ:"<<(*i)<<endl;
			index++;
		}
		
	}
	

	cout<<"Buf is "<<Buf<<endl;
	delete pSubStrings;
	return 0;
}

bool GetAllSubString(char *str, const char * const substr, vector<unsigned int> *presult)
{
	if ((str == NULL) || (substr == NULL))
	{
		return false;
	}

	/*
	unsigned int Count;
	Count = 0;
	List<unsigned int> tmpList;
	tmpList = new List<unsigned int>();
	*/

	

	unsigned int offset, totaloffset;
	char *s1;
	s1 = str;
	totaloffset = 0;
	//���ϴ���߻�ȡ���ַ���
	while (GetSubStringFromLeft(s1, substr, &offset))
	{
		s1 += offset;	//�������ҵ������ַ���
		totaloffset += offset;	//�����ҵ������ַ���ƫ��
		presult->push_back(totaloffset);
		s1++;	//����һ���ַ���ʼ����
		totaloffset++;		
	}

	return true;
}

bool GetSubStringFromLeft(char *str, const char * const substr, unsigned int *poffset)
{
	char *s1;
	s1 = str;

	unsigned int len1;
	len1 = strlen(substr);
	unsigned int offset;
	offset = 0;
	while (*s1)	//���û���������ַ�0x0����ɨ��
	{
		if (cyj_strcmp(s1, substr, len1) == STRCMP_SAME)	//��ǰλ���ַ����Ƿ����
		{
			*poffset = offset;
			return true;
		}
		
		offset++;
		s1++;
	}
}

int cyj_strcmp(char *str, const char * const substr, unsigned int len)
{
	for (int i = 0; i < len; i++)
	{
		if ((str[i] == '0x0') || (str[i] != substr[i]))
		{
			return STRCMP_NOTSAME;
		}
	}

	return STRCMP_SAME;
}