#include<stdio.h>
int main()
{
	int num,count,total,n;
	count=1;
	total=0;
	do
	{
		printf("������μӿ�������%d_\b",n);
		scanf("%d",&n);
		printf("�������%d��ͬѧ�ĳɼ�_\b",count);
		scanf("%d",&num);
		if(num>100||num<=0)
		{
			printf("���ǹ��㣬���и���"); 
		}
		else
		{
	 	count++;
		total+=num;
		}
	}
	while(count<n);
	printf("��10λͬѧ��ƽ���ɼ���%d\n",total/10);
	return 0;
}
