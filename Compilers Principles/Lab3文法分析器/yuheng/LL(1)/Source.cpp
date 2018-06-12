#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<map>
#include<vector>
#include<set>
#include<stack>
#include <iomanip>
#include"tinyxml2.h"


using namespace std;
using namespace tinyxml2;

#define xmlpath "C:\\Users\\yuheng\\Desktop\\LL(1)\\test.xml"
//#define path "C:\\Users\\yuheng\\Desktop\\LL(1)\\test.txt"
#define path "C:\\Users\\yuheng\\Desktop\\LL(1)\\LL(1).txt"
#define buff_MIXN 200
#define table_MIXN 200


class Gx									//һ����Ŀ
{
public:
	string left;							//����ʽ��
	vector<string>right;					//����ʽ�Ҳ�

public:
	Gx initG(char* buff);					//���ı��ж���һ������ʽ��������г�ʼ��(�������Ҳ�)
};

struct analysis
{
	string str;
	bool flag = false;
}g;


vector<Gx>I_list;
set<string>nonTerminal;						//���ս����
set<string>Terminal;						//�ս����
map<string, bool>non_terminal;				//���ս����
map<string, set<string>>first;				//first��
map<string, set<string>>follow;				//follow��
map<string, map<string, int>>tableMap;		//������

Gx Gx::initG(char* buff)					//���ǳ�ʼ������������ʼ���ķ��ģ����ù�ʵ�ֹ���
{
	Gx G_temp;
	int i = 0, k = 0, l = 0;
	char buff_left[buff_MIXN], buff_right[buff_MIXN];
	while (buff[i] != ':')buff_left[i++] = buff[i];
	buff_left[i] = '\0';
	G_temp.left = buff_left;
	while (buff[i] != '\0' && buff[++i] != '\n')
	{
		buff_right[k++] = buff[i];
		if (buff[i] == ' ')
		{
			buff_right[k - 1] = '\0';
			G_temp.right.push_back(buff_right);
			k = 0;
		}
	}
	buff_right[k] = '\0';
	G_temp.right.push_back(buff_right);

	return G_temp;
}

void init()
{
	FILE* fp = fopen(path, "r");
	if (fp == NULL) cout << "The path is not exist" << endl;
	else
	{
		char buff[buff_MIXN];
		int i = 0;
		Gx G_temp;
		while (fgets(buff, buff_MIXN, fp) != NULL)
		{
			if (buff[0] == '/' || buff[0] == '\n' || buff[0] == ' ')continue;
			I_list.push_back(G_temp.initG(buff));
		}
		for (int j = 0; j < I_list.size(); j++)
		{
			non_terminal[I_list[j].left] = true;
			nonTerminal.insert(I_list[j].left);
		}
		for (int j = 0; j < I_list.size(); j++)
		{
			for (int k = 0; k < I_list[j].right.size(); k++)
			{
				if (non_terminal[I_list[j].right[k]] == false)
					Terminal.insert(I_list[j].right[k]);
			}
		}
		Terminal.insert("#");
	}

	fclose(fp);

}

//����first��
void calFirst(string g)
{
	int tag = 0;
	int flag = 0;
	for (int i = 0; i < I_list.size(); i++)
	{
		if (I_list[i].left == g)			//ƥ�����ʽ�Ҳ�
		{
			if (non_terminal[I_list[i].right[0]] == false)		//�����ս����ֱ�Ӽ���first
			{
				first[g].insert(I_list[i].right[0]);
			}
			else
			{
				for (int j = 0; j < I_list[i].right.size(); j++)
				{
					if (non_terminal[I_list[i].right[j]] == false)//�ս������
					{
						first[g].insert(I_list[i].right[j]);
						break;
					}
					calFirst(I_list[i].right[j]);				//�ݹ�

					set<string>::iterator it;
					for (it = first[I_list[i].right[j]].begin(); it != first[I_list[i].right[j]].end(); it++)
					{
						if (*it == "@") flag = 1;
						else
							first[g].insert(*it);				//��first(Y)�е÷�@����first(g)
					}
					if (flag == 0)break;
					else
					{
						tag += flag;
						flag = 0;
					}
				}
				if (tag == I_list[i].right.size())				//���е��Ҳ�first(y)����@����@����first(g)
				{
					first[g].insert("@");
				}
			}
		}
	}
}

//����follow��
void calFollow(string g)
{
	for (int i = 0; i < I_list.size(); i++)
	{
		int index = -1;
		int len = I_list[i].right.size();
		for (int j = 0; j < len; j++)
		{
			if (I_list[i].right[j] == g)				//Ѱ�Һ��и��ַ��ò���ʽ
			{
				index = j;
				break;
			}
		}
		if (index != -1 && index < len - 1)
		{
			string next = I_list[i].right[index + 1];
			if (non_terminal[next] == false)			//g����õ�һ���ַ�������ս���ͽ������g��follow����
				follow[g].insert(next);
			else
			{
				int isExt = 0;
				set<string>::iterator it;
				for (it = first[next].begin(); it != first[next].end(); it++)
				{
					//��A->aBb��һ������ʽ,���FIRST(b)\{��}����FOLLOW(B)��;
					if (*it == "@")
						isExt = 1;
					else
						follow[g].insert(*it);
				}
				//��A->aBb��һ������ʽ��b=>��(���š�FIRST(b))���FOLLOW(A)����FOLLOW(B)��
				if (isExt && I_list[i].left != g)
				{
					calFollow(I_list[i].left);
					set<string>::iterator it;
					string temp = I_list[i].left;
					for (it = follow[temp].begin(); it != follow[temp].end(); it++)
						follow[g].insert(*it);
				}

			}
		}

		//��A->aB��һ������ʽ,��b=>��(���š�FIRST(b))���FOLLOW(A)����FOLLOW(B)��
		else if (index != -1 && index == len - 1 && g != I_list[i].left)
		{
			calFollow(I_list[i].left);
			set<string>::iterator it;
			string temp = I_list[i].left;
			for (it = follow[temp].begin(); it != follow[temp].end(); it++)
				follow[g].insert(*it);
		}
		
	}
}

//����LL(1)������
void makeTable()
{
	for (int i = 0; i < I_list.size(); i++)
	{
		string temp = I_list[i].right[0];
		if (non_terminal[temp] == false)
		{
			if (temp != "@")
				tableMap[I_list[i].left][temp] = i;
			if (temp == "@")
			{
				set<string>::iterator it;
				for (it = follow[I_list[i].left].begin(); it != follow[I_list[i].left].end(); it++)
				{
					tableMap[I_list[i].left][*it] = i;
				}
			}
		}
		else
		{
			set<string>::iterator it1;
			for (it1 = first[temp].begin(); it1 != first[temp].end(); it1++)
			{
				tableMap[I_list[i].left][*it1] = i;
			}
			if (first[temp].count("@") != 0)
			{
				set<string>::iterator it;
				for (it = follow[I_list[i].left].begin(); it != follow[I_list[i].left].end(); it++)
				{
					tableMap[I_list[i].left][*it] = i;
				}
			}
		}
	}
}

void showAnalysis(stack<analysis>analyStr, vector<string>remainStr, vector<string>::iterator remainStrIt, int number)
{
	cout << "������������������������������������������������������������������������������������������������" << endl;
	cout << "����ջ��";
	while (!analyStr.empty())
	{
		cout << setw(10) << analyStr.top().str;
		analyStr.pop();
	}
	cout << endl;

	vector<string>::iterator tempIt;
	
	cout << "ʣ�����봮��";
	for (tempIt = remainStr.begin(); tempIt != remainStr.end(); tempIt++)
	{
		cout << setw(10) << *tempIt;
	}
	cout << endl;

	cout << "����ʽ��ţ�" << number << endl;
}

//�ܿس���
void analyExp(vector<string>remainStr)
{
	int number;
	stack<analysis>Str;							//����ջ
	vector<string>remainStrCopy;				//�������봮�ĸ��������������������
	remainStrCopy = remainStr;

	//��ʼ��
	g.str = "#";
	g.flag = false;
	Str.push(g);
	g.str = I_list[0].left;
	g.flag = false;
	Str.push(g);
	vector<string>::iterator it;
	it = remainStr.begin();

	//д��xml�ļ�
	ofstream fp;
	fp.open(xmlpath);
	fp << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
	fp << "<config name=\"test.xml\">" << endl;
	fp << "<" << Str.top().str << ">" << endl;
	string data = Str.top().str;

	while (1)
	{
		number = 255;
		//����ǰ����ջջ������X�����봮ʣ���һ��ai�����ķ����ս����
		if (non_terminal[Str.top().str] == false && non_terminal[remainStr[0]] == false)
		{
			//X=ai=# ��ʾ�����ɹ���ֹͣ��������
			if (Str.top().str == *it && *it == "#")
			{
				cout << "analysis succeed!" << endl;
				break;
			}
			//X=ai��=# ��X�ӷ���ջ���˵���ʣ�മ����ƶ�
			else if (Str.top().str == *it && *it != "#")
			{
				fp << Str.top().str << endl;
				//fp << "</" << Str.top().str << ">" << endl;

				Str.pop();
				it++;
				remainStrCopy.erase(remainStrCopy.begin());
			}
			else
			{
				cout << "analysis failed!" << endl;
				break;
			}
		}
		//��X���ڷ��ս������ѯ������
		else if (non_terminal[Str.top().str] == true)
		{
			//��tableMap[X,ai]��Ϊ�հף���ִ�г��������
			if (tableMap[Str.top().str][*it] < 0 || tableMap[Str.top().str][*it] >= I_list.size())
			{
				cout << "The table is NULL" << endl;
				break;
			}
			//��tableMap[X,ai]��Ϊһ������ʽ������X��ջ�е��������˹����Ҳ��÷������а������ƽ�ջ��������ʽ����Ϊx->@,��x��ջ�е���
			else
			{
				string analyNonTerminal = Str.top().str;

				if (Str.top().flag == true)
				{
					fp << "</" << Str.top().str << ">" << endl;
					Str.pop();
				}
				else
				{
					Str.top().flag = true;
					if (I_list[tableMap[analyNonTerminal][*it]].right[0] != "@")
					{
						number = tableMap[analyNonTerminal][*it];
						for (int i = I_list[tableMap[analyNonTerminal][*it]].right.size() - 1; i >= 0; i--)
						{
							analysis temp_G;
							temp_G.flag = false;
							temp_G.str = I_list[tableMap[analyNonTerminal][*it]].right[i];
							Str.push(temp_G);
							if (non_terminal[Str.top().str] == true)
								fp << "<" << Str.top().str << ">" << endl;
						}
					}
				}
			}
		}
		//showAnalysis(Str, remainStrCopy, it, number);
	}

	fp << "</config>" << endl;
}

void input()
{
	vector<string>remainStr;					//�������봮
	string ss;
	cout << "Waiting input, end with #" << endl;

	do
	{
		cin >> ss;
		remainStr.push_back(ss);
	} while (ss != "#");

	//��������ַ����������ջ
	analyExp(remainStr);
}

void showFirst()
{
	cout << "==========First��==========" << endl;
	map<string, set<string>>::iterator it;
	for (it = first.begin(); it != first.end(); it++)
	{
		cout << "first(" << it->first << ")" << "{";
		set<string>::iterator it1;
		int i = 0;
		for (it1 = it->second.begin(); it1 != it->second.end(); it1++)
		{
			if (i != it->second.size() - 1)
			{
				cout << *it1 << ",";
				i++;
			}
			else cout << *it1;
		}

		cout << "}" << endl;
	}
	cout << "==========First��==========" << endl;
	cout << endl;
}

void showFollow()
{
	cout << "==========Follow��==========" << endl;
	map<string, set<string>>::iterator it;
	for (it = follow.begin(); it != follow.end(); it++)
	{
		cout << "follow(" << it->first << ")" << "{";
		set<string>::iterator it1;
		int i = 0;
		for (it1 = it->second.begin(); it1 != it->second.end(); it1++)
		{
			if (i != it->second.size() - 1)
			{
				cout << *it1 << ",";
				i++;
			}
			else cout << *it1;
		}

		cout << "}" << endl;
	}
	cout << "==========Follow��==========" << endl;
	cout << endl;
}

void showTable()
{
	cout << "==========������==========" << endl;

	set<string>::iterator it;
	int i = 0;
	for (it = Terminal.begin(); it != Terminal.end(); it++)
		cout << setw(20) << *it;
	cout << endl;

	map<string, map<string, int>>::iterator it1;
	for (it1 = tableMap.begin(); it1 != tableMap.end(); it1++)
	{
		cout << it1->first;
		for (it = Terminal.begin(); it != Terminal.end(); it++)
		{
			int flag = 0;
			map<string, int>::iterator it2;
			for (it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
			{
				if (*it == it2->first)
				{
					flag = 1;
					cout << setw(20) << tableMap[it1->first][*it];
				}
			}
			if(flag == 0)cout << setw(20) << "error";
		}
		cout << endl;
	}

	cout << "==========������==========" << endl;
}


int main()
{
	init();

	set<string>::iterator it;
	for (it = nonTerminal.begin();it!=nonTerminal.end();it++)
	{
		calFirst(*it);
	}

	follow[I_list[0].left].insert("#");
	for (it = nonTerminal.begin(); it != nonTerminal.end(); it++)
	{
		calFollow(*it);
	}

	makeTable();
	
	//showFirst();
	//showFollow();
	//showTable();

	input();

	return 0;
}