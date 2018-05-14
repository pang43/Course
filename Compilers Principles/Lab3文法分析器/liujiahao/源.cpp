#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <cctype>
#include <stack>
#include <map>
#include <set>
#include <conio.h>
#include<fstream>
#define MAX 507
#ifndef MAKETREE_H
#define MAKETREE_H
#ifndef TRAVERSAL_H
#define TRAVERSAL_H
#ifndef TREENODE_H
#define TREENODE_H
#define STR_FILENAME "in.txt"
#define STR_ADD_NODE "node"
#define STR_ADD_LEAF "leaf"
#define STR_FATHER_CHILD "fc"
#define STR_DELETE "del"
#define STR_PREORDER "preorder"
#define STR_INORDER "inorder"
#define STR_POSTORDER "postorder"
#define STR_EXIT "exit"
using namespace std;
ofstream fout; 
class CTreeNode;	//ǰ��������tNodeLink��CTreeNode���ຬ�б˴�
struct tNodeLink
{
	CTreeNode *pChild;		//���ӽڵ�ĵ�ַ
	tNodeLink *pNext;
};
class Tree
{
public:
	Tree();
	~Tree();
	int getSum();									//��ȡ�ڵ�����
	void addTreeNode(float value);					//������ͨ�ڵ�
	void addLeafNode(float value);					//����Ҷ�ӽڵ�
	CTreeNode *pNode(int num);						//���ر��Ϊnum�Ľڵ��ַ
	bool fatherChild(int numFather, int numChild);	//�󶨸��ӹ�ϵ
	bool delNode(int num);							//ɾ���ڵ�
	
private:
	int m_sum;				//�ڵ�����
	tNodeLink *m_pHead;		//�ڵ�����ͷ
};
void preorderTraversal(CTreeNode *pRoot);	//�������
void inorderTraversal(CTreeNode *pRoot);	//�������
void postorderTraversal(CTreeNode *pRoot);	//�������



//�ڵ�����ṹ��


//���ڵ���
class CTreeNode
{
public:
	friend class Tree;

	CTreeNode();
	~CTreeNode();
	int getNum();									//��ȡ���
	float getValue();								//��ȡ�ڵ��ֵ
	void setValue(float value);						//��ȡ�ڵ��ֵ
	tNodeLink *getChildHead();						//ȡ�����ӽڵ�����ͷ
	bool hasFatherNode();							//�ж��Ƿ��и��׽ڵ�
	bool setFatherNode(CTreeNode *pFather);			//���ø��׽ڵ�
	virtual bool addChildNode(CTreeNode *pChild);	//���Ӷ��ӽڵ�
	bool delChildNode(CTreeNode *pChild);			//ɾ�����ӽڵ�
	void setLeaf();									//���ó�Ҷ�ӽڵ�

private:
	static int s_max;		//�ѷ���ı�ŵ���Ŀ
	int m_num;				//�ڵ���
	float m_value;			//�ڵ��ֵ
	CTreeNode *m_pFather;	//���׽ڵ�
	tNodeLink *m_pChildHead;//���ӽڵ�����
	bool m_bLeaf;
};

//��Ҷ�ӽڵ��ࣨ�̳����ڵ��ࣩ
class CLeafNode : public CTreeNode
{
public:
	CLeafNode();
	bool addChildNode(CTreeNode *pChild);	//���Ӷ��ӽڵ�
};

#endif
#endif
#endif




char tnode[MAX];
int nodenum;
FILE *fp;
//��д��ĸΪ����ֹ��(���Զ�һ��'�ı��������)��Сд��ĸΪ��ֹ��,��~����epsilon
FILE *g_pFilePointer;

char g_cmd[256];
float g_value;
int g_num, g_child;
Tree g_tree;
Tree::Tree()
{
	m_sum = 0;
	m_pHead = NULL;
}

/*��������*/
Tree::~Tree()
{
	tNodeLink *pCurrent = m_pHead;
	tNodeLink *pNext;

	//�ͷ����еĽڵ�
	while (pCurrent != NULL)
	{
		pNext = pCurrent->pNext;

		delete pCurrent->pChild;
		delete pCurrent;

		pCurrent = pNext;
	}
}

/*��ȡ�ڵ�����*/
int Tree::getSum()
{
	return m_sum;
}

/*������ͨ�ڵ�*/
void Tree::addTreeNode(float value)
{
	tNodeLink **ppLast = &this->m_pHead;
	tNodeLink *pCurrent = m_pHead;
	tNodeLink *pNewLink = new tNodeLink;
	CTreeNode *pNewTreeNode = new CTreeNode;	//����һ����ͨ�ڵ�

	pNewLink->pChild = pNewTreeNode;
	pNewTreeNode->setValue(value);

	//�ҵ����һ���ڵ�
	while (pCurrent != NULL)
	{
		ppLast = &pCurrent->pNext;
		pCurrent = pCurrent->pNext;
	}

	//�����½ڵ�
	*ppLast = pNewLink;
	pNewLink->pNext = NULL;

}

/*����Ҷ�ӽڵ�*/
void Tree::addLeafNode(float value)
{
	tNodeLink **ppLast = &this->m_pHead;
	tNodeLink *pCurrent = m_pHead;
	tNodeLink *pNewLink = new tNodeLink;
	CLeafNode *pNewLeafNode = new CLeafNode;	//����һ��Ҷ�ӽڵ�

	pNewLink->pChild = pNewLeafNode;
	pNewLeafNode->setValue(value);

	//�ҵ����һ���ڵ�
	while (pCurrent != NULL)
	{
		ppLast = &pCurrent->pNext;
		pCurrent = pCurrent->pNext;
	}

	//�����½ڵ�
	*ppLast = pNewLink;
	pNewLink->pNext = NULL;
}

/*���ر��Ϊnum�Ľڵ��ַ*/
CTreeNode *Tree::pNode(int num)
{
	tNodeLink *pCurrent = m_pHead;

	while (pCurrent != NULL)
	{
		if (pCurrent->pChild->getNum() == num) return pCurrent->pChild;

		pCurrent = pCurrent->pNext;
	}

	return NULL;
}

/*�󶨸��ӹ�ϵ*/
bool Tree::fatherChild(int numFather, int numChild)
{
	char flag;
	CTreeNode *pAncestor;
	CTreeNode *pFather = pNode(numFather);
	CTreeNode *pChild = pNode(numChild);

	if (numFather == numChild)
	{
		printf("���ܺ��Լ��������ӹ�ϵ\n");
		return false;
	}

	//���׽ڵ�����Ч
	if (pFather == NULL)
	{
		printf("û�б��Ϊ%d�Ľڵ�\n", numFather);
		return false;
	}

	//���ӽڵ�����Ч
	if (pChild == NULL)
	{
		printf("û�б��Ϊ%d�Ľڵ�\n", numChild);
		return false;
	}
	//����Ƿ��γɻ�·
	pAncestor = pFather->m_pFather;
	while (pAncestor != NULL)
	{	
	
		if (pAncestor == pChild)
		{
			printf("�γɻ�·\n");
			return false;
		}

		pAncestor = pAncestor->m_pFather;
	}
	if (pFather->addChildNode(pChild) == false) return false;
	if (pChild->hasFatherNode() == true)
	{
		printf("���棺���и��׽ڵ㣬�������������ESCȡ��");
		flag = getch();
		if (flag == '\033')
		{
			pFather->delChildNode(pChild);
			return false;
		}
	}

	pChild->setFatherNode(pFather);

	return true;
}

/*ɾ���ڵ�*/
bool Tree::delNode(int num)
{
	tNodeLink **ppLast = &this->m_pHead;
	tNodeLink *pCurrent = this->m_pHead;

	while (pCurrent != NULL)
	{
		if (pCurrent->pChild->getValue() == num)
		{
			*ppLast = pCurrent->pNext;

			delete pCurrent->pChild;
			delete pCurrent;

			return true;
		}

		ppLast = &pCurrent->pNext;
		pCurrent = pCurrent->pNext;
	}

	return false;
}




int i=0;
void preorderTraversal(CTreeNode *pRoot)
{
	for(int j=0;j<i;j++)
	{
		printf("    ");
		fout<<"  ";
	}
	tNodeLink *pCurrend;
	
	if (pRoot != NULL)
	{
		if((tnode[pRoot->getNum()-1]>='A')&&(tnode[pRoot->getNum()-1]<='Z'))
		{
			if(tnode[pRoot->getNum()-1]=='T')
				fout<<"<Type>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='A')
				fout<<"<ARGS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='G')
				fout<<"<FRAGS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='B')
				fout<<"<ALIST>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='U')
				fout<<"<UNION>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='E')
				fout<<"<PROGRAM>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='I')
				fout<<"<IDENT>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='S')
				fout<<"<STMTS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='P')
				fout<<"<PLUS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='O')
				fout<<"<OPER>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='Q')
				fout<<"<CLOS>"<<endl;
		//	fout<<"<"<<tnode[pRoot->getNum()-1]<<">"<<endl;
		printf("<%d>\n", pRoot->getNum());
		pCurrend = pRoot->getChildHead();
		}
		else 
		{
			if(tnode[pRoot->getNum()-1]=='i')
				fout<<"<keyword>int</keyword>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='r')
				fout<<"<keyword>return</keyword>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='m')
				fout<<"<identifier>main</identifier>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='a')
				fout<<"<identifier>a</identifier>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='b')
				fout<<"<identifier>b</identifier>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='(')
				fout<<"<separator>(</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]==')')
				fout<<"<separator>)</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]==',')
				fout<<"<separator>,</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]==';')
				fout<<"<separator>;</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='{')
				fout<<"<separator>{</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='}')
				fout<<"<separator>}</separator>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='+')
				fout<<"<operarator>+</operarator>"<<endl;
		//	fout<<tnode[pRoot->getNum()-1]<<endl;
		printf("<%d>\n", pRoot->getNum());
		pCurrend = pRoot->getChildHead();
		}
		//�ȱ�����
		for(int j=0;j<i;j++)
	{
		printf("    ");
	}
		printf("�ڵ���%5d: %f\n", pRoot->getNum(), pRoot->getValue());
		
		//�ٱ�������
		
		while (pCurrend != NULL)
		{
			i++;
			preorderTraversal(pCurrend->pChild);
			i--;
			pCurrend = pCurrend->pNext;
		}

		if((tnode[pRoot->getNum()-1]>='A')&&(tnode[pRoot->getNum()-1]<='Z'))
		{
			for(int j=0;j<i;j++)
			{
				printf("    ");
				fout<<"  ";
			}
			if(tnode[pRoot->getNum()-1]=='T')
				fout<<"</Type>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='A')
				fout<<"</ARGS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='G')
				fout<<"</FRAGS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='B')
				fout<<"</ALIST>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='U')
				fout<<"</UNION>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='E')
				fout<<"</PROGRAM>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='I')
				fout<<"</IDENT>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='S')
				fout<<"</STMTS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='P')
				fout<<"</PLUS>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='O')
				fout<<"</OPER>"<<endl;
			else if(tnode[pRoot->getNum()-1]=='Q')
				fout<<"</CLOS>"<<endl;
		//	fout<<"</"<<tnode[pRoot->getNum()-1]<<">"<<endl;
		}
		printf("</%d>\n",pRoot->getNum());

	}
}

int CTreeNode::s_max = 0;	//�ѷ���ڵ���Ŀ��ʼ��Ϊ0

CTreeNode::CTreeNode()
{
	m_num = ++s_max;	//Ϊ�ڵ����һ�������ı��
	m_value = 0;		//��ʼ���ڵ��ֵ
	m_pFather = NULL;	//��ʼ�����׽ڵ�Ϊ��
	m_pChildHead = NULL;//��ʼ����������ͷΪ��
	m_bLeaf = false;	//����Ҷ�ӽڵ�
}

CTreeNode::~CTreeNode()
{
	tNodeLink *pCurrent = this->m_pChildHead;
	tNodeLink *pNext;

	//����и��ף����Լ��Ӹ��׵Ķ���������ɾ��
	if (m_pFather != NULL) m_pFather->delChildNode(this);

	//ɾ�������ǵĸ���
	while (pCurrent != NULL)
	{
		pNext = pCurrent->pNext;
		
		//�ͷŶ�������Ŀռ�
		(pCurrent->pChild)->setFatherNode(NULL);
		pCurrent = pNext;
	}
}

/*��ȡ���*/
int CTreeNode::getNum()
{
	return m_num;
}

/*��ȡ�ڵ��ֵ*/
float CTreeNode::getValue()
{
	return m_value;
}

/*���ýڵ��ֵ*/
void CTreeNode::setValue(float value)
{
	m_value = value;
}

/*��ȡ���ӽڵ�����ͷ��ָ��*/
tNodeLink *CTreeNode::getChildHead()
{
	return m_pChildHead;
}

/*�ж��Ƿ��и��׽ڵ�*/
bool CTreeNode::hasFatherNode()
{
	if (m_pFather == NULL) return false;

	return true;
}


//���ø��׽ڵ�

bool CTreeNode::setFatherNode(CTreeNode *pFather)
{
	//���и��׽ڵ�
	if (m_pFather != NULL) m_pFather->delChildNode(this);
	
	//����
	m_pFather = pFather;

	return true;
}

//���Ӷ��ӽڵ�

bool CTreeNode::addChildNode(CTreeNode *pChild)
{
	tNodeLink **ppLast = &this->m_pChildHead;
	tNodeLink *pCurrent = this->m_pChildHead;
	tNodeLink *pNewChild;

	//����ָ��Ϊ��
	if (pChild == NULL)
	{
		printf("���󣺶���ָ��Ϊ��\n");
		return false;
	}

	if (this->m_bLeaf == true)
		return (CLeafNode *)(this)->addChildNode(pChild);
	
	//ָ���������һ������
	while (pCurrent != NULL)
	{
		ppLast = &pCurrent->pNext;
		pCurrent = pCurrent->pNext;
	}
	
	//�����¶���
	pNewChild = new tNodeLink;
	pNewChild->pChild = pChild;
	pNewChild->pNext = NULL;
	*ppLast = pNewChild;

	return true;
}

//ɾ�����ӽڵ�

bool CTreeNode::delChildNode(CTreeNode *pChild)
{
	tNodeLink **ppLast = &this->m_pChildHead;
	tNodeLink *pCurrent = this->m_pChildHead;

	//����ָ��Ϊ��
	if (pChild == NULL)
	{
		printf("���󣺶���ָ��Ϊ��\n");
		return false;
	}

	while (pCurrent != NULL)
	{
		//�ҵ����ӽڵ�
		if (pCurrent->pChild == pChild)
		{
			*ppLast = pCurrent->pNext;
			delete pCurrent;

			return true;
		}

		ppLast = &pCurrent->pNext;
		pCurrent = pCurrent->pNext;
	}

	//û�ҵ�����
	return false;
}

/*���ó�Ҷ�ӽڵ�*/
void CTreeNode::setLeaf()
{
	m_bLeaf = true;
}

/*��������*/
CLeafNode::CLeafNode()
{
	setLeaf();
}

/*
���Ӷ��ӽڵ�
1.������
*/
bool CLeafNode::addChildNode(CTreeNode *pChild)
{
	printf("Ҷ�ӽڵ㲻���ж��ӽڵ�\n");

	return false;
}
class WF
{
    public:
    string left;
    set<string> right;
    WF ( char s[] )
    {
        left = s;
    }
    void print ( )
    {
        printf ( "%s->" , left.c_str() );
        set<string>::iterator it = right.begin();
        if ( right.begin()!= right.end() )
        {
            printf ( "%s" , it->c_str() );
            it++;
        }
        for(; it != right.end() ; it++ )
            printf ( "|%s" , it->c_str() );
        puts("");
    }
    void insert ( char s[] )
    {
        right.insert ( s );
    }
};

map<string,set<char> > first;
map<string,set<char> > follow;
map<string,int> VN_dic;
vector<WF> VN_set;
bool used[MAX];

void dfs ( int x )
{
    if ( used[x] ) return;
    used[x] = 1;
    string& left = VN_set[x].left;
    set<string>& right = VN_set[x].right;
    set<string>::iterator it = right.begin();
    for ( ; it!= right.end() ; it++ )
        for ( int i = 0 ; i < it->length() ; i++ )
        {
            if ( !isupper( it->at(i) ) && it->at(i) != '\'' )
            {
                first[left].insert ( it->at(i) );
                break;
            }
            if ( isupper( it->at(i) ) )
            {
                int y;
                if ( i != it->length()-1 && it->at(i+1) == '\'' )
                    y = VN_dic[it->substr(i,2)]-1;
                else y = VN_dic[it->substr(i,1)]-1;
                string& tleft = VN_set[y].left;
                dfs ( y );
                set<char>& temp = first[tleft];
                set<char>::iterator it1 = temp.begin();
                bool flag = true;
                for ( ; it1 != temp.end() ; it1++ )
                {
                    if ( *it1 == '~' ) flag = false;
                    first[left].insert( *it1 );
                }
                if ( flag ) break;
            }
            else continue;
        }
}

void make_first ( )
{
    memset ( used , 0 , sizeof ( used ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        dfs ( i );
#define DEBUG
#ifdef DEBUG
        puts ("***************FIRST��***********************");
        map<string,set<char> >::iterator it = first.begin();
        for ( ; it != first.end() ; it++ )
        {
            printf ( "FIRST(%s)={" , it->first.c_str() );
            set<char> & temp = it->second;
            set<char>::iterator it1 = temp.begin();
            bool flag = false;
            for ( ; it1 != temp.end() ; it1++ )
            {
                if ( flag ) printf ( "," );
                printf ( "%c" , *it1 );
                flag = true;
            }
            puts ("}");
        }
#endif
}

void append ( const string& str1 , const string& str2 )
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for ( ; it != from.end() ; it++ )
        to.insert ( *it );
}

void make_follow ( )
{
    while ( true )
    {
        bool goon = false;
        for ( int i = 0 ; i < VN_set.size() ; i++ )
        {
            string& left = VN_set[i].left;
            set<string>& right = VN_set[i].right;
            set<string>::iterator it = right.begin();
            for ( ; it!= right.end() ; it++ )
            {
                bool flag = true;
                const string& str = *it;
                for ( int j = it->length()-1 ; j >= 0 ; j-- )
                {
                    if ( str[j] == '\'' )
                    {
                        int x = VN_dic[it->substr(j-1,2)]-1;
                        if ( flag ) 
                        {
                            int tt = follow[it->substr(j-1,2)].size();
                            append ( left , it->substr(j-1,2) );
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if ( tt1 > tt ) goon = true;
                            if ( !VN_set[x].right.count("~" ) ) 
                                flag = false;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper(str[k]) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1,2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert ( *it1 );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1,2)].insert ( str[k] );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt )
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if ( isupper(str[j] ) )
                    {
                        int x = VN_dic[it->substr(j,1)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j,1)].size();
                            append ( left , it->substr(j,1) );
                            if ( !VN_set[x].right.count("~") )
                                flag = false;
                            int tt1 = follow[it->substr(j,1)].size();
                            if ( tt1 > tt ) goon = true;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper( str[k] ) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[it->substr(j,1)].size();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert( *it1 );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert ( str[k] );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if ( !goon ) break;
    }
#define DEBUG
#ifdef DEBUG
    puts ("****************FOLLOW��**********************" );
    map<string,set<char> >::iterator it = follow.begin();
    for ( ; it != follow.end() ; it++ )
    {
        printf ( "FOLLOW(%s)={" , it->first.c_str() );
        set<char> & temp = it->second;
        temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for ( ; it1 != temp.end() ; it1++ )
        {
            if ( flag ) printf ( "," );
            printf ( "%c" , *it1 );
            flag = true;
        }
        puts ("}");
    }
#endif
}

vector<map<char,string> > predict_table;

//���һ���ַ��Ƿ�����һ���ַ�����FIRST����
bool check_first ( const string& text , char ch )
{
    for ( int i = 0 ; i < text.length() ; i++ )
    {
        bool hasEmpty = false;
        if ( !isupper(text[i]) && text[i] != '\'' )
        {
            if ( text[i] != ch ) return false;
            else return true;
        }
        else if ( isupper(text[i] ) )
        {
            string temp;
            if ( i != text.length()-1 && text[i+1] == '\'' )
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for ( ; it != dic.end() ; it++ )
            {
                if ( *it == '~' ) hasEmpty = true;
                if ( *it == ch ) return true;
            }
            if ( !hasEmpty) break;
        }
        else continue;
    }
    return false;
}

//���һ���ַ��Ƿ�����һ���ַ�����FOLLOW����
bool check_follow ( const string& text , char ch )
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for ( ; it != dic.end() ; it++ )
        if ( *it == ch ) return true;
    return false;
}

void make_table ()
{
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < it->length() ; j++ )
                if ( !isupper(it->at(j)) && it->at(j) != '\'' ) 
                {
                    if ( vis[it->at(j)] ) continue;
                    vis[it->at(j)] = true;
                    letter.push_back ( it->at(j) );
                }
    }
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < letter.size() ; j++ )
            {
                //cout << *it << " " <<  letter[j] << endl;
                if ( check_first ( *it , letter[j] ) )
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if ( it->at(0) == '~' && check_follow ( left, letter[j] ))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back ( temp );
    }
#define DEBUG
#ifdef DEBUG
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts ("");
    printf ( "|%9s" , "|" );
    for ( int i = 0 ; i < letter.size() ; i++ )
        printf ( "%5c%5s" , letter[i] , "|" );
    puts("");
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts("");
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        printf ( "|%5s%4s" , VN_set[i].left.c_str() , "|" );
        for ( int j = 0 ; j < letter.size() ; j ++ )
            if ( predict_table[i].count(letter[j] ) )
                printf ( "%7s%3s" , predict_table[i][letter[j]].c_str() , "|" );
            else printf ( "%10s" , "|" );
        puts("");
        for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
            printf ( "-" );
        puts ("");
    }
#endif
}
string stackin[MAX];
int tempnum;
void print ( int steps , stack<string> stk , string src , string wf , int x )
{
    printf ( "%-10d?" , steps );
    string out = "";
    while ( !stk.empty() )
    {
        out = stk.top()+out;
        stk.pop();
    }
    printf ( "#%-9s!" , out.c_str() );
    out ="";
    for ( int i = x ; i < src.length() ; i++ )
        out += src[i];
    printf ( "%-10s@" , (out+"#").c_str() );
	stackin[tempnum]+=wf.c_str();

    printf ( "%-10s&\n" , wf.c_str() );
}

void analyse ( const string& src )
{
    stack<string> stk;
    stk.push ( "E" );
	tnode[nodenum]='E';
	nodenum++;
	fputs("node 1\n",fp);
    int steps = 0;
    int idx = 0;
    printf ( "%-10s%-10s%-10s%-10s\n" , "����","����ջ","���봮","���ò���ʽ" );
    while ( !stk.empty() )
    {
        string u = stk.top();
        string tmp="";
        stk.pop();
        if ( !isupper(u[0]) ) 
        {
            if ( idx == src.length() && u[0] == '~' );
            else if ( src[idx] == u[0] ) 
			{
				
                idx++;
				cout<<"part of tail"<<src[idx]<<endl;
			}
        }
        else 
        {
            int x = VN_dic[u]-1;
            tmp = predict_table[x][src[idx]];
			cout<<"tail"<<tmp<<endl;
            for ( int i = tmp.length()-1 ; i >= 0 ; i-- )
            {
                if ( tmp[i] == '\'' )
                {
                    string v = tmp.substr(i-1,2);
                    stk.push ( v );
                    i--;
                }
                else
                {
                    string v = tmp.substr(i,1);
                    stk.push( v );
                }
            }
			const char *temp1=u.c_str();
			int head=0;
			for(int tempi=nodenum-1;tempi>=0;tempi--)
			{
				if(temp1[0]==tnode[tempi])
				{
					head=tempi+1;
					break;
				}
			}
			int length=tmp.length();
			const char *temp2=tmp.c_str();
			for(int tempi=0;tempi<length;tempi++)
			{
				if(temp2[tempi]>='A'&&temp2[tempi]<='Z')
				{
					tnode[nodenum]=temp2[tempi];
					nodenum++;
					fprintf(fp,"node %d\n",nodenum);
					fprintf(fp,"fc %d %d\n",head,nodenum);
				}
				else 
				{
					tnode[nodenum]=temp2[tempi];
					nodenum++;
					fprintf(fp,"leaf %d\n",nodenum);
					fprintf(fp,"fc %d %d\n",head,nodenum);
				}
			}
            tmp = u+"->"+tmp;
			cout<<"head"<<u<<endl;
        }
		cout<<tmp<<"!!!!!!!!!!!"<<endl;
        print ( steps++ , stk , src , tmp  , idx );
		tempnum++;
    }
}

int main ( )
{
    int n=16;
    char s[MAX];
	fp=fopen("in.txt","w");
	char input[17][MAX]={"E->TI(A){S}","T->i","I->m","I->a","I->b","A->GB","G->TI","B->UG","U->,","S->RP","R->r","P->IQ;","Q->OI","O->+","O->-","O->*","O->/"};
        for ( int i = 0 ; i < n ; i++ )
        {
            strcpy(s,input[i]);
            int len = strlen ( s ),j;
            for ( j = 0 ; j < len ; j++ )
                if ( s[j] == '-' ) break;
            s[j] = 0;
            if ( !VN_dic[s] )
            {
                VN_set.push_back ( s );
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert ( s+j+2 );
        }
        make_first();
        make_follow();
        make_table();
		string in = "im(ia,ib){ra+b;}";
        analyse( in );
		fputs("preorder 1\n",fp);
		fputs("exit\n",fp);
		for(int i=0;i<nodenum;i++)
		{
			printf("%c %d\n",tnode[i],i+1);
		}
    fclose(fp);
	char key;

	//��ֻ����ʽ���ļ��������ļ�ָ��

	g_pFilePointer = fopen(STR_FILENAME, "r");
	fout.open("test.tree.xml");
	//fout<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"<<endl<<"<ParserTree name=\"test.tree.xml\">"<<endl;
	//�ж��ļ�ָ���Ƿ���ȷ
	if(!g_pFilePointer)
	{
		//����ļ�ָ�벻��ȷ������ʾ������Ϣ
		printf("Unable to find or open the file: %s\n", STR_FILENAME);
	}

	do
	{
		memset(g_cmd, 0, sizeof(g_cmd));
		fscanf(g_pFilePointer, "%s", g_cmd);

		if (strcmp(g_cmd, STR_ADD_NODE) == 0)
		{
			fscanf(g_pFilePointer, "%f", &g_value);
			g_tree.addTreeNode(g_value);
			continue;
		}

		if (strcmp(g_cmd, STR_ADD_LEAF) == 0)
		{
			fscanf(g_pFilePointer, "%f", &g_value);
			g_tree.addLeafNode(g_value);
			continue;
		}

		if (strcmp(g_cmd, STR_FATHER_CHILD) == 0)
		{
			fscanf(g_pFilePointer, "%d %d", &g_num, &g_child);
			if (g_tree.fatherChild(g_num, g_child) == true)

			continue;
		}

		if (strcmp(g_cmd, STR_DELETE) == 0)
		{
			fscanf(g_pFilePointer, "%d", &g_num);
			if (g_tree.delNode(g_num) == true) 
			continue;
		}

		if (strcmp(g_cmd, STR_PREORDER) == 0)
		{
			fscanf(g_pFilePointer, "%d", &g_num);
			preorderTraversal(g_tree.pNode(g_num));
			fout.close();
			continue;
		}

	}
	while (strcmp(g_cmd, STR_EXIT) != 0);

//	printf("������˳�����\n");

	key = getch();
}




