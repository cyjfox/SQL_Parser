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
	unsigned int loffset;//表达式开始的偏移
	unsigned int roffset;//表达式结束的偏移
	vector<CTreeNode *> *children;//子节点

	CTreeNode()
	{
		loffset = 0;
		roffset = 0;
		children = new vector<CTreeNode *>();
	}
};

//遍历树
void tree_traverse(CTreeNode *proot);

//解析成功返回true,出错失败返回false
bool Parser(const char * const sqlstr);
//case_sense表示是否对大小写敏感，true表示敏感，false表示不敏感
bool GetAllSubString(const char * const str, const char * const substr, vector<unsigned int> *presult, bool case_sense);
bool GetSubStringFromLeft(const char * const str, const char * const substr, unsigned int *poffset, bool case_sense);

/*
从右边在str的0至len个字符内获取第一个目标子串，如果存在目标子串，返回true，否则，返回false
*/
bool GetSubStringFromRight(const char * const str, unsigned int len, const char * const substr, unsigned int *poffset, bool case_sense);
int cyj_strcmp(const char * const str, const char * const substr, unsigned int len, bool case_sense);

//假设，每个SELECT必然有一个FROM配对。并且，用户自定义标识不能与关键字相同，并且，关键字不出现由同一字母重复出现的情况，比如tt,ttt,作为一个关键字
//那么，尝试将每个SELECT和FROM配对（大小写不敏感）。

//上述至少应该改为每个FROM必然有一个select配对，因为有一种语法是select 常量，这时没有FROM配对


// 指定在 SQL Server 2014 的 DELETE、SELECT 和 UPDATE 语句中使用的表、视图、派生表和联接表。 在 SELECT 语句中，FROM 子句是必需的，除非选择列表只包含常量、变量和算术表达式（没有列名）。 

//继续假设，每个from和它对应的select之间是列名，先提取这一部分

//from后面一定是数据源。可以判定后面表达式的值类型是不是数据源。比如两个表连接的表达式返回值的类型仍然是数据源
//首先检索出 from后面的第一个合法元素，包括（即左括号，（左括号只有一个字符可以和from之间没有空格或者有空格，，表名这几种情况

//SQL语句括号具有最高的运算级别，首先扫描括号的配对，执行最里层的括号内的表达式
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
			cout<<"第"<<index<<"个字串的偏移为:"<<(*i)<<endl;
			unsigned int offset_sel;
			if (GetSubStringFromRight(Buf, (*i), "select", &offset_sel, false))//获取此偏移（即该from配对的select，大小写不敏感）
			{
				cout<<"对应的select的偏移为:"<<offset_sel<<endl;
			}
			else
			{
				cout<<"该from没有对应的select"<<endl;
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
检查括号匹配情况，匹配正确返回true，同时构建一棵树返回,检查该树的子节点情况，确定有没有括号及括号的情况，匹配失败返回false
*/
//一个右括号总是跟在左边与它最近的左括号配对

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
		if ((*s1) == '(' && lfound == false)//找到未匹配的左括号
		{			
			child = new CTreeNode();
			loffset = offset + 1;//表达式开始
			lfound = true;
			cout<<"got a left bracket"<<" *s1:"<<*s1<<endl;
			offset++;
			s1++;
			continue;
		}

		if ((*s1) == '(' && lfound == true)//找到里层的左括号
		{
			if (MatchBrackets(s1, child))//如果子串配对成功
			{
				//子串不可能匹配成功，因为父串的左括号还没匹配,如果是一个合法的表达式，肯定有一个右括号还没有匹配。而函数成功的情况只有扫描到末尾，并且之前的匹配全部成功，这种情况只可能发生在最外层的函数中。
				cout<<"sub MatchBrackets returned true which means a error"<<endl;
				return false;
			}
			else//配对失败,对这里的调用来说是正常的
			{
				//应该至少匹配了一个子串
				if (child->children->size() < 1)
				{
					cout<<"sub Matchbrackets not matched!"<<endl;
					return false;
				}
				else
				{
					cout<<"prepare to skip part that had been scanned!"<<endl;
					//取最后一个表达式的右侧偏移，跳过匹配好的子串部分
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
			lfound = false;//一组配对完毕，重置相关标志
			child->loffset = loffset;
			child->roffset = roffset;
			ptree_exp->children->push_back(child);//一个完整的表达式获取完毕，可以添加到父节点
			offset++;
			s1++;
			continue;
		}

		offset++;
		s1++;
	}


	//如果扫描到了末尾
	if (lfound == true)//如果仍然有未配对的左括号，则是一个不合法的表达式
	{
		//遍历树，清除已插入的节点，交给调用函数执行吧
		return false;
	}

	//整个表达式是根节点
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
	//先配对括号
	CTreeNode *tree_exp;
	tree_exp = new CTreeNode();
	if (MatchBrackets(sqlstr, tree_exp))//true表示括号匹配正确，同时返回表达式树，false表示匹配失败
	{
		//遍历树输出
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
		cout<<"括号匹配失败！非法的表达式"<<endl;
		return false;
	}
	//先判断句型（type)，表明整个表达式（即最外层）的句型为select,update,insert等
	//如果是select句型
	//先查找" from"子字串，即前面一个空格连接from（大小写不敏感）形成的字串

	return true;
}
/*
得到所有子串的位置,将每个子串的偏移按顺序填充到一个vector容器中
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
bool GetSubStringFromLeft(const char * const str, const char * const substr, unsigned int *poffset, bool case_sense)
{
	const char *s1;
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
从右边在str的0至len个字符内获取第一个目标子串，如果存在目标子串，返回true，否则，返回false
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
	s1 += len2;//将指针调整到第一个可能的位置
	cout<<"len1 is:"<<len1<<"case_sense is:"<<case_sense<<endl;
	while (len2 >= 0)
	{
		cout<<"s1 is："<<s1<<"substr is:"<<substr<<endl;
		if (cyj_strcmp(s1, substr, len1, case_sense) == STRCMP_SAME)//当前位置字符串是否符合
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
在len个字符中比较两个子串是否相等。
*/
int cyj_strcmp(const char * const str, const char * const substr, unsigned int len, bool case_sense)
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

