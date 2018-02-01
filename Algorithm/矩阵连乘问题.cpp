#include"cstdio"//��׼�������
#include"cmath"//math
#include"iostream"//���������
#include"algorithm"//�㷨
#include"stack"//ջ
#include"functional"
#include"cstring"//�ַ�������
#include"string"//���ĸ��ֶ���
#include"vector"
#include"queue"//����
#include"map"//ӳ��,����������һ��һ
#include"set"
#include"ctime"
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const double esp = 1e-6;
const int maxn = 1000;
const int INF = 0x3f3f3f3f;

int dp[maxn][maxn], p[maxn], s[maxn][maxn];
int n;
void form()
{
	for (int i = 1; i <= n; i++)
		dp[i][i] = 0;
	for (int r = 2; r <= n; r++)
		for (int i = 1; i <= n - r + 1; i++)
		{
			int j = i + r - 1;
			dp[i][j] = dp[i + 1][j] + p[i - 1] * p[i] * p[j];
			s[i][j] = i;
			for (int k = i + 1; k < j; k++)
			{
				int t = dp[i][k] + dp[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (t < dp[i][j])
				{
					dp[i][j] = t;
					s[i][j] = k;
				}	
			}
		}
}
void score(int i, int j)
{
	if (i == j)
	{
		printf("A%d", i);
		return;
	}
	printf("(");
	score(i, s[i][j]);
	score(s[i][j] + 1, j);
	printf(")");
}
int main()
{
	scanf("%d", &n);
	for (int i = 0; i <= n; i++)
		scanf("%d", &p[i]);
	if (n == 1)
	{
		printf("0\n(A1)\n");
		return 0;
	}
	form();
	printf("%d\n", dp[1][n]);
	score(1, n);
	printf("\n");
}