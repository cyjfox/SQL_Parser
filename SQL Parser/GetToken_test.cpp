#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

const int STRCMP_SAME = 0;
const int STRCMP_NOTSAME = -1;

//case_sense��ʾ�Ƿ�Դ�Сд���У�true��ʾ���У�false��ʾ������
bool GetAllSubString(const char * const str, const char * const substr, vector<unsigned int> *presult, bool case_sense);
bool GetSubStringFromLeft(const char * const str, const char * const substr, unsigned int *poffset, bool case_sense);

/*
���ұ���str��0��len���ַ��ڻ�ȡ��һ��Ŀ���Ӵ����������Ŀ���Ӵ�������true�����򣬷���false
*/
bool GetSubStringFromRight(const char * const str, unsigned int len, const char * const substr, unsigned int *poffset, bool case_sense);
int cyj_strcmp(const char * const str, const char * const substr, unsigned int len, bool case_sense);

//���裬ÿ��SELECT��Ȼ��һ��FROM��ԡ����ң��û��Զ����ʶ������ؼ�����ͬ�����ң��ؼ��ֲ�������ͬһ��ĸ�ظ����ֵ����������tt,ttt,��Ϊһ���ؼ���
//��ô�����Խ�ÿ��SELECT��FROM��ԣ���Сд�����У���

//��������Ӧ�ø�Ϊÿ��FROM��Ȼ��һ��select��ԣ���Ϊ��һ���﷨��select ��������ʱû��FROM���

int main(int argc, char *argv[])
{
	cout<<"Hello, World!"<<endl;
	char *test = (char *)new fstream();
	fstream file("e:\\\\git\\SQL Parser\\test.txt", ios::app | ios::in | ios::out, _SH_DENYWR);
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
		//cin>>buf1;
		//GetAllSubString(Buf, buf1, pSubStrings, false);
		GetAllSubString(Buf, " from", pSubStrings, false);
		vector<unsigned int>::iterator i, iend;
		iend = pSubStrings->end();
		unsigned int index;
		index = 0;
		for (i = pSubStrings->begin(); i < iend; i++)
		{
			cout<<"��"<<index<<"���ִ���ƫ��Ϊ:"<<(*i)<<endl;
			unsigned int offset_sel;
			if (GetSubStringFromRight(Buf, (*i), "select", &offset_sel, false))//��ȡ��ƫ�ƣ�����from��Ե�select����Сд�����У�
			{
				cout<<"��Ӧ��select��ƫ��Ϊ:"<<offset_sel<<endl;
			}
			else
			{
				cout<<"��fromû�ж�Ӧ��select"<<endl;
			}

			index++;
		}
		
	}
	

	cout<<"Buf is "<<Buf<<endl;
	delete pSubStrings;
	return 0;
}

/*
�õ������Ӵ���λ��,��ÿ���Ӵ���ƫ�ư�˳����䵽һ��vector������
*/
bool GetAllSubString(const char * const str, const char * const substr, vector<unsigned int> *presult, bool case_sense)
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
	const char *s1;
	s1 = str;
	totaloffset = 0;
	//���ϴ���߻�ȡ���ַ���
	while (GetSubStringFromLeft(s1, substr, &offset, case_sense))
	{
		cout<<"get one"<<endl;
		s1 += offset;	//�������ҵ������ַ���
		totaloffset += offset;	//�����ҵ������ַ���ƫ��
		presult->push_back(totaloffset);
		s1++;	//����һ���ַ���ʼ����
		totaloffset++;		
	}

	return true;
}

/*
����߻�ȡ��һ��Ŀ���Ӵ�,�������Ŀ���Ӵ�������true,���򣬷���false
*/
bool GetSubStringFromLeft(const char * const str, const char * const substr, unsigned int *poffset, bool case_sense)
{
	const char *s1;
	s1 = str;

	unsigned int len1;
	len1 = strlen(substr);
	unsigned int offset;
	offset = 0;
	while (*s1)	//���û���������ַ�0x0����ɨ��
	{
		//cout<<"scaning..."<<endl;
		if (cyj_strcmp(s1, substr, len1, case_sense) == STRCMP_SAME)	//��ǰλ���ַ����Ƿ����
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
���ұ���str��0��len���ַ��ڻ�ȡ��һ��Ŀ���Ӵ����������Ŀ���Ӵ�������true�����򣬷���false
*/
bool GetSubStringFromRight(const char * const str, unsigned int len, const char * const substr, unsigned int *poffset, bool case_sense)
{
	const char *s1;
	s1 = str;;
	
	unsigned int len1;
	len1 = strlen(substr);
	unsigned int offset;
	offset = 0;

	int len2;
	len2 = len - len1;
	s1 += len2;//��ָ���������һ�����ܵ�λ��
	cout<<"len1 is:"<<len1<<"case_sense is:"<<case_sense<<endl;
	while (len2 >= 0)
	{
		cout<<"s1 is��"<<s1<<"substr is:"<<substr<<endl;
		if (cyj_strcmp(s1, substr, len1, case_sense) == STRCMP_SAME)//��ǰλ���ַ����Ƿ����
		{
			cout<<"got it(right)"<<endl;
			*poffset = len2;
			return true;
		}

		len2--;
		s1--;
	}
	return false;
}

/*
��len���ַ��бȽ������Ӵ��Ƿ���ȡ�
*/
int cyj_strcmp(const char * const str, const char * const substr, unsigned int len, bool case_sense)
{
	//cout<<"str is :"<<str<<endl;
	//cout<<"substr is :"<<substr<<endl;
	//cout<<"len is :"<<len<<endl;
	//�����Сд����
	if (case_sense)
	{
		for (int i = 0; i < len; i++)
		{
			//if ((str[i] == '0x0') || (str[i] != substr[i]))
			//�߼�Ϊ��len���ַ��ڽ��бȽϣ���ʹ�м������0x0
			if (str[i] != substr[i])
			{
				//cout<<"i is "<<i<<" str[i] is "<<str[i]<<" substr[i] is "<<substr[i]<<endl;
				return STRCMP_NOTSAME;
			}
		}
	}
	else//�����Сд������
	{
		for (int i = 0; i < len; i++)
		{
			char c1, c2;
			if (str[i] >= 'a' && str[i] <= 'z')
			{
				c1 = (char)((int)str[i] - 32);//��Сд��ĸת���ɴ�д��ĸ
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

