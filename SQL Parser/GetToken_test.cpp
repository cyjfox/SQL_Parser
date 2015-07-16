#include <iostream>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

const int STRCMP_SAME = 0;
const int STRCMP_NOTSAME = -1;


class CTreeNode
{
public:
	unsigned int loffset;//���ʽ��ʼ��ƫ��
	unsigned int roffset;//���ʽ������ƫ��
	vector<CTreeNode *> *children;//�ӽڵ�

	CTreeNode()
	{
		loffset = 0;
		roffset = 0;
		children = new vector<CTreeNode *>();
	}
};

//������
void tree_traverse(CTreeNode *proot);

//�����ɹ�����true,����ʧ�ܷ���false
bool Parser(const char * const sqlstr);
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


// ָ���� SQL Server 2014 �� DELETE��SELECT �� UPDATE �����ʹ�õı���ͼ������������ӱ� �� SELECT ����У�FROM �Ӿ��Ǳ���ģ�����ѡ���б�ֻ�����������������������ʽ��û���������� 

//�������裬ÿ��from������Ӧ��select֮��������������ȡ��һ����

//from����һ��������Դ�������ж�������ʽ��ֵ�����ǲ�������Դ���������������ӵı��ʽ����ֵ��������Ȼ������Դ
//���ȼ����� from����ĵ�һ���Ϸ�Ԫ�أ��������������ţ���������ֻ��һ���ַ����Ժ�from֮��û�пո�����пո񣬣������⼸�����

//SQL������ž�����ߵ����㼶������ɨ�����ŵ���ԣ�ִ�������������ڵı��ʽ
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
	
	Parser(Buf);

	cout<<"Buf is "<<Buf<<endl;
	delete pSubStrings;
	return 0;
}

void tree_traverse(CTreeNode *proot)
{
	static int i = 0;
	cout<<"i is:"<<i<<endl;
	cout<<"loffset:"<<proot->loffset<<endl;
	cout<<"roffset:"<<proot->roffset<<endl;
	int n = proot->children->size();
	if (n > 0)
	{
		i++;
		for (int j = 0; j < n; j++)
		{
			tree_traverse(proot->children->at(j));
		}
		i--;

	}

}

/*
�������ƥ�������ƥ����ȷ����true��ͬʱ����һ��������,���������ӽڵ������ȷ����û�����ż����ŵ������ƥ��ʧ�ܷ���false
*/
//һ�����������Ǹ������������������������

bool MatchBrackets(const char * const sqlstr, CTreeNode *ptree_exp)
{
	const char *s1;
	s1 = sqlstr;

	unsigned int offset = 0;

	bool lfound = false;
	CTreeNode *child;

	unsigned int loffset = 0, roffset = 0;
	cout<<"begin scanning"<<endl;
	while (*s1)
	{
		if ((*s1) == '(' && lfound == false)//�ҵ�δƥ���������
		{			
			child = new CTreeNode();
			loffset = offset + 1;//���ʽ��ʼ
			lfound = true;
			cout<<"got a left bracket"<<" *s1:"<<*s1<<endl;
			offset++;
			s1++;
			continue;
		}

		if ((*s1) == '(' && lfound == true)//�ҵ�����������
		{
			if (MatchBrackets(s1, child))//����Ӵ���Գɹ�
			{
				//�Ӵ�������ƥ��ɹ�����Ϊ�����������Ż�ûƥ��,�����һ���Ϸ��ı��ʽ���϶���һ�������Ż�û��ƥ�䡣�������ɹ������ֻ��ɨ�赽ĩβ������֮ǰ��ƥ��ȫ���ɹ����������ֻ���ܷ����������ĺ����С�
				cout<<"sub MatchBrackets returned true which means a error"<<endl;
				return false;
			}
			else//���ʧ��,������ĵ�����˵��������
			{
				//Ӧ������ƥ����һ���Ӵ�
				if (child->children->size() < 1)
				{
					cout<<"sub Matchbrackets not matched!"<<endl;
					return false;
				}
				else
				{
					cout<<"prepare to skip part that had been scanned!"<<endl;
					//ȡ���һ�����ʽ���Ҳ�ƫ�ƣ�����ƥ��õ��Ӵ�����
					unsigned int len1;
					len1 = child->children->at(child->children->size() - 1)->roffset + 1;
					s1 += len1;
					offset += len1;
					cout<<"skipped!len1 is "<<len1<<endl;
				}
			}
			
		}

		if ((*s1) == ')' && lfound == false)
		{
			cout<<"found a right bracket without a left bracket"<<endl;
			return false;
		}

		if ((*s1) == ')' && lfound == true)
		{
			roffset = offset;
			lfound = false;//һ�������ϣ�������ر�־
			child->loffset = loffset;
			child->roffset = roffset;
			ptree_exp->children->push_back(child);//һ�������ı��ʽ��ȡ��ϣ�������ӵ����ڵ�
			offset++;
			s1++;
			continue;
		}

		offset++;
		s1++;
	}


	//���ɨ�赽��ĩβ
	if (lfound == true)//�����Ȼ��δ��Ե������ţ�����һ�����Ϸ��ı��ʽ
	{
		//������������Ѳ���Ľڵ㣬�������ú���ִ�а�
		return false;
	}

	//�������ʽ�Ǹ��ڵ�
	ptree_exp->loffset = 0;
	ptree_exp->roffset = offset;
	cout<<"offset is :"<<offset<<endl;
	cout<<"sqlstr is :"<<sqlstr<<endl;
	cout<<"s1 is :"<<s1<<endl;
	return true;
}

/*
bool MatchBrackets(const char * const sqlstr, CTreeNode *ptree_exp)
{
	vector<unsigned int> loffsets;
	
	const char *s1;
	s1 = sqlstr;
	unsigned int offset;
	offset = 0;
	while (*s1)
	{
		if (*s1 == '(')
		{
			loffsets.push_back(offset);
		}
		else if (*s1 == ')')
		{
			ptree_exp->
		}

}
*/

bool Parser(const char * const sqlstr)
{
	//���������
	CTreeNode *tree_exp;
	tree_exp = new CTreeNode();
	if (MatchBrackets(sqlstr, tree_exp))//true��ʾ����ƥ����ȷ��ͬʱ���ر��ʽ����false��ʾƥ��ʧ��
	{
		//���������
		CTreeNode *currnode;
		currnode = tree_exp;
		
		{
			/*
			cout<<"loffset "<<currnode->loffset<<endl;
			cout<<"roffset "<<currnode->roffset<<endl;

			currnode = currnode->children->at(0);
			cout<<"loffset "<<currnode->loffset<<endl;
			cout<<"roffset "<<currnode->roffset<<endl;
			*/
			tree_traverse(currnode);
		}
	}
	else
	{
		cout<<"����ƥ��ʧ�ܣ��Ƿ��ı��ʽ"<<endl;
		return false;
	}
	//���жϾ��ͣ�type)�������������ʽ��������㣩�ľ���Ϊselect,update,insert��
	//�����select����
	//�Ȳ���" from"���ִ�����ǰ��һ���ո�����from����Сд�����У��γɵ��ִ�

	return true;
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

