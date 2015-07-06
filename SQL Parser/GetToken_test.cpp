#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

const int STRCMP_SAME = 0;
const int STRCMP_NOTSAME = -1;

//case_sense表示是否对大小写敏感，true表示敏感，false表示不敏感
bool GetAllSubString(char *str, const char * const substr, vector<unsigned int> *presult, bool case_sense);
bool GetSubStringFromLeft(char *str, const char * const substr, unsigned int *poffset, bool case_sense);
int cyj_strcmp(char *str, const char * const substr, unsigned int len, bool case_sense);

//假设，每个SELECT必然有一个FROM配对。并且，用户自定义标识不能与关键字相同，并且，关键字不出现由同一字母重复出现的情况，比如tt,ttt,作为一个关键字
//那么，尝试将每个SELECT和FROM配对（大小写不敏感）。

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
		//file<<"helloworld1234567890"<<endl;
		//cout<<"written"<<endl;
		cout<<"get pointer is "<<file.tellg()<<endl;
		file.seekg(0);
		cout<<"get pointer is "<<file.tellg()<<endl;
		file.getline(Buf, 1024, '\0');
		cout<<"Buf now is :"<<Buf<<endl;
		file.close();

		
		char buf1[1024];
		cin>>buf1;
		//GetAllSubString(Buf, buf1, pSubStrings, false);
		GetAllSubString(Buf, " from ", pSubStrings, false);
		vector<unsigned int>::iterator i, iend;
		iend = pSubStrings->end();
		unsigned int index;
		index = 0;
		for (i = pSubStrings->begin(); i < iend; i++)
		{
			cout<<"第"<<index<<"个字串的偏移为:"<<(*i)<<endl;
			index++;
		}
		
	}
	

	cout<<"Buf is "<<Buf<<endl;
	delete pSubStrings;
	return 0;
}

/*
得到所有子串的位置,将每个子串的偏移按顺序填充到一个vector容器中
*/
bool GetAllSubString(char *str, const char * const substr, vector<unsigned int> *presult, bool case_sense)
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
	//不断从左边获取子字符串
	while (GetSubStringFromLeft(s1, substr, &offset, case_sense))
	{
		cout<<"get one"<<endl;
		s1 += offset;	//跳到已找到的子字符串
		totaloffset += offset;	//保存找到的子字符串偏移
		presult->push_back(totaloffset);
		s1++;	//从下一个字符开始搜索
		totaloffset++;		
	}

	return true;
}

/*
从左边获取第一个目标子串,如果存在目标子串，返回true,否则，返回false
*/
bool GetSubStringFromLeft(char *str, const char * const substr, unsigned int *poffset, bool case_sense)
{
	char *s1;
	s1 = str;

	unsigned int len1;
	len1 = strlen(substr);
	unsigned int offset;
	offset = 0;
	while (*s1)	//如果没遇到结束字符0x0，则扫描
	{
		//cout<<"scaning..."<<endl;
		if (cyj_strcmp(s1, substr, len1, case_sense) == STRCMP_SAME)	//当前位置字符串是否符合
		{
			cout<<"got it"<<endl;
			*poffset = offset;
			return true;
		}
		
		offset++;
		s1++;
	}
	return false;
}

/*
在len个字符中比较两个子串是否相等。
*/
int cyj_strcmp(char *str, const char * const substr, unsigned int len, bool case_sense)
{
	//cout<<"str is :"<<str<<endl;
	//cout<<"substr is :"<<substr<<endl;
	//cout<<"len is :"<<len<<endl;
	//如果大小写敏感
	if (case_sense)
	{
		for (int i = 0; i < len; i++)
		{
			//if ((str[i] == '0x0') || (str[i] != substr[i]))
			//逻辑为在len个字符内进行比较，即使中间包括有0x0
			if (str[i] != substr[i])
			{
				//cout<<"i is "<<i<<" str[i] is "<<str[i]<<" substr[i] is "<<substr[i]<<endl;
				return STRCMP_NOTSAME;
			}
		}
	}
	else//如果大小写不敏感
	{
		for (int i = 0; i < len; i++)
		{
			char c1, c2;
			if (str[i] >= 'a' && str[i] <= 'z')
			{
				c1 = (char)((int)str[i] - 32);//将小写字母转换成大写字母
			}
			else
			{
				c1 = str[i];
			}

			if (substr[i] >= 'a' && substr[i] <= 'z')
			{
				c2 = (char)((int)substr[i] - 32);
			}
			else
			{
				c2 = substr[i];
			}

			if (c1 != c2)
			{
				return STRCMP_NOTSAME;
			}
		}
	}

	return STRCMP_SAME;
}

