#include<stdio.h>
int main()
{
	int n, T;
	double str[105];
	double time, w, r, x, t;
	int s, e, v, i, j;
	scanf_s("%d", &T);
	while (T--)
	{
		time = 0;
		for (j = 0; j < 105; j++)
		{
			str[j] = 0;
		}
		scanf_s("%lf %lf %lf %lf %d", &x, &w, &r, &t, &n);
		str[0] = x;
		for (j = 0; j < n; j++)   //���ٶȲ�ͬ���� 
		{
			scanf_s("%d %d %d", &s, &e, &v);
			str[v] += e - s;     //��ͬ�ٶȴ��ʹ��ľ��� 
			str[0] -= e - s;     //��·���� 
		}
		for (j = 0; j < 105; j++)
		{
			if (str[j] != 0 && t*(r + j) > str[j]) //���� 
			{
				time += str[j] / (r + j);
				t -= str[j] / (r + j);
			}
			else if (str[j] != 0)  //��һ�� 
			{
				time += (str[j] - t*(r + j)) / (j + w) + t;
				t = 0;
			}

		}
		printf("%.6lf\n", time);
	}
}
