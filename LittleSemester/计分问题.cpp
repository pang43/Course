#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int n;
int cas = 0;

struct team {
	int problem;//���ac���������
	int time;//����ʱ
	int flag[1000];//�������Ƿ���
	int tp[1000];//��ʾ��ŵķ�ʱ
	int tn;//����
	int lastming;//�������
};

int cmp(const void *a, const void *b)
{
	struct team *x = (struct team*)a;
	struct team *y = (struct team*)b;
	if (x->problem == y->problem)
	{
		if (x->time == y->time)
			return x->tn - y->tn;
		return x->time - y->time;//solve���ʱ����time��С������
	}
	return y->problem - x->problem;//��solve�Ӵ�С��
}

int cmp1(const void *a, const void *b)
{
	return *(char *)a - *(char *)b;
}
int main()
{
	while (scanf("%d", &n) != EOF && n)
	{
		struct team china[105];

		memset(china, 0, sizeof(china));

		int tip = 0, name = 0, num = 0, mark = 0;
		char ch1[5], ch2[10] = { 0 };
		mark = strlen(ch1);
		strcpy(ch2, ch1);
		strcat(ch2, ch1);
		if (mark == 5)
		{
			qsort(ch2, 10, sizeof(ch2[0]), cmp1);
		}
		for (int i = 1; i <= n; i++)
			china[i].tn = i;
		while (scanf("%d", &tip) != EOF && tip)//����ÿ�����ݵĿ�ͷ
		{
			scanf("%d%d", &name, &num);
			scanf("%s", ch1);
			china[name].tn = name;//��Ƕ���
			if (ch1[0] == 'y')//ac��
			{
				china[name].problem++;
				china[name].time += tip;
				if (china[name].flag[num] != 0)//��ʾ��name�ӵ�num��wa��
					china[name].time += china[name].tp[num];//���Ϸ�ʱ
			}
			else//wa��
			{
				china[name].tp[num] += 20;
				china[name].flag[num] = 1;//��ʾ��name�ӵ�num��wa��
			}
		}

		qsort(china + 1, n, sizeof(china[1]), cmp);//����

		printf("case %d:\n", ++cas);
		for (int i = 1; i <= n; i++)//����Ŀ�ͷ�ʱһ�����Ŷ��������ɲ���
		{
			if (i != 1 && china[i].problem == china[i - 1].problem && china[i].time == china[i - 1].time)//jiayousuana
				china[i].lastming = china[i - 1].lastming;
			else
				china[i].lastming = i;//���Ǿ�����
		}
		for (int i = 1; i <= n && china[i].lastming <= 3; i++)
			printf("%d %d %d %d\n", china[i].tn, china[i].problem, china[i].time, china[i].lastming);
	}
	return 0;
}