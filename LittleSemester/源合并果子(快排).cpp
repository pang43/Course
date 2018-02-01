#include <stdio.h>
#include <stdlib.h>
#define N 10005

int cmp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;//��С��������
}

int main()
{
	int n, ans = 0;
	int a[N] = { 0 };
	scanf_s("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf_s("%d", &a[i]);
	qsort(a + 1, n, sizeof(a[1]), cmp);

	int i, j;

	for (i = 1; i < n; i++)//�ܹ��ϲ�n-1��
	{
		a[i] += a[i + 1];//�ϲ�ǰ�������Ѻ͸�ֵ��ǰ���Ǹ�
		ans += a[i];//�ۼ�
		for (j = i + 2; j <= n && a[j] < a[i]; j++)//�ӵ�ǰλ�ú�����λ�ÿ�ʼ�Ƚϣ��ҵ��ȵ�ǰλ�ô����λ��
		{
			a[j - 1] = a[j];//ǰ��
		}
		a[j - 1] = a[i];//�ѵ�ǰ������
	}

	printf("%d\n", ans);

	return 0;
}
