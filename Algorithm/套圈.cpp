#include"cstdio"//��׼�������
#include"cmath"//math
#include"iostream"//���������
#include"algorithm"//�㷨
#include"iomanip"//����λ�����
#include"functional"

using namespace std;
typedef long long ll;
const int maxn = 100000 + 10;
const int maxs = 50000;

struct localTag
{
	double x, y;
}local[maxn], p1[maxs], p2[maxs];

bool cmpx(localTag a, localTag b)
{
	return a.x < b.x;
}
bool cmpy(localTag a, localTag b)
{
	return a.y > b.y;
}
double dis(localTag a, localTag b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
double select(int l, int r)
{
	if (l == r - 2)//ʣ������
		return min(dis(local[l], local[l + 1]), min(dis(local[l], local[r]), dis(local[l + 1], local[r])));
	else if (l == r - 1)//ʣ������
		return dis(local[l], local[r]);
	int mid = (l + r) / 2;
	double d = min(select(l, mid), select(mid + 1, r));

	int cntl = 0, cntr = 0;
	for (int i = l; i <= r; i++)
	{
		if (fabs(local[i].x - local[mid].x) <= d)
		{
			if (i <= mid) p1[cntl++] = local[i];
			else p2[cntr++] = local[i];
		}
	}
	sort(p1, p1 + cntl, cmpy);
	sort(p2, p2 + cntr, cmpy);
	for (int i = 0; i < cntl; i++)
		for (int j = 0; j < cntr; j++) d = min(d, dis(p1[i], p2[j]));
	return d;
}
int main(int arg, char* argv[])
{
	int n;
	while (~scanf("%d", &n), n)
	{
		for (int i = 0; i < n; i++)
			scanf("%lf %lf", &local[i].x, &local[i].y);
		sort(local, local + n, cmpx);//��x��һ��
		double d = select(0, n - 1);
		printf("%.2f\n", d / 2);
	}
	return 0;
}