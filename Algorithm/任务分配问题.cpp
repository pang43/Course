#include"cstdio"//��׼�������
#include"algorithm"//�㷨
using namespace std;
const int maxn = 15;
const int INF = 0x3f3f3f3f;
int n, cv, ans = INF;
int minx[maxn], mp[maxn][maxn], vis[maxn];

void Search_Tree(int step)
{
	if (step > n)//�ѵ�Ҷ�ӽڵ�ͷ���
	{
		if (cv < ans)ans = cv;
		return;
	}
	for (int i = 1; i <= n; i++)
	{
		if (cv + mp[step][i] < ans && !vis[i])//���µ�ǰ���Ž⣬���Ҹõ�û���ù�
		{
			int minnal = cv + mp[step][i];
			for (int j = 1; j <= n; j++)
				if (j != i && !vis[j])
					minnal += minx[j];
			if (minnal < ans)//���õ������ȵ�ǰ��С�ͼ���������
			{
				vis[i] = 1;
				cv += mp[step][i];
				Search_Tree(step + 1);
				vis[i] = 0;
				cv -= mp[step][i];
			}
		}
	}
}
void Min_line()
{
	for (int i = 1; i <= n; i++)//�ҵ�ÿһ�е���Сֵ
		for (int j = 1; j <= n; j++)
			if (minx[i] > mp[i][j])
				minx[i] = mp[i][j];
}
int main()
{
	memset(minx, INF, sizeof(minx));
	memset(vis, 0, sizeof(vis));
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			scanf("%d", &mp[i][j]);

	Min_line();
	Search_Tree(1);
	printf("%d\n",ans);
	return 0;
}