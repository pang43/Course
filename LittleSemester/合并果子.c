#include<stdio.h>
int main()
{
	int fruit[10005] = { 0 }, a[10005] = { 0 };
	int sum = 0, type, temp;
	scanf_s("%d", &type);//������ӵ������� 
	for (int i = 0; i < type; i++)
	{
		scanf_s("%d", &fruit[i]);
	} //ÿ�ֹ��ӵ�����

	int i, j;
	for (j = type - 1; j>0; j--)
	{
		for (i = 1; i<j; i++)
		{
			if (fruit[0]>fruit[i])//�����i��ȵ�һ��С���ͽ��������λ�ã����������е�����С�ķ��ڵ�һ�� 
			{
				temp = fruit[0];
				fruit[0] = fruit[i];
				fruit[i] = temp;
			}
		}
		for (i = 1; i<j; i++)//�����i+1��ȵڶ���С���ͽ��������λ�ã����������е����ڶ�С�ķ��ڵڶ��� 
		{
			if (fruit[1]>fruit[i + 1])
			{
				temp = fruit[1];
				fruit[1] = fruit[i + 1];
				fruit[i + 1] = temp;
			}
		}
		fruit[0] = fruit[0] + fruit[1];//��С����ϲ� 
		fruit[1] = fruit[j];//�����һ���ֵ����һ�
		sum = sum + fruit[0];
	}
	printf("%d", sum);
}
