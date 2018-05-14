#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<map>
#include<vector>
#include<set>
#include<stack>

using namespace std;

#define path "C:\\Users\\yuheng\\Documents\\College\\Compilers Principles\\Lab3\\rule1"		//��ȡ�ķ���·��
#define buff_MIXN 200						//һ������ʽG����󳤶�
#define table_size 200

map<string, bool>non_terminal;				//���ս����

struct table
{
	map<string, int>action[table_size];
	map<string, int>go[table_size];
}table1;

class Gx									//һ����Ŀ
{
public:
	int loop = 0;							//��¼���λ��
	string left;							//����ʽ��
	vector<string>right;					//����ʽ�Ҳ�
	set<string>search;						//������

public:

	Gx init_G(char* buff);					//���ı��ж���һ������ʽ��������г�ʼ��
}gx;

class Ix									//һ����Ŀ��I
{
private:
	vector<Gx>G;							//һ����Ŀ��I���ж������ʽG
public:

	int unique_I = 0;
	Gx pop_G(int i);						//�Ӹ���Ŀ����ȡ��һ���ķ�
	Gx init_FIRST(Gx &g1, Gx g2);			//��FRST��ǰ�ȳ�ʼ����g2����Ŀ���еĵ�ǰ�g1�����������Ĵ�����
	int size_I();							//������Ŀ���Ĵ�С
	void FIRST(Gx &g1, vector<string>Ba);	//�����Ŀ��first(��a),������������.
	void FIRST_empty(Gx &g1, vector<string>Ba);//�ж�"��"�Ƿ�����first(��a)
	void closure(Gx g);						//����Ŀ���ıհ�
	void push_I(Gx g);						//��ĳһ���ķ�������Ŀ����
	void print_I();							//��ӡ������Ŀ��
	void init_non_terminal();				//��ʼ�����ս����
	void init_search(string a);
}I_list;									//��Ŀ��I������ŵ�ǰ���е�ȫ������ʽ

class Cx
{
private:
	vector<Ix>I;							//һ����Ŀ���淶��C���ж����Ŀ��I
public:

	int size();
	Ix pop_I(int i);						//�Ӹ���Ŀ���淶����ȡ��һ����Ŀ��		
	void push_I(Ix I1);
	void init_I(Gx g);						//��ʼ����Ŀ��Ii�������Ŀ������ӵ�һ���ķ�������հ�
	Ix closure(Ix I1);
	void Goto(Ix I1, set<string>next);		//��C�е�ÿ����Ŀ��I��ÿ���ķ�����x����goto[I, X]�ǿգ��Ҳ���C�����goto[I, X]���뵽C
	
}C;

bool cmp_G(vector<Gx> g1, Gx g2, int G_num);
bool cmp_Gx(Gx g1, Gx g2);
bool same(Ix I1, Ix I3);

Gx Gx::init_G(char* buff)
{
	Gx G_demo;
	int i = 0, k = 0, l = 0;
	char buff_left[buff_MIXN], buff_right[buff_MIXN];
	while (buff[i] != ':')buff_left[i++] = buff[i];
	buff_left[i] = '\0';
	G_demo.left = buff_left;
	while (buff[i] != '\0' && buff[++i] != '\n')
	{
		buff_right[k++] = buff[i];
		if (buff[i] == ' ')
		{
			buff_right[k - 1] = '\0';
			G_demo.right.push_back(buff_right);
			k = 0;
		}
	}
	buff_right[k] = '\0';
	G_demo.right.push_back(buff_right);

	return G_demo;
}


Gx Ix::pop_G(int i)
{
	return G[i];
}

void Ix::print_I()
{
	for (int i = 0; i < G.size(); i++)
	{
		cout << "left:" << G[i].left << "\t\t";
		for (int j = 0; j < G[i].right.size(); j++)
			cout << "right" << j << ":" << G[i].right[j] << " ";
		cout << endl;
	}
}

void Ix::push_I(Gx g)
{
	G.push_back(g);
}

void Ix::init_non_terminal()
{
	for (int i = 0; i < G.size(); i++)
		non_terminal[G[i].left] = true;
}

void Ix::closure(Gx g)
{
	//�жϸ����Ƿ��Ѿ���������Ŀ���У��������ڽ����������Ŀ��
	if (cmp_G(G, g, G.size()) == false)
	{
		G.push_back(g);
		//�жϸ����Ƿ�Ϊ��Լ��Ŀ������ǣ�������չ������ĿҲ������Ŀ����
		if (g.loop!=g.right.size() && non_terminal[g.right[g.loop]] == true)
		{
			//ɨ���ķ������Ǹ÷��ս������Ŀ���������
			for (int i = 0; i < I_list.G.size(); i++)
			{
				if (I_list.G[i].left == g.right[g.loop])
				{
					//�ж��µĴ���չ����Ŀ�Ƿ��Ѿ���������Ŀ���У��������������
					if (cmp_G(G, I_list.G[i], G.size()) == false)
					{
						Gx g0 = I_list.G[i];
						Gx g1 = init_FIRST(g0, g);
						if (non_terminal[I_list.G[i].right[I_list.G[i].loop]] == false)
						{
							G.push_back(g1);
						}
						//�������չ������Ŀ��Ȼ��һ����Լ��Ŀ��������closure
						else if (non_terminal[I_list.G[i].right[I_list.G[i].loop]] == true)
						{
							closure(g1);
						}
					}
				}
			}
		}
	}
}

Gx Ix::init_FIRST(Gx &g1, Gx g2)
{
	//g1�����������Ĵ����g2����Ŀ���еĵ�ǰ��
	vector<vector<string>>Ba;
	vector<string>a;
	set<string>::iterator it;
	//������ǰ���е���������������g1�Ħ²���ƴ����һ��
	for (it = g2.search.begin(); it != g2.search.end(); it++)
	{
		for (int i = g2.loop + 1; i < g2.right.size(); i++)
		{
			a.push_back(g2.right[i]);
		}
		a.push_back(*it);
		Ba.push_back(a);
	}

	/*debug
	Ba[0].clear();
	Ba[0].push_back("A");
	Ba[0].push_back("B");
	Ba[0].push_back("b");
	debug*/

	//g1.search.clear();
	for (int i = 0; i < Ba.size(); i++)
	{
		FIRST(g1, Ba[i]);
		FIRST_empty(g1, Ba[i]);
	}

	return g1;
}

void Ix::FIRST(Gx &g1, vector<string>Ba)
{
	//�����ٲ���ʽ������ң��ҵ��Ը÷��ս����ʼ�Ĳ���ʽ�������ǽ��зֱ����

	//vector<string>::iterator it = Ba.begin();
	for (int i = 0; i < I_list.G.size(); i++)
	{
		if (Ba[0] != "#" && I_list.G[i].left == Ba[0])
		{
			//����Ҳ��Ƴ��գ���Ҫ��󴫵�
			//eg X->ABCD  A->empty  A->aAB,�Ǵ�ʱ���Ǿ�Ҫ��B��first������
			if (I_list.G[i].right[0] == "@")
			{
				vector<string>Ba1 = Ba;
				Ba1.erase(Ba1.begin());
				FIRST(g1, Ba1);
			}
			//�����Ҳ࿪ʼʱ�ս�����ͽ�֮����
			else if (non_terminal[I_list.G[i].right[0]] == false)
			{
				g1.search.insert(I_list.G[i].right[0]);
			}
			//����Ҳ���ʼʱ���ս�����͵ݹ���Ҹ��ս��
			else
			{
				FIRST(g1, I_list.G[i].right);
			}
		}
		else if (Ba[0] != "#" && I_list.G[i].left != Ba[0] && i == (I_list.G.size() - 1) && non_terminal[Ba[0]] == false)
		{
			g1.search.insert(Ba[0]);
		}
		else if (Ba[0] == "#" && g1.search.size() == 0)
		{
			g1.search.insert(Ba[0]);
		}
	}
}

void Ix::FIRST_empty(Gx &g1, vector<string>Ba)
{
	for (int i = 0; i < I_list.size_I(); i++)
	{
		if (I_list.G[i].left == Ba[0])
		{
			//����Ҳ��Ƴ��գ���Ҫ��󴫵�
			//eg X->ABCD  A->empty  A->aAB,�Ǵ�ʱ���Ǿ�Ҫ��B��first������
			if (I_list.G[i].right[0] == "@")
			{
				vector<string>Ba1 = Ba;
				Ba1.erase(Ba1.begin());

				if (Ba1.size() == 0)
				{
					g1.search.insert("@");
					break;
				}
				FIRST_empty(g1, Ba1);
			}
		}
	}
}

void Ix::init_search(string a)
{
	G[0].search.insert(a);
}

int Ix::size_I()
{
	return G.size();
}

Ix Cx::pop_I(int i)
{
	return I[i];
}

void Cx::push_I(Ix I1)
{
	I.push_back(I1);
}

void Cx::init_I(Gx g)
{
	Ix I1;
	I.push_back(I1);
	I.back().closure(g);			//�ѵ�һ���ķ�������Ŀ��Ii������հ�
}

void Cx::Goto(Ix I1, set<string>next)
{
	bool flag = false;
	set<string>::iterator it;
	//��ǰ��Ŀ�����Ծ����ַ���next�����µ���Ŀ���������ɾ������ַ�
	for (it = next.begin(); it != next.end(); it++)
	{
		Ix I2;
		//���next�ǿվ�����
		if (*it == "@")continue;
		//������Ŀ�������ַ�next[i]�����µ���Ŀ��
		for (int i = 0; i < I1.size_I(); i++)
		{
			//�жϸ���Ŀloop�����ַ���next[i]�Ƿ����
			Gx g1 = I1.pop_G(i);
			if (g1.loop<g1.right.size() && g1.right[g1.loop] == *it)
			{
				g1.loop++;						//������ƶ�һλ
				I2.push_I(g1);					//���µ���Ŀ������Ŀ��
			}
		}

		Ix I3 = C.closure(I2);					//����Ŀ��I2��հ��õ��µ���Ŀ��I3
		I3.unique_I = C.size();

		int flag1 = 0;
		for (flag1 = 0; flag1 < C.size(); flag1++)
		{
			if (same(C.pop_I(flag1), I3) == true)
			{
				flag = true;
				break;
			}
		}
		if (flag == true)						//�жϸ���Ŀ���淶���е�ÿһ��Ii����Ŀ��I1�Ƿ����
		{
			flag = false;

			if (non_terminal[*it] == true)
			{
				table1.go[I1.unique_I][*it] = flag1;
			}
			else
			{
				table1.action[I1.unique_I][*it] = flag1;
			}

			if (I3.size_I() == 1 && I3.pop_G(0).loop == I3.size_I())
			{
				Gx g3 = I3.pop_G(0);
				Gx g4 = I3.pop_G(0);
				g3.search.clear();
				g3.loop = 0;
				for (int i = 0; i < I_list.size_I(); i++)
				{
					if (cmp_Gx(I_list.pop_G(i), g3) == true)
					{
						if (g3.left == I_list.pop_G(0).left)
						{
							table1.action[I3.unique_I]["#"] = 254;				//����̬Ϊ254
						}
						else
						{
							set<string>::iterator it1 = g4.search.begin();
							for (it1 = g4.search.begin(); it1 != g4.search.end(); it1++)
							{
								table1.action[I3.unique_I][*it1] = 2000 + i;
							}
						}
					}
				}
			}

			continue;
		}
		C.push_I(I3);							//��Ŀ��I3��Ϊ��Ŀ���淶���һ���µ���Ŀ�����������C

		/*��ӷ�����*/

		if (non_terminal[*it] == true)
		{
			table1.go[I1.unique_I][*it] = I3.unique_I;
		}
		else if (non_terminal[*it] == false)
		{
			table1.action[I1.unique_I][*it] = I3.unique_I;
		}

		/*��ӷ�����*/


		set<string>next1;						//����µ���Ŀ��I3�������ַ���next1���Ե����µ���Ŀ
		for (int i = 0; i < I3.size_I(); i++)
		{
			Gx g_demo = I3.pop_G(i);
			if (g_demo.loop < g_demo.right.size())
				next1.insert(g_demo.right[g_demo.loop]);
		}
		if (next1.size() != 0)
			Goto(I3, next1);
		else
		{
			Gx g3 = I3.pop_G(0);
			Gx g4 = I3.pop_G(0);
			g3.search.clear();
			g3.loop = 0;
			for (int i = 0; i < I_list.size_I(); i++)
			{
				if (cmp_Gx(I_list.pop_G(i), g3) == true)
				{
					if (g3.left == I_list.pop_G(0).left)
					{
						table1.action[I3.unique_I]["#"] = 254;				//����̬Ϊ254
					}
					else
					{
						set<string>::iterator it1 = g4.search.begin();
						for (it1 = g4.search.begin(); it1 != g4.search.end(); it1++)
						{
							table1.action[I3.unique_I][*it1] = 2000 + i;
						}
					}
				}
			}
		}
	}
}

Ix Cx::closure(Ix I1)
{
	vector<Ix>I_chongfu;
	for (int i = 0; i < I1.size_I(); i++)
	{
		Gx g1 = I1.pop_G(i);
		Ix I2;
		I2.closure(g1);
		I_chongfu.push_back(I2);
	}
	Ix I3 = I_chongfu[0];
	for (int i = 1; i < I_chongfu.size(); i++)
	{
		for (int j = 0; j < I_chongfu[i].size_I(); j++)
		{
			int k = 0;
			for (k = 0; k < I3.size_I(); k++)
			{
				if (cmp_Gx(I3.pop_G(k), I_chongfu[i].pop_G(j)) == true)
				{
					break;
				}
			}
			if (k == I3.size_I())
				I3.push_I(I_chongfu[i].pop_G(j));
		}
	}
	return I3;
}

int Cx::size()
{
	return I.size();
}

void init()
{
	FILE* fp = fopen(path, "r");
	char buff[buff_MIXN];
	int i = 0;
	while (fgets(buff, buff_MIXN, fp) != NULL)
	{
		if (buff[0] == '/' || buff[0] == '\n' || buff[0] == ' ')continue;
		I_list.push_I(gx.init_G(buff));
	}

	I_list.init_non_terminal();

	/*for (int i = 0; i < table_size; i++)
	{
		table1.action[i].resize(table_size);
		table1.go[i].resize(table_size);
	}*/

	fclose(fp);
}

bool cmp_G(vector<Gx> g1, Gx g2, int G_num)
{
	for (int i = 0; i < G_num; i++)
	{
		if (g1[i].left == g2.left && g1[i].right.size() == g2.right.size())
		{
			for (int j = 0; j < g2.right.size(); j++)
			{
				if (g1[i].right[j] == g2.right[j] && g1[i].loop == g2.loop && j == (g2.right.size() - 1))
				{
					set<string>::iterator it1 = g1[i].search.begin();
					set<string>::iterator it2 = g2.search.begin();
					for (it2 = g2.search.begin(), it1 = g1[i].search.begin(); it2 != g2.search.end(); it1++, it2++)
					{
						if (*it1 != *it2)return false;
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool cmp_Gx(Gx g1, Gx g2)
{
	if (g1.left == g2.left && g1.right.size() == g2.right.size())
	{
		for (int j = 0; j < g2.right.size(); j++)
		{
			if (g1.right[j] != g2.right[j] || g1.loop != g2.loop)
			{
				return false;
			}
			else if (g1.right[j] == g2.right[j] && g1.loop == g2.loop && j == (g2.right.size() - 1))
			{
				set<string>::iterator it1 = g1.search.begin();
				set<string>::iterator it2 = g2.search.begin();
				for (it2 = g2.search.begin(), it1 = g1.search.begin(); it2 != g2.search.end(); it1++, it2++)
				{
					if (*it1 != *it2)return false;
				}
				return true;
			}
		}
	}
	return false;
}

bool same(Ix I1, Ix I3)
{
	for (int i = 0; i < I3.size_I(); i++)
	{
		for (int j = 0; j < I1.size_I(); j++)
		{
			if (cmp_Gx(I1.pop_G(j), I3.pop_G(i)) == true)
				break;
			else if (j == (I1.size_I() - 1) && cmp_Gx(I1.pop_G(j), I3.pop_G(i)) == false)
				return false;								//I3�е�ĳһ����I1�е�����һ������ͬ��˵������һ���µ���Ŀ��
		}
	}
	return true;
}

//���غ���
void mian_process()
{
	vector<string>str;
	string a;
	cout << "�������ַ���,ÿ���ַ��Իس�����, #��ʾ�������" << endl;
	int i;
	while (cin >> a)
	{
		str.push_back(a);
		if (a == "#")break;
	}

	
	stack<int>stateStack;				//״̬ջ
	stack<string>letterStack;			//����ջ
	
	stateStack.push(0);
	letterStack.push("#");

	while(1)
	{

		//��ǰΪ�ƽ���Ŀ
		if (table1.action[stateStack.top()][str[0]] < 2000)
		{
			stateStack.push(table1.action[stateStack.top()][str[0]]);
			letterStack.push(str[0]);
			str.erase(str.begin());
		}
		//��ǰΪ��Լ��Ŀ
		else
		{
			int symbol = table1.action[stateStack.top()][str[0]] - 2000;
			for (int j = 0; j < I_list.pop_G(symbol).right.size(); j++)
			{
				letterStack.pop();
				stateStack.pop();
			}
			letterStack.push(I_list.pop_G(symbol).left);
			table1.go[stateStack.top()][letterStack.top()];
		}
		if (table1.action[stateStack.top()][str[0]] == 254)
			break;
	}

}

int main()
{
	init();
	//I_list.init_search("#");

	Ix II;
	II.push_I(I_list.pop_G(0));
	II.init_search("#");
	C.init_I(II.pop_G(0));

	Ix I_demo = C.pop_I(0);
	set<string>next;
	for (int i = 0; i < I_demo.size_I(); i++)
	{
		Gx g_demo = I_demo.pop_G(i);
		next.insert(g_demo.right[g_demo.loop]);
	}

	C.Goto(I_demo, next);

	printf("make table succeed\n");

	mian_process();
	return 0;
}